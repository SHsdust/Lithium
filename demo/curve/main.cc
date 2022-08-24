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

#include "../../src/appframe/SDLdefs.h"
#include "../../src/config.h"
#include "../../src/combinators/sciencebook.h"
#include "../../src/combinators/alchemybook.h"
#include "../../src/support.h"

#ifdef main
#undef main
#endif

static inline
::SDL_Point transform(Li::vec2_t v, Li::real_t from_size, int to_size)
{
  return {
    int((v.x + from_size) / (2 * from_size) * to_size),
    int(to_size - (v.y + from_size) / (2 * from_size) * to_size)
  };
}

int main(int argc, const char **argv)
{
  (void)argc;
  (void)argv;

  Assert_SDL_Call(::SDL_Init(SDL_INIT_VIDEO) == 0);

  ::SDL_DisplayMode dm;
  ::SDL_GetCurrentDisplayMode(0, &dm);

  Li_TraceF("dm.w = %d, dm.h = %d", dm.w, dm.h);

  auto *window = Assert_SDL_Acquire(
    ::SDL_CreateWindow(
      "AppFrame",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      dm.w,
      dm.h,
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

#define curve(CT, EP)                            \
  f_repeat(t_query_curve(CT, EP, 1000, 5.0f, 0), \
           f_const(1000.0f),                     \
           5.0f,                                 \
           INF_LOOP)

#define curve_group(CT)           \
    curve(CT, EP_Ease_In)         \
  , curve(CT, EP_Ease_Out)        \
  , curve(CT, EP_Ease_In_Out)     \
  , curve(CT, EP_Ease_Out_In)

#define Gen(G)      \
  G(CT_Linear)      \
, G(CT_Quadratic)   \
, G(CT_Cubic)       \
, G(CT_Quartic)     \
, G(CT_Quintic)     \
, G(CT_Sinusoidal)  \
, G(CT_Exponential) \
, G(CT_Circular)

  R_Filter_Ptr curves[]    = { Gen(curve_group) };
  constexpr size_t ncurves = sizeof (curves) / sizeof (curves[0]);

  Update_TickTack_Info ticktack;

  ticktack.delta_time_elpased = 0.01f;
  ticktack.delta_tick         = 1;

  ::SDL_Rect view = { 0, 0, dm.w, dm.h };

  do
  {
    ::SDL_Delay(10);

    ::SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    ::SDL_RenderClear(renderer);



    int colors[][3] = {
      {   0,   0, 255 }
    , {   0, 255,   0 }
    , { 255,   0,   0 }
    , { 255, 255, 255 }
    };

    for (size_t idx = 0; idx != ncurves; ++idx, ++ticktack.global_tick)
    {
      auto result = curves[idx]->apply(ticktack);

      int w = result / 1000 * (dm.w - 10);
      int y = (dm.h - 10) / ncurves * idx + 5;

      auto &color = colors[idx % 4];

      ::SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);

      ::SDL_Rect cursor = { 5, y, w, 5 };
      ::SDL_RenderDrawRect(renderer, &cursor);
    }

    ::SDL_RenderPresent(renderer);

    while (::SDL_PollEvent(&e) && e.type != ::SDL_QUIT);
  } while (e.type != ::SDL_QUIT);

  Li::free_SDL_something(renderer);
  Li::free_SDL_something(window);

  return 0;
}


