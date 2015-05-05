// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef NUCLEUS_OBSERVER_LIST_H_
#define NUCLEUS_OBSERVER_LIST_H_

#include "nucleus/logging.h"
#include "nucleus/utils/detail/observer_list_base.h"

namespace nu {

template <typename ObserverType, bool CheckEmpty = false>
class ObserverList : public detail::ObserverListBase<ObserverType> {
public:
  ObserverList() {}

  ~ObserverList() {
    // When CheckEmpty is true, assert that the list is empty on destruction.
    if (CheckEmpty) {
      ObserverListBase<ObserverType>::compact();
      DCHECK(ObserverListBase<ObserverType>::isEmpty());
    }
  }

  bool isEmpty() const { return ObserverListBase<ObserverType>::isEmpty(); }
};

}  // namespace nu

#define FOR_EACH_OBSERVER(ObserverType, ObserverList, Func)                    \
  do {                                                                         \
    if (!(ObserverList).isEmpty()) {                                           \
      ::nu::detail::ObserverListBase<ObserverType>::Iterator                   \
          itInsideObserverMacro(&ObserverList);                                \
      ObserverType* oobbss;                                                    \
      while ((oobbss = itInsideObserverMacro.getNext()) != nullptr) {          \
        oobbss->Func;                                                          \
      }                                                                        \
    }                                                                          \
  } while (0)

#endif  // NUCLEUS_OBSERVER_LIST_H_
