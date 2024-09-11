module;
#include <concepts>
#include <memory>
export module my.base:Tensor;

import :Type;
import :Shape;
import :Device;
import :Storage;

export namespace myt::base {

class Tensor {

public:
    template <typename T> class AccessorBase {
    public:
        AccessorBase(Tensor& tensor, void* ptr)
            : tensor_(tensor)
            , ptr_(static_cast<decltype(ptr_)>(ptr))
        {
        }

        T* ptr() const noexcept { return static_cast<T*>(ptr_); }

    protected:
        Tensor& tensor_;
        uint8* ptr_ {};
    };

    template <typename T, size_type Dim, size_type SubDim = Dim - 1>
        requires std::integral<T> or std::floating_point<T>
    class Accessor : public AccessorBase<T> {
    public:
        using AccessorBase<T>::AccessorBase;

        Accessor<T, SubDim> operator[](size_type index)
        {
            return Accessor<T, SubDim>(this->tensor_,
                this->ptr_
                    + index
                        * this->tensor_.mem_size(
                            this->tensor_.shape().dim() - SubDim));
        }
    };

    template <typename T>
        requires std::integral<T> or std::floating_point<T>
    class Accessor<T, 1> : public AccessorBase<T> {
    public:
        using AccessorBase<T>::AccessorBase;

        T& operator[](size_type index)
        {
            return *reinterpret_cast<T*>(
                this->ptr_ + index * this->tensor_.type().size());
        }
    };

    const Type& type() const { return type_; }
    const Shape& shape() const { return shape_; }
    const Device& device() const { return device_; }

    Tensor() = default;
    ~Tensor() = default;
    Tensor(const Tensor&) = default;
    Tensor(Tensor&&) = default;
    Tensor& operator=(const Tensor&) = default;
    Tensor& operator=(Tensor&&) = default;

    void create(const Shape& shape, const Type& type = Type::FLOAT32,
        const Device& device = {})
    {
        type_ = type;
        shape_ = shape;
        device_ = device;
        storage_ = std::make_shared<Storage>(shape_.size() * type.size());
    }

    size_type mem_size(size_type axis) const noexcept
    {
        if (axis >= shape_.size()) {
            return type_.size();
        }
        return type_.size() * shape_.size(axis);
    }

    template <typename T, size_type Dim> Accessor<T, Dim> make_accessor()
    {
        return Accessor<T, Dim>(*this, storage_->data());
    }

private:
    std::shared_ptr<myt::base::Storage> storage_;
    myt::base::Type type_ {};
    myt::base::Shape shape_ {};
    myt::base::Device device_ {};
};

} // namespace myt::base