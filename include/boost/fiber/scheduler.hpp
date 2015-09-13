//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_FIBER_MANAGER_H
#define BOOST_FIBERS_FIBER_MANAGER_H

#include <cstddef>
#include <chrono>
#include <memory>
#include <vector>

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/fiber/context.hpp>
#include <boost/fiber/detail/autoreset_event.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/clock_cast.hpp>
#include <boost/fiber/detail/queues.hpp>
#include <boost/fiber/detail/spinlock.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {

class context;
struct sched_algorithm;

class BOOST_FIBERS_DECL scheduler {
private:
    typedef detail::ready_queue< context >          ready_queue_t;
    typedef detail::sleep_queue< context >          sleep_queue_t;
    typedef std::vector< context * >                terminated_queue_t;
    typedef detail::remote_ready_queue< context * > remote_ready_queue_t;

    detail::autoreset_event                 ev_;
    std::unique_ptr< sched_algorithm >      sched_algo_;
    context                             *   main_context_;
    ready_queue_t                           ready_queue_;
    sleep_queue_t                           sleep_queue_;
    terminated_queue_t                      terminated_queue_;
    remote_ready_queue_t                    remote_ready_queue_;

    void resume_( context *, context *);

public:
    scheduler( context *) noexcept;

    scheduler( scheduler const&) = delete;
    scheduler & operator=( scheduler const&) = delete;

    virtual ~scheduler() noexcept;

    void spawn( context *);

    void run( context *);

    bool wait_until( context *,
                     std::chrono::steady_clock::time_point const&);

    void yield( context *);

    void signal( context *);

    void remote_signal( context *);

    size_t ready_fibers() const noexcept;

    void set_sched_algo( std::unique_ptr< sched_algorithm >);
};

}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_FIBERS_FIBER_MANAGER_H
