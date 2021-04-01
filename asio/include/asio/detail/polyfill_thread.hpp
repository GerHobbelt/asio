//
// detail/polyfill_thread.hpp
// ~~~~~~~~~~~~~~~~~~~~~


#ifndef ASIO_DETAIL_POLYFILL_THREAD_HPP
#define ASIO_DETAIL_POLYFILL_THREAD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"

#if defined(ASIO_HAS_POLYFILL_THREAD)

#include <polyfill/thread.hpp>
#include "asio/detail/noncopyable.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace detail {

class polyfill_thread
  : private noncopyable
{
public:
  // Constructor.
  template <typename Function>
  polyfill_thread(Function f, const cpf::thread::attributes& attributes = {}, unsigned int = 0)
    : thread_(attributes , f)
  {
  }

  // Wait for the thread to exit.
  void join()
  {
    if (thread_.joinable())
      thread_.join();
  }

  // Get number of CPUs.
  static std::size_t hardware_concurrency()
  {
    return cpf::thread::hardware_concurrency();
  }

  cpf::thread::priority_type priority() const { return thread_.priority(); }
  void priority(cpf::thread::priority_type value) { thread_.priority(value); }

  cpf::thread::native_priority_type native_priority() const { return thread_.native_priority(); }
  void native_priority(cpf::thread::native_priority_type value) { thread_.native_priority(value); }

  cpf::thread::affinity_type affinity() const { return thread_.affinity(); }
  void affinity(const cpf::thread::affinity_type& affinity) { thread_.affinity(affinity); }

  cpf::thread::dtor_action_type dtor_action() const noexcept{ return thread_.dtor_action();}
  void dtor_action(cpf::thread::dtor_action_type action) noexcept{ thread_.dtor_action(action); }

private:
  cpf::thread thread_;
};

} // namespace detail
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // defined(ASIO_HAS_POLYFILL_THREAD)

#endif // ASIO_DETAIL_POLYFILL_THREAD_HPP
