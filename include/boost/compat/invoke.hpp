#ifndef BOOST_COMPAT_INVOKE_HPP_INCLUDED
#define BOOST_COMPAT_INVOKE_HPP_INCLUDED

// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/compat/mem_fn.hpp>
#include <boost/compat/type_traits.hpp>
#include <boost/compat/detail/returns.hpp>
#include <utility>

namespace boost {
namespace compat {

// invoke

template<class F, class... A>
constexpr auto invoke( F&& f, A&&... a )
BOOST_COMPAT_RETURNS( std::forward<F>(f)(std::forward<A>(a)...) )

template<class M, class T, class... A>
constexpr auto invoke( M T::* pm, A&&... a )
BOOST_COMPAT_RETURNS( mem_fn(pm)(std::forward<A>(a)...) )

// invoke_result_t

template<class F, class... A> using invoke_result_t = decltype( compat::invoke( std::declval<F>(), std::declval<A>()... ) );

// is_invocable

namespace detail {

template<class, class F, class... A> struct is_invocable_: std::false_type {};
template<class F, class... A> struct is_invocable_< void_t<invoke_result_t<F, A...>>, F, A... >: std::true_type {};

} // namespace detail

template<class F, class... A> struct is_invocable: detail::is_invocable_<void, F, A...> {};

// is_nothrow_invocable

namespace detail {

template<class, class F, class... A> struct is_nothrow_invocable_: std::false_type {};
template<class F, class... A> struct is_nothrow_invocable_< enable_if_t< noexcept( compat::invoke( std::declval<F>(), std::declval<A>()... ) ) >, F, A... >: std::true_type {};

} // namespace detail

template<class F, class... A> struct is_nothrow_invocable: detail::is_nothrow_invocable_<void, F, A...> {};

} // namespace compat
} // namespace boost

#endif // BOOST_COMPAT_INVOKE_HPP_INCLUDED
