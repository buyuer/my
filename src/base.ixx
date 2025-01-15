/**
 * @author  dingddding@163.com
 * @date    2024.11.02
 * @brief   bass struct
 */

module;
#include <any>
#include <array>
#include <cassert>
#include <complex>
#include <concepts>
#include <map>
#include <string>
#include <string_view>
#include <variant>
export module myt.base;

import myt.common;

export namespace myt {

class Type {
public:
  enum ID : uint8 {
    NONE      = 0,
    SINT8     = 1,
    UINT8     = 2,
    SINT16    = 3,
    UINT16    = 4,
    SINT32    = 5,
    UINT32    = 6,
    SINT64    = 7,
    UINT64    = 8,
    FLOAT8    = 9,
    FLOAT16   = 10,
    FLOAT32   = 11,
    FLOAT64   = 12,
    COMPLEX8  = 13,
    COMPLEX16 = 14,
    COMPLEX32 = 15,
    COMPLEX64 = 16,
    SUM       = 17,
  };

  Type()                        = default;
  ~Type()                       = default;
  Type(const Type &)            = default;
  Type(Type &&)                 = default;
  Type &operator=(const Type &) = default;
  Type &operator=(Type &&)      = default;

  template <typename T>
    requires std::integral<T> or std::same_as<T, ID>
  Type(T id) : id_(id) {
    if (not(id > NONE and id < SUM)) {
      id = NONE;
    }
  }

  ID id() const { return id_; }

  sizeT size() const { return info_table[id_].size; }

  const std::string_view &name() const { return info_table[id_].name; }

private:
  ID id_{};
  struct TypeInfo {
    sizeT                  size;
    const std::string_view name{};
  };
  static constexpr TypeInfo info_table[]{
      {0, "none"},
      {sizeof(sint8), "sint8"},
      {sizeof(uint8), "uint8"},
      {sizeof(sint16), "sint16"},
      {sizeof(uint16), "uint16"},
      {sizeof(sint32), "sint32"},
      {sizeof(uint32), "uint32"},
      {sizeof(sint64), "sint64"},
      {sizeof(uint64), "uint64"},
      {sizeof(f8), "float8"},
      {sizeof(f16), "float16"},
      {sizeof(f32), "float32"},
      {sizeof(f64), "float64"},
      {sizeof(cpx8), "complex8"},
      {sizeof(cpx32), "complex16"},
      {sizeof(cpx32), "complex32"},
      {sizeof(cpx64), "complex64"},
  };
};

class Shape {
public:
  using size_type = myt::sizeT;

  Shape()                         = default;
  ~Shape()                        = default;
  Shape(const Shape &)            = default;
  Shape(Shape &&)                 = default;
  Shape &operator=(const Shape &) = default;
  Shape &operator=(Shape &&)      = default;

  template <typename T>
    requires std::integral<T>
  Shape(const std::initializer_list<T> &init) {
    assert(init.size() < MAX_DIM);
    dimension = init.size() > MAX_DIM ? MAX_DIM : init.size();
    auto item = init.begin();
    for (decltype(dimension) it = 0; it < dimension; ++it) {
      dimValues[it] = *item;
      ++item;
    }
  }

  size_type size(size_type axis) const noexcept {
    assert(axis < dimension);
    size_type ret{dimValues[axis]};
    for (decltype(dimension) it = axis + 1; it < dimension; ++it) {
      ret *= dimValues[it];
    }
    return ret;
  }

  size_type size() const noexcept { return size(0); }

  size_type dim() const noexcept { return dimension; }

  size_type dim_value(size_type axis) const {
    assert(axis < dimension);
    return dimValues[axis];
  }

  void add_dim(const size_type value) {
    assert(dimension < MAX_DIM);
    dimValues[dimension] = value;
    dimension += 1;
  }

  Shape sub(const size_type axis_start) const { return sub(axis_start, dimension); }

