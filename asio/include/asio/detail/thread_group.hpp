//
// detail/thread_group.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_THREAD_GROUP_HPP
#define ASIO_DETAIL_THREAD_GROUP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/scoped_ptr.hpp"
#include "asio/detail/thread.hpp"

namespace asio {
namespace detail {

class thread_group
{
public:
  // Constructor initialises an empty thread group.
  thread_group()
    : first_(0)
  {
  }

  // Destructor joins any remaining threads in the group.
  ~thread_group()
  {
    join();
  }

  // Create a new thread in the group.
  template <typename Function>
  void create_thread(Function f)
  {
    first_ = new item(f,
#ifdef ASIO_HAS_POLYFILL_THREAD
                      cpf::thread::attributes{},
#endif
                      first_);
  }

  // Create new threads in the group.
  template <typename Function>
  void create_threads(Function f, std::size_t num_threads)
  {
    for (std::size_t i = 0; i < num_threads; ++i)
      create_thread(f);
  }

#ifdef ASIO_HAS_POLYFILL_THREAD
  // Create a new thread in the group.
  template <typename Function>
  void create_thread(Function f, const cpf::thread::attributes& attr)
  {
    first_ = new item(f, attr, first_);
  }

  // Create new threads in the group.
  template <typename Function>
  void create_threads(Function f, const cpf::thread::attributes& attr, std::size_t num_threads)
  {
    for (std::size_t i = 0; i < num_threads; ++i)
      create_thread(f, attr);
  }


  cpf::thread::priority_type priority() const { return first_->thread_.priority(); }
  void priority(cpf::thread::priority_type value)
  {
    item* current = first_;
    while(current)
    {
      current->thread_.priority(value);
      current = current->next_;
    }
  }

  cpf::thread::native_priority_type native_priority() const { return first_->thread_.native_priority();}
  void native_priority(cpf::thread::native_priority_type value)
  {    item* current = first_;
    while(current)
    {
      current->thread_.native_priority(value);
      current = current->next_;
    }
  }

  cpf::thread::affinity_type affinity() const { return first_->thread_.affinity();}
  void affinity(const cpf::thread::affinity_type& affinity)
  {
    item* current = first_;
    while(current)
    {
      current->thread_.affinity(affinity);
      current = current->next_;
    }
  }

  cpf::thread::dtor_action_type dtor_action() const noexcept{ return first_->thread_.dtor_action();}
  void dtor_action(cpf::thread::dtor_action_type action) noexcept{
    item* current = first_;
    while(current)
    {
      current->thread_.dtor_action(action);
      current = current->next_;
    }
  }
#endif

  // Wait for all threads in the group to exit.
  void join()
  {
    while (first_)
    {
      first_->thread_.join();
      item* tmp = first_;
      first_ = first_->next_;
      delete tmp;
    }
  }

private:
  // Structure used to track a single thread in the group.
  struct item
  {
    template <typename Function>
    explicit item(Function f,
#ifdef ASIO_HAS_POLYFILL_THREAD
        const cpf::thread::attributes& attr,
#endif
        item* next)
      : thread_(f
#ifdef ASIO_HAS_POLYFILL_THREAD
      ,attr
#endif
      ),
        next_(next)
    {
    }

    asio::detail::thread thread_;
    item* next_;
  };

  // The first thread in the group.
  item* first_;
};

} // namespace detail
} // namespace asio

#endif // ASIO_DETAIL_THREAD_GROUP_HPP
