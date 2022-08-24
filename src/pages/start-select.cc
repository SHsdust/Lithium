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


#include "start-select.h"
#include "main.h"
#include "play.h"
#include "playconfig.h"
#include "../route.h"
#include "../SDLdefs.h"
#include "../inithook.h"
#include "../instance.h"
#include "../RCmanager.h"
#include "../rendercontext.h"
#include "../support/debug.h"
#include "../support/bootstrap.h"
#include "../SDLinput.h"

namespace Li {

constexpr static char entries[][100] = { "Type-A", "Type-B", "Main Menu" };

Start_Select::Start_Select(int initial_life)
  : key_resp_interval(0.2f, 0.2f)
  , route_interval(0.2f)
  , initial_life(initial_life)
{ }

void Start_Select::do_update(const Update_Ctx &)
{ }


void Start_Select::update_page(Route *route, real_t dt, utick_t)
{
  if (done && route_interval.next(dt))
  {
    if (selection == ST_Back_To_Main_Menu)
      route->route_to(std::make_shared<Main_Page>());
    else
    {
      Str ship = selection == ST_Type_A ? "player-ship.Type-A" : "player-ship.Type-B";

      auto pc = Play_Config { { }, { }, { "s" }, ship };
      pc.player_resource.life = initial_life;

      route->route_to(std::make_shared<Game_Play>(
            pc
          , std::make_unique<SDL_Input_Source>()
          , route->get_instance()));
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
    selection = (selection + 1 + ST_Total) % ST_Total;
    key_resp_interval.reset();

    Li_LogI("select: %d - %s", selection, entries[selection]);
  }

  if (ks[SDL_SCANCODE_UP])
  {
    selection = (selection - 1 + ST_Total) % ST_Total;
    key_resp_interval.reset();

    Li_LogI("select: %d - %s", selection, entries[selection]);
  }
}

void Start_Select::render_page(Route *route, const Render_Context &rc)
{
  auto instance = route->get_instance();
  auto mgr      = instance->get_rc_mgr();
  auto renderer = get_renderer();

  for (int i = 0; i != ST_Total; ++i)
  {
    auto c   = 255 - std::abs(i - selection) * 47;

    auto pos       = rc.transform({ 800, -100 - i * 60 });
    auto texture   = mgr->query_text_texture(entries[i], "res/opensans.ttf", 21, c, c, c);
    auto text_rect = ::SDL_Rect { pos.x, pos.y, texture.w, texture.h };

    if (i == selection)
      text_rect.x -= 10;

    ::SDL_RenderCopy(renderer, texture.texture.get(), nullptr, &text_rect);
  }

  if (selection != ST_Back_To_Main_Menu)
  {
    Str pic = selection == ST_Type_A ? "typea" : "typeb";

    auto pic_rect = rc.transform({ 0, 0 }, { 1000.0f, 1000.0f});
    auto pic_tex  = mgr->query_texture_by_name(pic);

    ::SDL_RenderCopy(renderer, pic_tex.texture.get(), nullptr, &pic_rect);
  }
}

static inline
void load_pic(RC_Mgr *mgr)
{
  mgr->register_texture("res/typea.jpg", "typea");
  mgr->register_texture("res/typeb.jpg", "typeb");
}

Li_Game_Init("reg.load.select-pic", "SDL-init", inst)
{
  auto handle = inst->get_init_handle();

  bootstrap::init_append_early_init(handle, "load.select-pic",
                                    [inst](bootstrap::Bootstrap_Args &,
                                           const bootstrap::Bootstrap_Reg &)
                                    {
                                      load_pic(inst->get_rc_mgr());
                                    });
}


} // namespace Li