  Shape sub(const size_type axis_start, const size_type axis_end) const {
    assert(axis_start <= axis_end);
    assert(axis_end <= dimension);
    Shape ret;
    ret.dimension = axis_end - axis_start;
    for (size_type it = axis_start, ret_it = 0; it < axis_end; ++it, ++ret_it) {
      ret.dimValues[ret_it] = dimValues[it];
    }
    return ret;
  }

  std::string to_string() const {
    std::string str{"["};
    size_type   axis = 0;
    for (; axis < dimension - 1; ++axis) {
      str += std::to_string(dimValues[axis]) + ", ";
    }
    str += std::to_string(dimValues[axis]);
    str += "]";
    return std::move(str);
  }

  bool operator==(const Shape &other) const noexcept {
    if (other.dimension not_eq dimension) {
      return false;
    }
    for (decltype(dimension) it = 0; it < dimension; ++it) {
      if (dimValues[it] not_eq other.dimValues[it]) {
        return false;
      }
    }
    return true;
  }

private:
  static constexpr size_type     MAX_DIM = 8;
  size_type                      dimension{};
  std::array<size_type, MAX_DIM> dimValues;
};

class Tensor;

class Operator {
public:
  using Data = std::map<std::string_view, myt::Tensor &>;
  using Para = std::map<std::string_view, std::variant<f32, sint32>>;

  explicit Operator(const std::string &name) { name_ = name; }

  virtual ~Operator() = default;

  virtual bool init() { return true; }

  virtual bool run(Data &data, Para &para) { return true; }

  const std::string &get_name() { return name_; }

protected:
  std::string name_;
};

class CPUDevice {
public:
  using Index = uint8;

  enum ID : uint8 {
    CPU = 0,
    CUDA,
  };

  CPUDevice()          = default;
  virtual ~CPUDevice() = default;

  CPUDevice(const CPUDevice::ID &id, const CPUDevice::Index &index) : id_(id), index_(index) {};

  virtual void *malloc(sizeT size) = 0;

  virtual void free(void *ptr) = 0;

  virtual void memcpy(void *dst, void *src, sizeT size) = 0;

  virtual bool copy_mem_to(const CPUDevice &device) { return true; }

  virtual bool copy_mem_from(const CPUDevice &device) { return true; }

  bool has_op(std::string_view op_name) const { return ops.contains(op_name); }

  bool run(std::string_view op_name, Operator::Data &data) {
    Operator::Para para{};
    if (has_op(op_name)) {
      auto &op = ops.at(op_name);
      return op->run(data, para);
    }
    return false;
  }

  virtual bool run(std::string_view op_name, Operator::Data &data, Operator::Para &para) {
    if (has_op(op_name)) {
      auto &op = ops.at(op_name);
      return op->run(data, para);
    }
    return false;
  }

  CPUDevice::ID get_id() const noexcept { return id_; }

  CPUDevice::Index get_index() const noexcept { return index_; }

  std::string get_string() const noexcept { return std::string{device_info[id_].name_} + std::to_string(index_); }

  bool operator==(const CPUDevice &other) const noexcept { return id_ == other.id_ and index_ == other.index_; }

protected:
  CPUDevice::ID    id_{};
  CPUDevice::Index index_{};

  std::map<std::string_view, std::unique_ptr<Operator>> ops{};

  struct DeviceInfo {
    std::string_view name_;
  };
  constexpr static DeviceInfo device_info[]{
      {"cpu"},
      {"cuda"},
  };
};

class Storage {
public:
  explicit Storage(CPUDevice &device, const sizeT size) : device_{device}, size_{size} { data_ = device.malloc(size); }
  //explicit Storage(Storage &other, Device &target) : Storage(target, other.size_()) {}

  ~Storage() { device_.free(data_); }

  Storage(const Storage &)            = delete;
  Storage(Storage &&)                 = delete;
  Storage &operator=(const Storage &) = delete;
  Storage &operator=(Storage &&)      = delete;

  sizeT size() const noexcept { return size_; }

  template <typename T = void>
  T *data() noexcept {
    return static_cast<T *>(data_);
  }

