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

/** Common functions and variables

Some items are not intentionally located in this header,
maybe because they are uncategorized.
*/
#ifndef __NBLA_COMMON_HPP__
#define __NBLA_COMMON_HPP__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <nbla/context.hpp>
#include <nbla/exception.hpp>
#include <nbla/init.hpp>

namespace nbla {

using std::vector;
using std::ostringstream;
using std::shared_ptr;

typedef vector<int64_t> Shape_t; ///< Type of array shape and strides etc.
typedef int64_t Size_t;          ///< Type of size of array

/** Compute array size by shape

@param[in] axis size under given axis will be computed
*/
inline Size_t compute_size_by_shape(const Shape_t &shape, Size_t axis = 0) {
  axis = std::max(static_cast<Size_t>(0), axis);
  NBLA_CHECK(axis <= shape.size(), error_code::value,
             "axis must be less than or equal to size of shape. "
             "axis: %ld > size of shape: %ld.",
             axis, shape.size());
  Size_t ret = 1;
  for (int i = axis; i < shape.size(); ++i) {
    ret *= shape[i];
  }
  return ret;
}

/** Helper for getting strides of C contiguous memory arrangement.
*/
inline Shape_t get_c_contiguous_strides(const Shape_t &shape) {
  if (!shape.size()) {
    return Shape_t(); // empty strides for 0-shaped array
  }
  Shape_t strides(shape.size(), 1);
  for (int i = strides.size() - 2; i >= 0; --i) {
    strides[i] *= strides[i + 1] * shape[i + 1];
  }
  return strides;
}

/** Join string with delimiter.
*/
template <typename T>
inline string string_join(const vector<T> &vec, const string &delim) {
  ostringstream oss;
  if (vec.empty()) {
    return "";
  }
  for (int i = 0; i < vec.size() - 1; ++i) {
    oss << vec[i] << delim;
  }
  oss << vec[vec.size() - 1];
  return oss.str();
}

/** Scoped callback
*/
class DestructorCallback {
  std::function<void(void)> callback_;

public:
  inline DestructorCallback(std::function<void(void)> callback)
      : callback_(callback) {}
  inline ~DestructorCallback() { callback_(); }
};

/** Hash combining function.

    @sa
   http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
 */
template <typename T> void hash_combine(size_t &seed, T const &v) {
  seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T>
vector<T *> as_pointer_array(const vector<shared_ptr<T>> &vec) {
  vector<T *> ret(vec.size());
  for (int i = 0; i < vec.size(); ++i) {
    ret[i] = vec[i].get();
  }
  return ret;
}

/** Getter for variadic template arguments.
 */
template <int index, typename... Args>
auto get_from_variadic_args(Args &&... args) -> typename std::remove_reference<
    decltype(std::get<index>(std::make_tuple(args...)))>::type {
  auto &&t = std::forward_as_tuple(args...);
  return std::get<index>(t);
}
/** Disable copy and assignment */
#define DISABLE_COPY_AND_ASSIGN(classname)                                     \
private:                                                                       \
  classname(const classname &);                                                \
  classname &operator=(const classname &)
}
#endif
