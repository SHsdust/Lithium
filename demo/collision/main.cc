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

#include "SDLdefs.h"
#include "gameloop.h"
#include "support.h"
#include "ctrl.h"
#include "li.h"

#ifdef main
#undef main
#endif

int main(int argc, const char **argv)
{
  (void)argc;
  (void)argv;

  Assert_SDL_Call(::SDL_Init(SDL_INIT_VIDEO) == 0);

  ::SDL_DisplayMode dm;
  ::SDL_GetCurrentDisplayMode(0, &dm);

  Li_TraceF("dm.w = %d, dm.h = %d", dm.w, dm.h);

  auto view_w = dm.w,
       view_h = dm.h;

  auto *window = Assert_SDL_Acquire(
    ::SDL_CreateWindow(
      "AppFrame",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      view_w,
      view_h,
      SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
  ));

  auto *renderer = Assert_SDL_Acquire(
    ::SDL_CreateRenderer(window,
                         -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  ));

  Li_TraceF("SDL is ready.");

  ::SDL_Event e;

  using namespace ::Li;
  using namespace ::Li::combinators;

  auto side = std::min(view_w, view_h) - 10;

  ::SDL_Rect game_view = { 5, 5, side, side };

  ::Li::Game_Loop game_loop;

  game_loop.add_unit(own((Unit *)new Ctrl));

  do
  {
    // ::SDL_Delay(10);

    ::SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    ::SDL_RenderClear(renderer);

    ::SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    ::SDL_RenderDrawRect(renderer, &game_view);

    game_loop.update(16.0f / 1000, 1);
    game_loop.render(renderer, 5, 5, side, side);

    ::SDL_RenderPresent(renderer);
    while (::SDL_PollEvent(&e) && e.type != ::SDL_QUIT);
  } while (e.type != ::SDL_QUIT);

  Li::free_SDL_something(renderer);
  Li::free_SDL_something(window);

  return 0;
}


