module;
export module my.base:tensor;

import :Type;
import :Shape;
import :Device;

export namespace myt::base {

class Tensor {
private:
    myt::base::Type type_ {};
    myt::base::Shape shape_ {};
    myt::base::Device device_ {};
};

} // namespace myt::base