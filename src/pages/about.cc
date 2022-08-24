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

#include "about.h"

#include "../route.h"
#include "../SDLdefs.h"
#include "../inithook.h"
#include "../instance.h"
#include "../RCmanager.h"
#include "../factory.h"
#include "../stage.h"
#include "../gameupdate.h"
#include "../rendercontext.h"
#include "../support/debug.h"
#include "../inputsource.h"

#include "main.h"

namespace Li {


void About_Page::update_page(Route *route, real_t dt, utick_t)
{
  if (done && route_interval.next(dt))
    route->route_to(std::make_shared<Main_Page>());

  if (done)
    return;

  auto ks = ::SDL_GetKeyboardState(nullptr);

  if (ks[SDL_SCANCODE_RETURN])
    done = true;
}

void About_Page::render_page(Route *route, const Render_Context &rc)
{
  auto *mgr = route->get_instance()->get_rc_mgr();
  auto *renderer = get_renderer();

  Str msgs[] = {
    "Shoot        --   Z"
  , "Confirm      --   RETURN"
  , "Pause        --   ESCAPE"
  , "         hold Left Shift to enter Slow Mode."
  };

  for (int i = 0; i != 4; ++i)
  {
    auto hint_msg_pos = rc.transform({ -100, 100 - i * 64 });
    auto hint_msg_tex = mgr->query_text_texture(msgs[i], "res/opensans.ttf", 20);
    auto hint_msg_rect = ::SDL_Rect { hint_msg_pos.x, hint_msg_pos.y, hint_msg_tex.w, hint_msg_tex.h };

    ::SDL_RenderCopy(renderer, hint_msg_tex.texture.get(), nullptr, &hint_msg_rect);
  }
}

void About_Page::do_update(const Update_Ctx &)
{ }

} // namespace Li

