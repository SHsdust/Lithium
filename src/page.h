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

#ifndef PAGE_H_INCLUDED_DFGY6VT2
#define PAGE_H_INCLUDED_DFGY6VT2

#include "config.h"
#include "core/unit.h"

namespace Li {

struct Render_Context;
class  Route;
class Page : public Unit
{
public:
  virtual void update_page(Route *route, real_t dt, utick_t dticks) = 0;
  virtual void render_page(Route *route, const Render_Context &rc)  = 0;
};

} // namespace Li

#endif // end of include guard: PAGE_H_INCLUDED_DFGY6VT2

