/**
 * @author  dingddding@163.com
 * @date    2024.11.02
 * @brief   bass struct
 */

module;
#include <cassert>
#include <complex>
#include <concepts>
#include <map>
#include <string>
#include <string_view>
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
  static constexpr size_type MAX_DIM = 8;
  size_type                  dimension{};
  size_type                  dimValues[MAX_DIM]{};
};

class Tensor;

class Operator {
public:
  using Data = std::map<std::string_view, myt::Tensor &>;

  explicit Operator(const std::string &name) { name_ = name; }

  virtual ~Operator() = default;

  virtual bool init() { return true; }

  virtual bool run(Data &data) { return true; }

  const std::string &get_name() { return name_; }

protected:
  std::string name_;
};

class Device {
public:
  using Index = uint8;

  enum ID : uint8 {
    CPU = 0,
    CUDA,
  };

  Device()          = default;
  virtual ~Device() = default;

  Device(const Device::ID &id, const Device::Index &index) : id_(id), index_(index) {};

  virtual void *malloc(sizeT size) = 0;

  virtual void free(void *ptr) = 0;

  virtual bool copy_mem_to(const Device &device) { return true; }

  virtual bool copy_mem_from(const Device &device) { return true; }

  Device::ID get_id() const noexcept { return id_; }

  Device::Index get_index() const noexcept { return index_; }

  std::string get_string() const noexcept { return std::string{device_info[id_].name_} + std::to_string(index_); }

  bool operator==(const Device &other) const noexcept { return id_ == other.id_ and index_ == other.index_; }

protected:
  Device::ID    id_{};
  Device::Index index_{};

  std::map<std::string_view, Operator> ops{};

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
  explicit Storage(Device &device, sizeT size) : device_{device}, size_{size} { data_ = device.malloc(size); }

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

  const Device &get_device() const noexcept { return device_; }

private:
  Device &device_;
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
    uint8  *ptr_{};
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

  const Type   &type() const { return type_; }
  const Shape  &shape() const { return shape_; }
  const Device &device() const { return storage_->get_device(); }

  void create(Device &device, const Shape &shape, const Type &type = Type::FLOAT32) {
    type_    = type;
    shape_   = shape;
    storage_ = std::make_shared<Storage>(device, shape_.size() * type.size());
  }

  sizeT mem_size(sizeT axis) const noexcept {
    assert(axis < shape_.size());
    return type_.size() * shape_.size(axis);
  }

  template <typename T, sizeT Dim>
  Accessor<T, Dim> make_accessor() {
    return Accessor<T, Dim>(*this, storage_->data());
  }

  std::string get_string() const noexcept {
    std::stringstream ss;

    return std::move(ss.str());
  }

private:
  std::shared_ptr<myt::Storage> storage_;
  myt::Type                     type_{};
  myt::Shape                    shape_{};
};

} // namespace myt
