module;
export module my.base:Device;

import :Type;

export namespace myt::base {

class Device {
public:
    using Index = uint8;
    enum ID : uint8 {
        CPU = 0,
    };

    Device() = default;

private:
    Device::ID id_ {};
    Device::Index index_ {};
};

} // namespace my::base