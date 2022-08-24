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


#include "main.h"
#include "start-select.h"
#include "about.h"
#include "../route.h"
#include "../SDLdefs.h"
#include "../inithook.h"
#include "../instance.h"
#include "../RCmanager.h"
#include "../rendercontext.h"
#include "../support/debug.h"
#include "../support/bootstrap.h"

namespace Li {

constexpr static char entries[][100] = { "Start", "Practice",  "About", "Quit" };

Main_Page::Main_Page()
  : key_resp_interval(0.2f, 0.2f)
  , route_interval(0.2f)
{ }

void Main_Page::do_update(const Update_Ctx &)
{ }

void Main_Page::update_page(Route *route, real_t dt, utick_t)
{
  if (done && route_interval.next(dt))
  {
    if (selection == MME_Start)
      route->route_to(std::make_shared<Start_Select>(5));

    if (selection == MME_Practice_Start)
      route->route_to(std::make_shared<Start_Select>(100));

    if (selection == MME_About)
      route->route_to(std::make_shared<About_Page>());

    if (selection == MME_Quit)
      route->mark_terminated();

    done = false;
    return;
  }

  if (done)
    return;

  auto ks = ::SDL_GetKeyboardState(nullptr);

  if (!key_resp_interval.forward(dt))
    return;

  if (ks[SDL_SCANCODE_RETURN])
  {
    done = true;
    return;
  }


  if (ks[SDL_SCANCODE_DOWN])
  {
    selection = (selection + 1 + MME_Total) % MME_Total;
    key_resp_interval.reset();

    Li_LogI("select: %d - %s", selection, entries[selection]);
  }

  if (ks[SDL_SCANCODE_UP])
  {
    selection = (selection - 1 + MME_Total) % MME_Total;
    key_resp_interval.reset();

    Li_LogI("select: %d - %s", selection, entries[selection]);
  }
}

void Main_Page::render_page(Route *route, const Render_Context &rc)
{
  auto instance = route->get_instance();
  auto mgr      = instance->get_rc_mgr();
  auto renderer = get_renderer();

  for (int i = 0; i != MME_Total; ++i)
  {
    auto c   = 255 - std::abs(i - selection) * 47;

    auto pos       = rc.transform({ 800, -100 - i * 60 });
    auto texture   = mgr->query_text_texture(entries[i], "res/opensans.ttf", 21, c, c, c);
    auto text_rect = ::SDL_Rect { pos.x, pos.y, texture.w, texture.h };

    if (i == selection)
      text_rect.x -= 10;

    ::SDL_RenderCopy(renderer, texture.texture.get(), nullptr, &text_rect);
  }

  auto fade  = std::abs(std::sin(get_time_elpased() / 2)) * 0.5 + 0.5;
  auto title = mgr->query_text_texture("Lithium", "res/opensans.ttf", 42,
                                       uint8_t(155 * fade), uint8_t(21 * fade), uint8_t(21 * fade));
  auto pos   = rc.transform({ -300, 300 });
  auto rect  = ::SDL_Rect { pos.x, pos.y, title.w, title.h };

  ::SDL_RenderCopy(renderer, title.texture.get(), nullptr, &rect);
}

Li_Game_Init("main-menu", "", inst)
{
  auto handle = inst->get_init_handle();

  bootstrap::init_append_early_init(handle,
                                    "route.main-menu",
                                    [inst](bootstrap::Bootstrap_Args &,
                                       const bootstrap::Bootstrap_Reg &)
                                    {
                                      inst->get_route()->push_page(std::make_shared<Main_Page>());
                                      Li_LogI("Main-Menu");
                                    });
}

} // namespace Li

