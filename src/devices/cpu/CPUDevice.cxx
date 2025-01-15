module;
#include <cassert>
#include <map>
#include <memory>
#include <ostream>
#include <print>
#include <string_view>
#include <variant>
module myt.devices.cpu;

import myt.base;
import myt.common;

namespace myt::devices {

namespace cpu {

bool AddOp::run(Data &data, Operator::Para &para) {
  auto &input  = data.find("input")->second;
  auto &output = data.find("output")->second;
  output.create(input.device(), input.shape(), input.type());

  auto v     = std::get<f32>(para["v"]);
  auto count = input.shape().size();
  for (sizeT i = 0; i < count; i++) {
    auto offset = i;
    if (input.type().id() == Type::SINT32) {
      auto in  = input.get_ptr<sint32>() + offset;
      auto out = output.get_ptr<sint32>() + offset;
      *out     = *in + v;
    }
  }
  return true;
}

} // namespace cpu

} // namespace myt::devices