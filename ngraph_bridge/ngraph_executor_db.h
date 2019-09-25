/*******************************************************************************
 * Copyright 2019 Intel Corporation
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

#ifndef NGRAPH_EXECUTOR_DB_H_
#define NGRAPH_EXECUTOR_DB_H_
#pragma once

#include <mutex>
#include <ostream>
#include <vector>
#include "tensorflow/core/framework/tensor_shape.h"
#include "tensorflow/core/graph/graph.h"

#include "ngraph/ngraph.hpp"

#include "logging/ngraph_log.h"
#include "ngraph_bridge/ngraph_freshness_tracker.h"
#include "ngraph_bridge/ngraph_pipelined_tensors.h"

namespace tensorflow {

namespace ngraph_bridge {

class NGraphExecutorDB {
 public:
  bool IsNgExecAvail(std::string signature,
                     std::shared_ptr<ngraph::runtime::Executable>&
                         ng_exec)  // line no. 216, 220
  {
    lock_guard<mutex> lock(m_mutex1);
    auto it = m_ng_exec_map.find(signature);
    if (it == m_ng_exec_map.end()) {
      return false;
    }
    ng_exec = it->second;
    return true;
  }

  bool IsNgFunctionAvail(
      std::shared_ptr<ngraph::runtime::Executable> ng_exec,
      std::shared_ptr<ngraph::Function>& ng_function)  // line no. 363, 364
  {
    lock_guard<mutex> lock(m_mutex1);
    auto it = m_ng_function_map.find(ng_exec);
    if (it == m_ng_function_map.end()) return false;
    ng_function = it->second;
    return true;
  }

  bool IsPipelinedTensorsStoreAvailable(
      std::shared_ptr<ngraph::runtime::Executable>
          ng_exec)  // line no. 216, 220
  {
    lock_guard<mutex> lock(m_mutex2);
    auto it =
        m_executable_pipelined_tensors_map.find(ng_exec);  // line no. 443-444
    if (it == m_executable_pipelined_tensors_map.end()) return false;
    return true;
  }

  shared_ptr<PipelinedTensorsStore> GetPipelineTensorStore(
      std::shared_ptr<ngraph::runtime::Executable> ng_exec) {
    lock_guard<mutex> lock(m_mutex2);
    return m_executable_pipelined_tensors_map.at(ng_exec);  // 486
  }

  void RemoveExecAndFunc(std::shared_ptr<ngraph::runtime::Executable>&
                             evicted_ng_exec)  // line no. 261, 262, 263
  {
    lock_guard<mutex> lock(m_mutex1);
    evicted_ng_exec = m_ng_exec_map[m_lru.back()];
    m_ng_exec_map.erase(m_lru.back());
    m_ng_function_map.erase(evicted_ng_exec);
  }

  void InsertNgExecMap(std::string signature,
                       std::shared_ptr<ngraph::runtime::Executable> ng_exec) {
    lock_guard<mutex> lock(m_mutex1);
    m_ng_exec_map.emplace(signature, ng_exec);  // line no. 324
  }

  void InsertNgFunctionMap(std::shared_ptr<ngraph::runtime::Executable> ng_exec,
                           std::shared_ptr<ngraph::Function> ng_function) {
    lock_guard<mutex> lock(m_mutex1);
    m_ng_function_map.emplace(ng_exec, ng_function);  // line no. 327
  }

  void InsertExecPipelineTesornMap(
      std::shared_ptr<ngraph::runtime::Executable> ng_exec,
      shared_ptr<PipelinedTensorsStore> pts) {
    lock_guard<mutex> lock(m_mutex2);
    m_executable_pipelined_tensors_map.emplace(ng_exec, pts);  // line no. 467
  }

  size_t SizeOfNgExecMap() {
    lock_guard<mutex> lock(m_mutex1);
    return m_ng_exec_map.size();
  }

  std::string LRUBack() {
    lock_guard<mutex> lock(m_mutex3);
    return m_lru.back();
  }

  std::string LRUFront() {
    lock_guard<mutex> lock(m_mutex3);
    return m_lru.front();
  }

  void RemoveFromLRU(std::string signature) {
    lock_guard<mutex> lock(m_mutex3);
    m_lru.remove(signature);
  }

  void PushFrontInLRU(std::string signature) {
    lock_guard<mutex> lock(m_mutex3);
    m_lru.push_front(signature);
  }

  void PopBackLRU() {
    lock_guard<mutex> lock(m_mutex3);
    m_lru.pop_back();
  }

 private:
  mutex m_mutex1;
  mutex m_mutex2;
  mutex m_mutex3;
  std::list<std::string> m_lru;
  std::unordered_map<std::string, std::shared_ptr<ngraph::runtime::Executable>>
      m_ng_exec_map;
  std::unordered_map<std::shared_ptr<ngraph::runtime::Executable>,
                     std::shared_ptr<ngraph::Function>>
      m_ng_function_map;
  std::unordered_map<std::shared_ptr<ngraph::runtime::Executable>,
                     shared_ptr<PipelinedTensorsStore>>
      m_executable_pipelined_tensors_map;
};

}  // namespace ngraph_bridge

}  // namespace tensorflow
#endif  // NGRAPH_EXECUTOR_H_
