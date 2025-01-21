/**
 * @author  dingddding@163.com
 * @date    2024.11.02
 * @brief   Implementation of CUDADevice
 */

module;
#include <cassert>
#include <map>
#include <memory>
#include <ostream>
#include <print>
#include <string_view>
#include <variant>
export module myt.devices.cuda;

import myt.base;
import myt.common;

namespace myt::devices {

export class CUDADevice : public myt::Device {
public:
  CUDADevice() : Device(CPU, 0) {}
  using myt::Device::Device;

  void *malloc(myt::sizeT size) override;
  void  free(void *ptr) override;
  void  memcpy(void *dst, void *src, sizeT size) override;
};

} // namespace myt::devices