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

#ifndef MEMBERDETECT_H_INCLUDED_7OF0EVDH
#define MEMBERDETECT_H_INCLUDED_7OF0EVDH

// Member Detector
// from https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector

#include <type_traits>
#include "../prelude/pp.h"

#define $Li_Generate_Member_Detector_With_NS(member, ns)           \
namespace ns {                                                     \
template <class Target>                                            \
struct Li_Cat2(Member_Detector_For_, member)                       \
{                                                                  \
  struct Fallback { int member; };                                 \
  struct Derived : public Target, Fallback { };                    \
                                                                   \
  template <class U>                                               \
  static std::false_type test(decltype(U::member) *);              \
  template <class U>                                               \
  static std::true_type  test(...);                                \
                                                                   \
  using result_type = decltype(test<Derived>(nullptr));            \
};                                                                 \
} /* namespace ns */                                               \
                                                                   \
template <class Target>                                            \
struct Li_Cat2(has_member_, member)                                \
  : ns::Li_Cat2(Member_Detector_For_, member)<Target>::result_type \
{ }

#define $Li_Generate_Typedef_Detector_With_NS(member, ns)           \
namespace ns {                                                      \
template <class Target>                                             \
struct Li_Cat2(Typedef_Detector_For_, member)                       \
{                                                                   \
  struct Fallback { struct member { }; };                           \
  struct Derived : public Target, Fallback { };                     \
                                                                    \
  template <class U>                                                \
  static std::false_type test(typename U::member *);                \
  template <class U>                                                \
  static std::true_type  test(U *);                                 \
                                                                    \
  using result_type = decltype(test<Derived>(nullptr));             \
};                                                                  \
} /* namespace ns */                                                \
                                                                    \
template <class Target>                                             \
struct Li_Cat2(has_typedef_, member)                                \
  : ns::Li_Cat2(Typedef_Detector_For_, member)<Target>::result_type \
{ }

#define $Li_Generate_Member_Detector(member)  $Li_Generate_Member_Detector_With_NS(member, details)
#define $Li_Generate_Typedef_Detector(member) $Li_Generate_Typedef_Detector_With_NS(member, details)
#define $Li_Generate_Member_Detector_1(m)     $Li_Generate_Member_Detector(m)
#define $Li_Generate_Typedef_Detector_1(m)    $Li_Generate_Typedef_Detector(m)
#define $Li_Generate_Member_Detector_2(m, n)  $Li_Generate_Member_Detector_With_NS(m, n)
#define $Li_Generate_Typedef_Detector_2(m, n) $Li_Generate_Typedef_Detector_With_NS(m, n)

/* {{{ export */

#define Li_Generate_Member_Detector(...) \
  Li_Cat2($Li_Generate_Member_Detector_, Li_Nargs(__VA_ARGS__))(__VA_ARGS__)

#define Li_Generate_Typedef_Detector(...) \
  Li_Cat2($Li_Generate_Typedef_Detector_, Li_Nargs(__VA_ARGS__))(__VA_ARGS__)

/* }}} */

#endif // end of include guard: MEMBERDETECT_H_INCLUDED_7OF0EVDH

