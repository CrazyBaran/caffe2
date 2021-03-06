/**
 * Copyright (c) 2016-present, Facebook, Inc.
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
 */

#include "adam_op.h"

namespace caffe2 {

REGISTER_CPU_OPERATOR(Adam, AdamOp<float, CPUContext>);
OPERATOR_SCHEMA(Adam)
    .NumInputs(6)
    .NumOutputs(3)
    .AllowInplace({{0, 0}, {1, 1}, {2, 2}})
    .SetDoc(R"DOC(

Computes the Adam update (https://arxiv.org/abs/1412.6980) for an
input gradient and momentum parameters. Concretely, given inputs
(param, m1, m2, grad, lr, iters),

    t = iters + 1
    corrected_local_rate = lr * sqrt(1 - power(beta2, t)) /
      (1 - power(beta1, t))
    m1_o = (beta1 * m1) + (1 - beta1) * grad
    m2_o = (beta2 * m2) + (1 - beta2) * np.square(grad)
    grad_o = corrected_local_rate * m1_o / \
        (sqrt(m2_o) + epsilon)
    param_o = param + grad_o

and returns (param_o, m1_o, m2_o)

)DOC")
    .Input(0, "param", "Parameters to be updated")
    .Input(1, "moment_1", "First moment history")
    .Input(2, "moment_2", "Second moment history")
    .Input(3, "grad", "Gradient computed")
    .Input(4, "lr", "learning rate")
    .Input(5, "iter", "iteration number")
    .Output(0, "output_param", "Updated parameters")
    .Output(1, "output_moment_1", "Updated first moment")
    .Output(2, "output_moment_2", "Updated second moment")
    .Arg("beta1", "Default 0.9")
    .Arg("beta2", "Default 0.999")
    .Arg("epsilon", "Default 1e-5");

REGISTER_CPU_OPERATOR(SparseAdam, SparseAdamOp<float, CPUContext>);
OPERATOR_SCHEMA(SparseAdam)
    .NumInputs(7)
    .NumOutputs(3)
    .EnforceInplace({{0, 0}, {1, 1}, {2, 2}})
    .SetDoc(R"DOC(

Computes the Adam Update for the sparse case.
Given inputs (param, moment1, moment2, indices, grad, lr, iter), runs the dense
Adam on (param, moment1[indices], momemnt2[indices], lr, iter) and returns
(new_param, new_moment1, new_moment2) as in dense case

)DOC")
    .Input(0, "param", "Parameters to be updated")
    .Input(1, "moment_1", "First moment history")
    .Input(2, "moment_2", "Second moment history")
    .Input(3, "indices", "Sparse indices")
    .Input(4, "grad", "Gradient computed")
    .Input(5, "lr", "learning rate")
    .Input(6, "iter", "iteration number")
    .Output(0, "output_param", "Updated parameters")
    .Output(1, "output_moment_1", "Updated first moment")
    .Output(2, "output_moment_2", "Updated second moment")
    .Arg("beta1", "Default 0.9")
    .Arg("beta2", "Default 0.999")
    .Arg("epsilon", "Default 1e-5");

SHOULD_NOT_DO_GRADIENT(Adam);
SHOULD_NOT_DO_GRADIENT(SparseAdam);

}
