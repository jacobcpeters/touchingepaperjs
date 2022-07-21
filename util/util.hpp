#ifndef UTIL_HPP
#define UTIL_HPP

#include <memory>

namespace Util
{
  template <class T>
  using Scope = std::unique_ptr<T>;
  template<typename T, typename... Args>
  Scope<T> CreatScope(Args&&... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <class T>
  using Ref = std::shared_ptr<T>;
  template<typename T, typename... Args>
  Ref<T> CreatRef(Args&&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

} // namespace Util


#endif /* UTIL_HPP */
