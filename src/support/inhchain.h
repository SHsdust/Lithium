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

#ifndef INHCHAIN_H_INCLUDED_TQS5IHBS
#define INHCHAIN_H_INCLUDED_TQS5IHBS

#include "../config.h"

namespace Li {
namespace details {

struct End_Of_Chain;

template <class Base, class I, class ...R>
struct chain_impl
{
  using Klass = typename I::template Closure<typename chain_impl<Base, R...>::Klass>;
};

template <class Base>
struct chain_impl<Base, End_Of_Chain>
{
  using Klass = Base;
};

} // namespace details

template <class Base, class ...R>
class chain : public details::chain_impl<Base, R..., details::End_Of_Chain>::Klass
{
private:
  using Chain_Base = typename details::chain_impl<Base, R..., details::End_Of_Chain>::Klass;
protected:
  using Super = chain;
public:
  using Chain_Base::Chain_Base;
};

template <template <class ...> class F, class ...Partial_Args>
struct partially_apply
{
  template <class Base> using Closure = F<Base, Partial_Args...>;
};

#define $partially_apply(x)             $partially_apply_helper x
#define $partially_apply_helper(...)    Li_Prefix partially_apply<__VA_ARGS__>
#define $chain(Base, ...)               Li_Prefix chain<Base, Li_Map($partially_apply, __VA_ARGS__)>

} // namespace Li

#endif // end of include guard: INHCHAIN_H_INCLUDED_TQS5IHBS

