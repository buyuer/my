module;
#include <concepts>
#include <initializer_list>
export module my.base:Shape;

import :Type;

export namespace myt::base {

class Shape {
public:
    using size_type = myt::base::size_type;

    Shape() = default;
    ~Shape() = default;
    Shape(const Shape&) = default;
    Shape(Shape&&) = default;
    Shape& operator=(const Shape&) = default;
    Shape& operator=(Shape&&) = default;

    template <typename T>
        requires std::integral<T>
    Shape(const std::initializer_list<T>& init)
    {
        if (init.size() > MAX_DIM) { }
        dimension = init.size() > MAX_DIM ? MAX_DIM : init.size();
        auto item = init.begin();
        for (decltype(dimension) it = 0; it < dimension; ++it) {
            dimValues[it] = *item;
            ++item;
        }
    }

    size_type size(size_type axis) const noexcept
    {
        if (axis >= dimension) {
            return 0;
        }
        size_type ret { dimValues[axis] };
        for (decltype(dimension) it = axis + 1; it < dimension; ++it) {
            ret *= dimValues[it];
        }
        return ret;
    }

    size_type size() const noexcept { return size(0); }

    bool operator==(const Shape& other) const noexcept
    {
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
    static constexpr size_type MAX_DIM = 5;
    size_type dimension {};
    size_type dimValues[MAX_DIM] {};
};

} // namespace my::base
