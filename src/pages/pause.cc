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
#include "pause.h"
#include "../route.h"
#include "../SDLdefs.h"
#include "../inithook.h"
#include "../instance.h"
#include "../RCmanager.h"
#include "../rendercontext.h"
#include "../support/debug.h"
#include "../support/bootstrap.h"

namespace Li {

constexpr static char entries[][100] = { "Resume", "Give Up", "Quit" };


Game_Pause::Game_Pause(Page *play)
  : key_resp_interval(0.2f, 0.2f)
  , route_interval(0.2f)
  , play(play)
{ }

void Game_Pause::do_update(const Update_Ctx &)
{ }

void Game_Pause::update_page(Route *route, real_t dt, utick_t)
{
  if (done && route_interval.next(dt))
  {
    if (selection == PME_Resume)
      route->pop_page();

    if (selection == PME_Quit)
      route->mark_terminated();

    if (selection == PME_Back)
    {
      route->clear_pages();
      route->route_to(std::make_shared<Main_Page>());
    }

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
    selection = (selection + 1 + PME_Total) % PME_Total;
    key_resp_interval.reset();

    Li_LogI("select: %d - %s", selection, entries[selection]);
  }

  if (ks[SDL_SCANCODE_UP])
  {
    selection = (selection - 1 + PME_Total) % PME_Total;
    key_resp_interval.reset();

    Li_LogI("select: %d - %s", selection, entries[selection]);
  }
}

void Game_Pause::render_page(Route *route, const Render_Context &rc)
{
  play->render_page(route, rc);

  auto instance = route->get_instance();
  auto mgr      = instance->get_rc_mgr();
  auto renderer = get_renderer();

  for (int i = 0; i != PME_Total; ++i)
  {
    auto c   = 255 - std::abs(i - selection) * 47;

    auto pos       = rc.transform({ 800, -100 - i * 60 });
    auto texture   = mgr->query_text_texture(entries[i], "res/opensans.ttf", 21, c, c, c);
    auto text_rect = ::SDL_Rect { pos.x, pos.y, texture.w, texture.h };

    if (i == selection)
      text_rect.x -= 10;

    ::SDL_RenderCopy(renderer, texture.texture.get(), nullptr, &text_rect);
  }
}

} // namespace Li

