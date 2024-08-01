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

private:
    std::shared_ptr<Storage> storage_;

    myt::base::Type type_ {};
    myt::base::Shape shape_ {};
    myt::base::Device device_ {};
};

template <typename T, size_type N> class TensorAccessorBase {
public:
private:
};

template <typename T, size_type N>
    requires std::integral<T> or std::floating_point<T>
class TensorAccessor {
public:
    TensorAccessor<T, N - 1> operator[](size_type index)
    {
        return TensorAccessor<T, N - 1>();
    }

private:
    void* data {};
};

} // namespace myt::base