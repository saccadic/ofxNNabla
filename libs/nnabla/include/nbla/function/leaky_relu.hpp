// Copyright (c) 2017 Sony Corporation. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/** LeakyReLU
 */
#ifndef __NBLA_FUNCTION_LEAKYRELU_HPP__
#define __NBLA_FUNCTION_LEAKYRELU_HPP__

#include <nbla/cpu.hpp>
#include <nbla/function.hpp>
#include <nbla/function_registry.hpp>

#include <memory>
#include <string>

namespace nbla {

using std::string;

NBLA_REGISTER_FUNCTION_HEADER(LeakyReLU, float, bool);

/** Leaky Rectified Linear Unit (LeakyReLU) defined as
@f[
y_i= \left\{
           \begin{array}{ll}
                        x_i & if x_i > 0  \\
                alpha * x_i & otherwise
           \end{array} \right.,
@f]

Inputs:
- N-D array.

Outputs:
- N-D array.

@tparam T Data type for computation.
@param alpha Leakage parameter.
@param inplace The output array is will be shared with the input array if true.
\ingroup FunctionImplGrp
 */
template <typename T> class LeakyReLU : public BaseFunction<float, bool> {
protected:
  float alpha_;
  bool inplace_;

public:
  LeakyReLU(const Context &ctx, float alpha, bool inplace)
      : BaseFunction(ctx, alpha, inplace), alpha_(alpha), inplace_(inplace) {}
  virtual ~LeakyReLU() {}
  virtual shared_ptr<Function> copy() const {
    return create_LeakyReLU(ctx_, alpha_, inplace_);
  }
  virtual int min_inputs() { return 1; }
  virtual int min_outputs() { return 1; }
  virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
  virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
  virtual string name() { return "LeakyReLU"; }
  virtual vector<string> allowed_array_classes() {
    return SingletonManager::get<Cpu>()->array_classes();
  }
  virtual bool grad_depends_output_data(int i, int o) const { return inplace_; }
  virtual int inplace_data(int i) const {
    return inplace_ ? Function::INPLACE : Function::NOT_INPLACE;
  }
  virtual int inplace_data_with(int i) const { return 0; }
  virtual int inplace_grad(int i) const {
    return inplace_ ? Function::INPLACE : Function::NOT_INPLACE;
  }
  virtual int inplace_grad_with(int i) const { return 0; }

protected:
  NBLA_API virtual void setup_impl(const Variables &inputs,
                                   const Variables &outputs);
  NBLA_API virtual void forward_impl(const Variables &inputs,
                                     const Variables &outputs);
  NBLA_API virtual void backward_impl(const Variables &inputs,
                                      const Variables &outputs,
                                      const vector<bool> &propagate_down,
                                      const vector<bool> &accum);
};
}
#endif
