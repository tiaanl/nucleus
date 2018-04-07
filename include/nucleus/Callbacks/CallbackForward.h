
#ifndef NUCLEUS_CALLBACKS_CALLBACK_FORWARD_H_
#define NUCLEUS_CALLBACKS_CALLBACK_FORWARD_H_

namespace nu {

template <typename Signature>
class Callback;

using Closure = Callback<void(void)>;

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_CALLBACK_FORWARD_H_
