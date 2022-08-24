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

#ifndef ROUTE_H_INCLUDED_UN9OV08X
#define ROUTE_H_INCLUDED_UN9OV08X

#include "config.h"

namespace Li {

class Page;
class Game_Instance;
struct Render_Context;

class Route
{
  class Route_Impl;
  Ptr<Route_Impl> pimpl;
public:
   Route(Game_Instance *game_instance);
  ~Route();

  Game_Instance *get_instance() const;

  void route_to(Arc<Page>  to);
  void push_page(Arc<Page> p);
  void pop_page();

  void clear_pages();
  void mark_terminated();
  bool is_terminated() const;

  void update(real_t dt, utick_t dticks);
  void render(const Render_Context &rc);

  bool has_page() const;
};

} // namespace Li

#endif // end of include guard: ROUTE_H_INCLUDED_UN9OV08X

