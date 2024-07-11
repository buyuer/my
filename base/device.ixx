module;
export module my.base:Device;

export namespace myt::base {

class Device {
public:
    enum ID {
        CPU = 0,
    };

    Device() { }

private:
    ID id;
};

} // namespace my::base