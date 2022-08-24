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

#include "instance.h"
#include "RCmanager.h"
#include "gameupdate.h"
#include "factory.h"
#include "inithook.h"
#include "inputsource.h"
#include "support.h"
#include "stage.h"
#include "route.h"
#include "support/bootstrap.h"

namespace Li {

Game_Instance::Game_Instance(const Str &domain, int argc, const char **argv)
  : rc_mgr(new RC_Mgr)
  , factory(new Obj_Factory)
  , route(new Route(this))
  , init_handle(bootstrap::acquire_handle(domain))
{
  game_bootstrap::apply_hooks(this);

  auto args = bootstrap::Bootstrap_Args { argv[0], { argv + 1, argv + argc } };
  bootstrap::init_init(init_handle, args);
}

Game_Instance::~Game_Instance()
{
  bootstrap::init_fini(init_handle);
  bootstrap::release_handle(init_handle);
}

bool Game_Instance::terminated() const
{
  return route->is_terminated();
}

void Game_Instance::run(const Render_Context &rc)
{
  auto *renderer = get_renderer();
  auto *route    = get_route();

  auto count_per_second = ::SDL_GetPerformanceFrequency();
  auto before           = ::SDL_GetPerformanceCounter();

  ::SDL_Event e;
  do
  {
    ::SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    ::SDL_RenderClear(renderer);

    route->update(16.0f / 1000, 1);
    route->render(rc);

    ::SDL_RenderPresent(renderer);

    while (::SDL_PollEvent(&e) && e.type != ::SDL_QUIT);

    auto after = ::SDL_GetPerformanceCounter();

    while ((after - before) * 60 < count_per_second)
      after = ::SDL_GetPerformanceCounter();

    before = after;
  } while (e.type != ::SDL_QUIT && !route->is_terminated());

}

} // namespace Li


