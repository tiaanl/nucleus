
#ifndef NUCLEUS_UTILS_NUMBERS_H_
#define NUCLEUS_UTILS_NUMBERS_H_

#include <limits>

namespace nu {

template <typename T>
struct NumericLimits {

    static constexpr T min() noexcept {
        return std::numeric_limits<T>::min();
    }

    static constexpr T max() noexcept {
        return std::numeric_limits<T>::max();
    }

};

}  // namespace nu

#endif  // NUCLEUS_UTILS_NUMBERS_H_
