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

/** Array creator and synchronizer
 */
#ifndef __NBLA_ARRAY_REGISTRY_HPP__
#define __NBLA_ARRAY_REGISTRY_HPP__
#include <nbla/array.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace nbla {

using std::shared_ptr;
using std::string;
using std::map;
using std::pair;

/** ArrayCreator class this is never be instantiated. */
class NBLA_API ArrayCreator {
public:
  typedef std::function<Array *(const Size_t, dtypes, const Context &)> Creator;
  typedef std::function<Context(const Context &ctx)> FilterContext;
  typedef map<string, pair<Creator, FilterContext>> Registry_t;

  /** Interface to create array */
  static Array *create(const Size_t size, dtypes dtype, const Context &ctx);

  /** Filter an context into minimal info that describes the context.
  */
  static Context filter_context(const Context &ctx);

  /** Register new creator */
  static void add_creator(const string &name, Creator creator,
                          FilterContext filter_context);

private:
  // Never be created
  inline ArrayCreator() {}

  /** Get registry of array creator function.
  TODO: Validate whether this should be private.
  */
  static Registry_t &get_registry();
};

/** ArraySynchronizer
*/
class NBLA_API ArraySynchronizer {
public:
  typedef std::function<void(Array *, Array *)> Synchronizer;
  typedef map<pair<string, string>, Synchronizer> Registry_t;

  /** Synchronize array
  */
  static void synchronize(const string &src_class, Array *src_array,
                          const string &dst_class, Array *dst_array);

  /** Register new synchronizer
  */
  static void add_synchronizer(const string &src_class, const string &dst_class,
                               Synchronizer synchronizer);

private:
  inline ArraySynchronizer() {}

  /** Get registry of array creator function.
  TODO: Validate whether this should be private.
  */
  static Registry_t &get_registry();
};

/** Synchronizer rely on Array::copy_from.

    This should be used as a synchronizer between classes that are using the
   same device class like CpuArray-CpuCachedArray.
 */
NBLA_API void synchronizer_default(Array *src, Array *dst);

#define NBLA_REGISTER_ARRAY_CREATOR(CLS)                                       \
  {                                                                            \
    std::function<Array *(const Size_t, dtypes, const Context &)> func =       \
        [](const Size_t size, dtypes dtype, const Context &ctx) {              \
          return new CLS(size, dtype, ctx);                                    \
        };                                                                     \
    ArrayCreator::add_creator(#CLS, func, CLS::filter_context);                \
  }

#define NBLA_REGISTER_ARRAY_SYNCHRONIZER(SRC_CLASS, DST_CLASS, SYNCHRONIZER)   \
  { ArraySynchronizer::add_synchronizer(#SRC_CLASS, #DST_CLASS, SYNCHRONIZER); }
}
#endif
