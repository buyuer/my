/**
 * @author  dingddding@163.com
 * @date    2024.11.02
 * @brief   Implementation of CPUDevice
 */

module;
export module myt.devices;

import myt.base;

export namespace myt::devices {
class CPUDevice : public myt::Device {
public:
  CPUDevice() : Device(CPU, 0) {}
  using myt::Device::Device;

  void *malloc(myt::sizeT size) override { return operator new(size); }
  void  free(void *ptr) override { operator delete(ptr); }
};
} // namespace myt::devices
