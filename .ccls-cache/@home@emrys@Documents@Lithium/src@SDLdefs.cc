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
#include "support/debug.h"
#include "support/misc.h"
#include "support/bootstrap.h"
#include "support/math.h"

namespace Li {

#define IMPL(Something, Free)                                    \
  void free_SDL_something(Li_Join(::SDL_, Something) *something) \
  {                                                              \
    if (something) Li_Join(::SDL_, Free, Something)(something);  \
  }

IMPL(Window, Destroy)
IMPL(Renderer, Destroy)
IMPL(Surface, Free)
IMPL(Texture, Destroy)

struct SDL_Init_Helper
{
  ::SDL_Renderer *renderer;
  ::SDL_Window   *window;
  ::SDL_Rect      view;

  SDL_Init_Helper(bool fullscreen)
  {
    Li_LogI("fullscreen = %s", fullscreen ? "yes" : "no");

    Assert_SDL_Call(::SDL_Init(SDL_INIT_VIDEO) == 0);

    ::SDL_DisplayMode dm;
    ::SDL_GetCurrentDisplayMode(0, &dm);

    Li_LogI("dm.w = %d, dm.h = %d", dm.w, dm.h);

    auto view_w = fullscreen ? dm.w : dm.w * 10 / 15,
         view_h = fullscreen ? dm.h : dm.h * 10 / 15;

    Li_LogI("view_w = %d, view_h = %d", view_w, view_h);

    auto ws = fullscreen ? SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN : SDL_WINDOW_SHOWN;

    auto *window = Assert_SDL_Acquire(
      ::SDL_CreateWindow(
        "Lithium ver.0.0.1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        view_w,
        view_h,
        ws
    ));

    renderer = Assert_SDL_Acquire(
      ::SDL_CreateRenderer(window,
                           -1,
                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    ));

    view = { 0, 0, view_w, view_h };

    Li_LogI("SDL is ready.");
  }

  ~SDL_Init_Helper()
  {
    free_SDL_something(window);
    free_SDL_something(renderer);

    Li_LogI("SDL closed.");
  }
};

static
SDL_Init_Helper *get_SDL_init_helper(bool fullscreen)
{
  static SDL_Init_Helper _(fullscreen);
  return &_;
}

Li_Bootstrap_Register("SDL-init", "");
Li_Bootstrap_Append_Dependences("game-init", "SDL-init");
Li_Bootstrap_Append_Routine("SDL-init", early_init)
{
  Li_LogI("init args(from app) : %s", str_join($args.args, ","));

  Set<Str> params = { $args.args.begin(), $args.args.end() };

  if (u_has(params, "-f"))
  {
    Li_LogI("Initialize window with fullscreen arg.");
    (void)get_SDL_init_helper(true);
  }
}


::SDL_Renderer *get_renderer()
{
  return get_SDL_init_helper(false)->renderer;
}

::SDL_Rect      get_view_rect()
{
  return get_SDL_init_helper(false)->view;
}

} // namespace Li

