#include <format>
#include <print>

import myt.base;
import myt.devices;

int main(int argc, char *argv[]) {
  myt::devices::CPUDevice cpu;
  myt::Shape              shape{2, 3, 4};
  myt::Type               type{myt::Type::SINT32};
  myt::Tensor             tensor1;
  tensor1.create(cpu, shape, type);
  auto accessor1     = tensor1.make_accessor<float, 3>();
  accessor1[1][2][1] = 111;

  for (myt::uint8 i = 1; i < 25; i++) {
    *(tensor1.get_ptr<myt::sint32>() + i - 1) = i;
  }

  std::print("[");
  tensor1.traverse([](void *ptr) -> bool {
    std::print("{},", *static_cast<int *>(ptr));
    return true;
  });
  std::println("]");

  auto tensor2   = tensor1;
  auto accessor2 = tensor2.make_accessor<float, 3>();
  std::println("{}", accessor2[1][2][1]);

  tensor2.create(cpu, shape, type);

  auto tensor3 = tensor1[1][2];
  std::print("[");
  tensor3.traverse([](void *ptr) -> bool {
    std::print("{},", *static_cast<int *>(ptr));
    return true;
  });
  std::println("]");

  myt::Operator::Data data{
      {"input", tensor1},
      {"output", tensor2},
  };
  cpu.run("Add", data);

  std::println("shape: {}", shape.size());
  std::println("{}: {}", type.name(), type.size());
}