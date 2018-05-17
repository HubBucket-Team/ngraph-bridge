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
#ifndef NGRAPH_LIBRARY_MANAGER_H_
#define NGRAPH_LIBRARY_MANAGER_H_

#include <mutex>
#include <vector>

#include "tensorflow/core/framework/function.pb.h"

namespace tf = tensorflow;

namespace ngraph_bridge {
class NGraphLibraryManager {
 public:
  static int MakeNewLibrary();
  static tf::FunctionDefLibrary* GetLibrary(int idx);

 private:
  static std::vector<tf::FunctionDefLibrary*> s_libraries;
  static std::mutex s_libraries_mutex;
};
}  // namespace ngraph_bridge

#endif
