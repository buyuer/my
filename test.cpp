#include <format>
#include <print>

import my.base;

int main(int argc, char* argv[])
{
    myt::base::Shape shape { 1, 2, 3, 4 };
    myt::base::Type type { myt::base::Type::SINT8 };

    std::println("shape: {}", shape.size());
    std::println("{}: {}", type.name(), type.size());
}