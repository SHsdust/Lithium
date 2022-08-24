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

#ifndef STDALIAS_H_INCLUDED_TG17TJWA
#define STDALIAS_H_INCLUDED_TG17TJWA

#include "stdincl.h"

namespace Li {

using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

using std::size_t;

template <typename T>
using Opt = std::experimental::optional<T>;

using Str      = std::string;
using Str_List = std::vector<Str>;
using Str_View = std::experimental::string_view;

template <typename ...T> using Seq        = std::vector<T...>;
template <typename ...T> using List       = std::list<T...>;
template <typename ...T> using Queue      = std::queue<T...>;
template <typename ...T> using Deque      = std::deque<T...>;
template <typename ...T> using Stack      = std::stack<T...>;
template <typename ...T> using Map        = std::map<T...>;
template <typename ...T> using Set        = std::set<T...>;
template <typename ...T> using Arc        = std::shared_ptr<T...>;
template <typename ...T> using Wrc        = std::weak_ptr<T...>;
template <typename ...T> using Ptr        = std::unique_ptr<T...>;

#define attr_ctrl(...)        __attribute__((__VA_ARGS__))
#define attr_export           attr_ctrl(visibility("default"))
#define attr_local            attr_ctrl(visibility("hidden"))
#define attr_unused           attr_ctrl(unused)
#define attr_must_use_ret     attr_ctrl(warn_unused_result)
#define attr_packed           attr_ctrl(packed)
#define attr_init(prio)       attr_ctrl(constructor(prio))
#define attr_fini(prio)       attr_ctrl(destructor(prio))

#define Li_Default_Init_Prio     (Li_Internal_Init_Prio + 1000 + 1)
#define Li_Internal_Init_Prio    (50000)

} // namespace Li

#endif // end of include guard: STDALIAS_H_INCLUDED_TG17TJWA

