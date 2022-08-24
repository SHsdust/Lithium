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
 *                                GHL <2012.2.9.ghl@gmail.com>
 */

#include "SDLdefs.h"
#include "instance.h"
#include "gameupdate.h"
#include "rendercontext.h"

#include "support.h"

#ifdef main
#undef main
#endif

int main(int argc, const char **argv)
{
  using namespace ::Li;

  auto args = bootstrap::Bootstrap_Args { argv[0], { argv + 1, argv + argc } };

  bootstrap::init_init(Li::bootstrap::g_init_scope(), args);

  Li::Game_Instance  instance("game", argc, argv);

  auto view = get_view_rect();
  auto side = std::min(view.w, view.h) - 10;
  auto rc   = Render_Context { view.w, view.h, 5, 5, side, side };

  instance.run(rc);

  bootstrap::init_fini(Li::bootstrap::g_init_scope());

  return 0;
}



