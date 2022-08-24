/*
 *       _,.--.,_                             _,.--.,_
 *      |  _____ |                           | _____  |
 *      |-'     `'._________________________,'`     `-|
 *    ,'    ___    `.                     ,'     .-.   ``.
 *   /     |   |     \                   /      (/_\)     \
 *  !  ___  \ /  ___  |                 !   ,-.  `-'  ,-.  |
 *  | |___>     <___| !_SELECT   START__|  ([ ])     ( O ) !
 *  !       / \      ____`-.       .-'____  `-'  .-.  `-'  |
 *  |\     |___|   ,'    `. \     / ,'    `.    ( X )     /|
 *  | `~          /        \ |   | /        \    `-'    ,' |
 *  |   `-~___,-. \        / |___| \        / ,-.____,-'   |
 *  |           '\ `.____,' /     \ `.____,' /`.           |
 *  |          /  `-.____,-'       `-.____,-'   \          |
 *  \         /                                  \         /
 *   \       /                                    \       /
 *    `.__,-'                                      `-.__,'
 *
 *
 */

#ifndef SPAN_H_INCLUDED_NZGPZDHX
#define SPAN_H_INCLUDED_NZGPZDHX

#include "../prelude/stdincl.h"
#include "../prelude/stdalias.h"
#include "../prelude/pp.h"

namespace Li {
namespace details {
namespace span {

template <class Iter>
struct Iter_Enumerator
{
  Iter first, last;

  inline auto next()
  {
    using T = std::decay_t<decltype(*first)>;

    if (first != last)
      return Opt<T>(*first++);
    else
      return Opt<T> { };
  }
};

template <class Iter>
static inline
auto make_iter_enumerator(Iter first, Iter last)
{
  return Iter_Enumerator<Iter> { first, last };
}

template <class Enum, class MapFn>
struct Map_Enumerator
{
  Enum   enumerator;
  MapFn  mapfn;

  inline auto next()
  {
    using T = std::decay_t<decltype(mapfn(*enumerator.next()))>;

    if (auto next = enumerator.next())
      return Opt<T> { mapfn(*next) };
    else
      return Opt<T> { };
  }
};

template <class Enum, class MapFn>
static inline
auto make_map_enumerator(Enum  &&enumerator,
                         MapFn &&mapfn)
{
  return Map_Enumerator<Enum, MapFn>
    { std::forward<Enum>(enumerator)
    , std::forward<MapFn>(mapfn) };
}

template <class Enum, class FilterFn>
struct Filter_Enumerator
{
  Enum     enumerator;
  FilterFn filterfn;

