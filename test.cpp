// #include <format>
// #include <print>
// #include <concepts>
// #include <memory>

#include <iostream>

import my.base;

int main(int argc, char* argv[])
{
    myt::base::Shape shape { 2, 3, 4 };
    myt::base::Type type { myt::base::Type::SINT8 };
    myt::base::Tensor tensor;
    tensor.create(shape);
    auto accessor = tensor.make_accessor<float, 3>();
    std::cout << accessor[1][1][3] << std::endl;
    std::cout << shape.size() << std::endl;

    // std::println("shape: {}", shape.size());
    // std::println("{}: {}", type.name(), type.size());
}