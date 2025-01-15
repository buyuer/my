/**
 * @author  dingddding@163.com
 * @date    2024.11.02
 * @brief   Implementation of CPUDevice
 */

module;
#include <cstdlib>
#include <map>
#include <memory>
#include <print>
#include <string_view>
#include <variant>
export module myt.devices.cpu;

import myt.base;
import myt.common;

namespace myt::devices {

namespace cpu {

class AddOp : public myt::Operator {
public:
  AddOp() : myt::Operator("Add") {}
  bool run(Data &data, Operator::Para &para) override;
};

class MulOp : public myt::Operator {
public:
  MulOp() : myt::Operator("Mul") {}
  bool run(Data &, Operator::Para &para) override { return true; }
};

} // namespace cpu

export class CPUDevice : public myt::CPUDevice {
public:
  CPUDevice() : CPUDevice(CPU, 0) {
    ops["Add"] = std::make_unique<cpu::AddOp>();
    ops["Mul"] = std::make_unique<cpu::MulOp>();
  }
  using myt::CPUDevice::CPUDevice;

  void *malloc(myt::sizeT size) override { return operator new(size); }
  void  free(void *ptr) override { operator delete(ptr); }
  void  memcpy(void *dst, void *src, sizeT size) override { std::memcpy(dst, src, size); }
};

} // namespace myt::devices
