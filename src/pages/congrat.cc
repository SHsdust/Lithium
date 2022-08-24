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

#include "congrat.h"
#include "main.h"

namespace Li {

void Congrat::update_page(Route *route, real_t dt, utick_t)
{
  if (done && route_interval.next(dt))
    route->route_to(std::make_shared<Main_Page>());

  if (done)
    return;

  auto ks = ::SDL_GetKeyboardState(nullptr);

  if (ks[SDL_SCANCODE_RETURN])
    done = true;
}

void Congrat::render_page(Route *route, const Render_Context &rc)
{
  game_update->render(rc);

  auto instance = route->get_instance();
  auto mgr      = instance->get_rc_mgr();
  auto renderer = get_renderer();

  auto pos       = rc.transform({ 800, -100 });
  auto c         = 255;
  auto texture   = mgr->query_text_texture("Back", "res/opensans.ttf", 21, c, c, c);
  auto text_rect = ::SDL_Rect { pos.x, pos.y, texture.w, texture.h };

  ::SDL_RenderCopy(renderer, texture.texture.get(), nullptr, &text_rect);

  c = 100 + std::sin(get_time_elpased() * 3) * 100 + 5;
  auto title_pos  = rc.transform({ -100, 100 });
  auto title      = mgr->query_text_texture("Congratulation!", "res/opensans.ttf", 32, c, c, c);
  auto title_rect = ::SDL_Rect { title_pos.x, title_pos.y, title.w, title.h };

  ::SDL_RenderCopy(renderer, title.texture.get(), nullptr, &title_rect);
}

Congrat::~Congrat()
{ }

Congrat::Congrat(const Congrat &)
{ Li_PanicF("DO NOT CALL Congrat's COPY-CONSTRUCTOR!"); }

void Congrat::do_update(const Update_Ctx &)
{ }

} // namespace Li

