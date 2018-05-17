/*******************************************************************************
 * Copyright 2017-2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"

namespace tf = tensorflow;

namespace ngraph_bridge {

REGISTER_OP("NGraphEncapsulate")
    .Input("args: Targuments")
    .Attr("Targuments: list(type) >= 0")
    .Output("results: Tresults")
    .Attr("Tresults: list(type) >= 0")
    //.Attr("function: func")
    .Attr("library_index: int")
    .Attr("ngraph_cluster: int")
    .SetIsStateful()
    .Doc("nGraph Encapsulation Op. For use by the nGraph JIT only.");

class NGraphEncapsulateOp : public tf::OpKernel {
 public:
  explicit NGraphEncapsulateOp(tf::OpKernelConstruction* ctx)
      : tf::OpKernel(ctx) {
    // Implement
  }
  ~NGraphEncapsulateOp() override {
    // d-tor
  }
  void Compute(tf::OpKernelContext* ctx) override {}
};

}  // namespace ngraph_bridge

namespace tensorflow {
REGISTER_KERNEL_BUILDER(Name("NGraphEncapsulate").Device("NGRAPH_CPU"),
                        ngraph_bridge::NGraphEncapsulateOp);
}
