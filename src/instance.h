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

#ifndef INSTANCE_H_INCLUDED_QNZ3KGYI
#define INSTANCE_H_INCLUDED_QNZ3KGYI

#include "gamecfg.h"
#include "route.h"

namespace Li {

class RC_Mgr;
class Obj_Factory;

namespace bootstrap {
struct Init_Scope;
} // namespace bootstrap

namespace game_bootstrap {
class Game_Instance_Init_Hook;
} // namespace game_bootstrap

class Chapter;

class Game_Instance
{
  friend class game_bootstrap::Game_Instance_Init_Hook;

  Ptr<RC_Mgr>         rc_mgr;
  Ptr<Obj_Factory>    factory;
  Ptr<Route>          route;

  bootstrap::Init_Scope *init_handle;
public:
  Game_Instance(const Str &domain, int argc, const char **argv);
  ~Game_Instance();

  Game_Instance(const Game_Instance &) = delete;
  Game_Instance(Game_Instance &&) = default;

  inline auto *get_rc_mgr()      { return rc_mgr.get();  }
  inline auto *get_factory()     { return factory.get(); }
  inline auto *get_init_handle() { return init_handle;   }
  inline auto *get_route()       { return route.get();   }

  bool terminated() const;

  void run(const Render_Context &rc);
public:
  void operator =(const Game_Instance &) = delete;
};

} // namespace Li

#endif // end of include guard: INSTANCE_H_INCLUDED_QNZ3KGYI

