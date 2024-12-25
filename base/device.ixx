module;
#include <map>
#include <string_view>
export module myt.base:Device;

import :Type;
import :Operator;

export namespace myt::base {

class Device {
public:
    using Index = uint8;

    enum ID : uint8 { CPU = 0, CUDA };

    Device() = default;
    virtual ~Device() = default;

    virtual bool copy_mem_to(const Device& device) { return true; }

    virtual bool copy_mem_from(const Device& device) { return true; }

protected:
    Device::ID id_ {};
    Device::Index index_ {};

    std::map<std::string_view, Operator> ops {};
};

} // namespace my::base