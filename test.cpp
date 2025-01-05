#include <format>
#include <print>

import myt.base;
import myt.devices;

int main(int argc, char *argv[]) {
  myt::devices::CPUDevice cpu;
  myt::Shape        shape{2, 3, 4};
  myt::Type         type{myt::Type::SINT8};
  myt::Tensor       tensor1;
  tensor1.create(cpu, shape, type);
  auto accessor1     = tensor1.make_accessor<float, 3>();
  accessor1[1][2][1] = 111;

  auto tensor2   = tensor1;
  auto accessor2 = tensor2.make_accessor<float, 3>();
  std::println("{}", accessor2[1][2][1]);

  tensor2.create(cpu, shape, type);

  std::println("shape: {}", shape.size());
  std::println("{}: {}", type.name(), type.size());
}