  CPUDevice &get_device() const noexcept { return device_; }

private:
  CPUDevice &device_;
  void   *data_{};
  sizeT   size_{};
};

class Tensor {
public:
  template <typename T>
  class AccessorBase {
  public:
    AccessorBase(Tensor &tensor, void *ptr) : tensor_(tensor), ptr_(static_cast<decltype(ptr_)>(ptr)) {}

    T *ptr() const noexcept { return static_cast<T *>(ptr_); }

  protected:
    Tensor &tensor_;
    byteT  *ptr_{};
  };

  template <typename T, sizeT Dim, sizeT SubDim = Dim - 1>
    requires std::integral<T> or std::floating_point<T>
  class Accessor : public AccessorBase<T> {
  public:
    using AccessorBase<T>::AccessorBase;

    Accessor<T, SubDim> operator[](sizeT index) {
      return Accessor<T, SubDim>(this->tensor_,
                                 this->ptr_ + index * this->tensor_.mem_size(this->tensor_.shape().dim() - SubDim - 1));
    }
  };

  template <typename T>
    requires std::integral<T> or std::floating_point<T>
  class Accessor<T, 1> : public AccessorBase<T> {
  public:
    using AccessorBase<T>::AccessorBase;

    T &operator[](sizeT index) { return *reinterpret_cast<T *>(this->ptr_ + index * this->tensor_.type().size()); }
  };

public:
  Tensor()                          = default;
  ~Tensor()                         = default;
  Tensor(const Tensor &)            = default;
  Tensor(Tensor &&)                 = default;
  Tensor &operator=(const Tensor &) = default;
  Tensor &operator=(Tensor &&)      = default;

  const Type  &type() const { return type_; }
  const Shape &shape() const { return shape_; }
  CPUDevice      &device() const { return storage_->get_device(); }

  void create(CPUDevice &device, const Shape &shape, const Type &type = Type::FLOAT32) {
    type_    = type;
    shape_   = shape;
    storage_ = std::make_shared<Storage>(device, shape_.size() * type.size());
  }

  void to(CPUDevice &device) {
    if (device == storage_->get_device()) {
      return;
    }
    auto storage = std::make_shared<Storage>(device, shape_.size());
  }

  Tensor operator[](sizeT index) const {
    assert(index < shape_.dim_value(1));
    Tensor tensor;
    tensor.storage_ = storage_;
    tensor.offset   = index * mem_size(1);
    tensor.type_    = type_;
    tensor.shape_   = shape_.sub(1);
    return tensor;
  }

  template <typename T = void>
  T *get_ptr() {
    return reinterpret_cast<T *>(storage_->data<byteT>() + offset);
  }

  sizeT mem_size(sizeT axis) const noexcept {
    if (axis >= shape_.size()) {
      return type_.size();
    }
    return type_.size() * shape_.size(axis);
  }

  template <typename T, sizeT Dim>
  Accessor<T, Dim> make_accessor() {
    return Accessor<T, Dim>(*this, get_ptr<>());
  }

  using traverseFun = bool (*)(void *ptr);

  void traverse(traverseFun fun) { traverse(get_ptr<byteT>(), 0, fun); }

  std::string to_string() const noexcept {
    std::string str;

    return std::move(str);
  }

private:
  void traverse(byteT *ptr, sizeT axis, traverseFun fun) {
    const auto &tensor = *this;
    if (axis + 1 == tensor.shape().dim()) {
      for (auto i = 0; i < tensor.shape().dim_value(axis); i++) {
        fun(ptr + i * tensor.type().size());
      }
      return;
    }
    for (auto i = 0; i < tensor.shape().dim_value(axis); i++) {
      traverse(ptr + i * tensor.mem_size(axis + 1), axis + 1, fun);
    }
  }

  std::shared_ptr<Storage> storage_;
  std::size_t              offset{0};
  myt::Type                type_{};
  myt::Shape               shape_{};
};

} // namespace myt
