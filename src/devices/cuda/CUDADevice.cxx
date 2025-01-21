/**
 * @author  dingddding@163.com
 * @date    2024.11.02
 * @brief   Implementation of CPUDevice
 */

module;

#include <cassert>
#include <map>
#include <memory>
#include <ostream>
#include <print>
#include <string_view>
#include <variant>

#include <cuda_runtime.h>

module myt.devices.cuda;

import myt.base;
import myt.common;

namespace myt::devices {

void *CUDADevice::malloc(myt::sizeT size) {
  void       *ret;
  cudaError_t err = cudaMalloc(&ret, size);
  if (err != cudaSuccess) {
    return nullptr;
  }
  return ret;
}

void CUDADevice::free(void *ptr) { cudaError_t err = cudaFree(ptr); }

void CUDADevice::memcpy(void *dst, void *src, sizeT size) {
  cudaError_t err = cudaMemcpy(dst, src, size, cudaMemcpyDeviceToDevice);
}

} // namespace myt::devices
