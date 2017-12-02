
#ifndef NUCLEUS_UTILS_DETAIL_OBSERVER_LIST_H_
#define NUCLEUS_UTILS_DETAIL_OBSERVER_LIST_H_

#include <algorithm>
#include <cstdint>

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/macros.h"

namespace nu {

namespace detail {

template <typename ObserverType>
class ObserverListBase {
public:
  // An Iterator class that can be used to access the list of observer.
  class Iterator {
  public:
    explicit Iterator(ObserverListBase<ObserverType>* list) : m_list(list) {
      ++m_list->m_notifyDepth;
    }

    ~Iterator() {
      if (m_list && --m_list->m_notifyDepth == 0) {
        m_list->compact();
      }
    }

    ObserverType* getNext() {
      if (!m_list) {
        return nullptr;
      }

      ListType& observers = m_list->m_observers;

      // Advance if the current element is null.
      USize maxIndex = observers.getSize();
      while (m_index < maxIndex && !observers.get(m_index)) {
        ++m_index;
      }

      // Return the observer or nullptr if we are at the end of the list.
      return m_index < maxIndex ? observers.get(m_index++) : nullptr;
    }

  private:
    // The ObserverList we are iterating over.
    ObserverListBase<ObserverType>* m_list;

    // The current index we are on.
    USize m_index{0};
  };

  ObserverListBase() : m_notifyDepth(0) {}

  // Add an observer to the list.  An observer should not be added to the same
  // list more than once.
  void addObserver(ObserverType* observer) {
    DCHECK(observer);

    if (std::find(std::begin(m_observers), std::end(m_observers), observer) != std::end(m_observers)) {
      NOTREACHED() << "Observers can only be added once!";
      return;
    }

    m_observers.pushBack(observer);
  }

  // Remove an observer from the list if it is in the list.
  void removeObserver(ObserverType* observer) {
    DCHECK(observer);

    auto it = std::find(std::begin(m_observers), std::end(m_observers), observer);
    if (it != std::end(m_observers)) {
      if (m_notifyDepth) {
        *it = nullptr;
      } else {
        m_observers.remove(it);
      }
    }
  }

  // Determine whether a particular observer is in the list.
  bool hasObserver(const ObserverType* observer) const {
    return std::find(std::begin(m_observers), std::end(m_observers), observer) != std::end(m_observers);
  }

  // Clear out all the observers from the list.
  void clear() {
    if (m_notifyDepth) {
      std::fill(std::begin(m_observers), std::end(m_observers), nullptr);
    } else {
      m_observers.clear();
    }
  }

protected:
  bool isEmpty() const {
    return m_observers.isEmpty();
  }

  void compact() {
    // m_observers.erase(std::remove(std::begin(m_observers), std::end(m_observers), nullptr), std::end(m_observers));
  }

private:
  friend class Iterataor;

  using ListType = nu::DynamicArray<ObserverType*>;

  // The list of observer.
  ListType m_observers;

  // The notify depth.
  I32 m_notifyDepth{0};

  DISALLOW_COPY_AND_ASSIGN(ObserverListBase);
};

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_UTILS_DETAIL_OBSERVER_LIST_H_
