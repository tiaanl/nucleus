
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
      detail::ObserverListBase<ObserverType>::compact();
      DCHECK(detail::ObserverListBase<ObserverType>::isEmpty());
    }
  }

  bool isEmpty() const {
    return detail::ObserverListBase<ObserverType>::isEmpty();
  }
};

}  // namespace nu

#define FOR_EACH_OBSERVER(ObserverType, ObserverList, Func)                                                            \
  do {                                                                                                                 \
    if (!(ObserverList).isEmpty()) {                                                                                   \
      ::nu::detail::ObserverListBase<ObserverType>::Iterator itInsideObserverMacro(&ObserverList);                     \
      ObserverType* oobbss;                                                                                            \
      while ((oobbss = itInsideObserverMacro.getNext()) != nullptr) {                                                  \
        oobbss->Func;                                                                                                  \
      }                                                                                                                \
    }                                                                                                                  \
  } while (0)

#endif  // NUCLEUS_OBSERVER_LIST_H_