  inline auto next()
  {
    using T = std::decay_t<decltype(*enumerator.next())>;

    while (auto e = enumerator.next())
    {
      if (filterfn(*e))
        return Opt<T> { *e };
    }

    return Opt<T> { };
  }
};

template <class Enum, class FilterFn>
static inline
auto make_filter_enumerator(Enum     &&enumerator,
                            FilterFn &&filterfn)
{
  return Filter_Enumerator<Enum, FilterFn>
    { std::forward<Enum>(enumerator)
    , std::forward<FilterFn>(filterfn) };
}

template <class Proc>
struct Span_Proc
{
  Proc proc;
};

template <class Proc>
static inline auto make_span_proc(Proc &&proc)
{
  return Span_Proc<Proc> { std::forward<Proc>(proc) };
}

template <class Proc, class MapFn>
static inline
auto append_map_trans(Span_Proc<Proc> &&span_proc,
                      MapFn           &&mapfn)
{
  auto next_proc = [ proc  = std::move(span_proc.proc)
                   , mapfn = std::forward<MapFn>(mapfn) ] (auto e)
  {
    return make_map_enumerator(proc(e), std::move(mapfn));
  };

  return make_span_proc(std::move(next_proc));
}

template <class Proc, class FilterFn>
static inline
auto append_filter_trans(Span_Proc<Proc> &&span_proc,
                         FilterFn        &&filterfn)
{
  auto next_proc = [ proc     = std::move(span_proc.proc)
                   , filterfn = std::forward<FilterFn>(filterfn) ] (auto e)
  {
    return make_filter_enumerator(proc(e), std::move(filterfn));
  };

  return make_span_proc(std::move(next_proc));
}

template <class MapFn>
struct MapFn_Wrapper
{
  MapFn mapfn;
};

template <class FilterFn>
struct FilterFn_Wrapper
{
  FilterFn filterfn;
};

template <class MapFn>
static inline
auto make_mapfn(MapFn &&mapfn)
{
  return MapFn_Wrapper<MapFn> { std::forward<MapFn>(mapfn) };
}

template <class FilterFn>
static inline
auto make_filterfn(FilterFn &&filterfn)
{
  return FilterFn_Wrapper<FilterFn> { std::forward<FilterFn>(filterfn) };
}

template <template <class ...> class Container>
struct Collect_Wrapper { };

template <typename Enum, template <typename ...> class Seq>
static inline
auto make_container()
{
  using T = std::decay_t<decltype(*std::declval<Enum>().next())>;

  return Seq<T> { };
}

#define Generate_Collect_Impl(via)                               \
template <class Enum, template <typename ...> class Container>   \
struct Li_Cat2(Via_, via)                                        \
{                                                                \
  static inline auto collect(Enum enumerator)                    \
  {                                                              \
    auto output = make_container<Enum, Container>();             \
                                                                 \
    while (auto e = enumerator.next())                           \
      output.via(std::move(*e));                                 \
                                                                 \
    return output;                                               \
  }                                                              \
}

Generate_Collect_Impl(push_back);
Generate_Collect_Impl(push);
Generate_Collect_Impl(insert);

#define Generate_Collector(C, via)                           \
template <>                                                  \
struct Collect_Wrapper<C>                                    \
{                                                            \
  template <class Enum>                                      \
  static inline auto collect(Enum enumerator)                \
  {                                                          \
    return Li_Cat2(Via_, via)<Enum, C>::collect(enumerator); \
  }                                                          \
}

Generate_Collector(Seq, push_back);
Generate_Collector(Deque, push_back);
Generate_Collector(List, push_back);
Generate_Collector(Queue, push);
Generate_Collector(Stack, push);
Generate_Collector(Set, insert);
Generate_Collector(Map, insert);

#undef Generate_Collect_Impl
#undef Generate_Collector

template <template <class ...> class Container>
static inline
auto make_collector()
{
  return Collect_Wrapper<Container> { };
}

template <class Span, class FilterFn>
static inline
auto operator |(Span &&span, FilterFn_Wrapper<FilterFn> &&filter)
{
  return append_filter_trans(std::forward<Span>(span), std::move(filter.filterfn));
}

template <class Span, class MapFn>
static inline
auto operator |(Span &&span, MapFn_Wrapper<MapFn> &&map)
{
  return append_map_trans(std::forward<Span>(span), std::move(map.mapfn));
}

template <class Span, class Collector>
struct Applier
{
  Span      span;
  Collector collector;

  template <class Iter>
  inline
  auto operator ()(Iter first, Iter last) const
  {
    return collector.collect(span.proc(make_iter_enumerator(first, last)));
  }

  template <class Enum>
  inline
  auto operator ()(Enum enumerator) const
  {
    return collector.collect(span.proc(enumerator));
  }

  template <class Container>
  inline
  auto operator |(const Container &container) const
  {
    return (*this)(std::begin(container), std::end(container));
  }
};

template <class Span, template <class ...> class Container>
static inline
auto operator |(Span &&span, Collect_Wrapper<Container> &&collector)
{
  return Applier<Span, Collect_Wrapper<Container>>
    { std::move(span)
    , std::move(collector) };
}


static inline
auto span_start()
{
  return make_span_proc([] (auto e) { return e; });
}

struct FilterFn_Candy
{
  template <class FilterFn>
  inline
  auto operator +(FilterFn &&filterfn)
  {
    return make_filterfn(std::forward<FilterFn>(filterfn));
  }
};

struct MapFn_Candy
{
  template <class MapFn>
  inline
  auto operator +(MapFn &&mapfn)
  {
    return make_mapfn(std::forward<MapFn>(mapfn));
  }
};


#define $span    ::Li::details::span::span_start()
#define $$filter ::Li::details::span::FilterFn_Candy() +
#define $$map    ::Li::details::span::MapFn_Candy()    +

#define $map(...)     $$map    [](__VA_ARGS__)
#define $filter(...)  $$filter [](__VA_ARGS__)
#define $map$         $$map    [](const auto &$)
#define $filter$      $$filter [](const auto &$)

#define $collect(as) ::Li::details::span::make_collector<as>()

} // namespace span

} // namespace details
} // namespace Li


#endif // end of include guard: SPAN_H_INCLUDED_NZGPZDHX

