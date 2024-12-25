module;
#include <string>
export module myt.base:Operator;

export namespace myt::base {
class Operator {
public:
    explicit Operator(const std::string& name) { name_ = name; }

    virtual ~Operator() = default;

    virtual bool init() { return true; }

    virtual bool run() { return true; }

protected:
    std::string name_;
};
}