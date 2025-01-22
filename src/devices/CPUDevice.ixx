/**
 * @author  dingddding@163.com
 * @date    2024.11.02
 * @brief   CPUDevice
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

export class CPUDevice : public myt::Device {
public:
  CPUDevice();
  using myt::Device::Device;

  void *malloc(myt::sizeT size) override { return operator new(size); }
  void  free(void *ptr) override { operator delete(ptr); }
  void  memcpy(void *dst, void *src, sizeT size) override { std::memcpy(dst, src, size); }
};

} // namespace myt::devices
