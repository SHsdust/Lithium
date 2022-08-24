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

#include "../RCmanager.h"
#include "../instance.h"
#include "../inithook.h"
#include "../SDLdefs.h"
#include "../support/debug.h"

#include <SDL2/SDL_ttf.h>

namespace Li {


Seq<::SDL_Rect> clip_rects(int x, int y, int w, int h, int woff, int n)
{
  auto result = Seq<::SDL_Rect> { };
  auto rect   = ::SDL_Rect { x, y, w, h };

  for (auto i = 0; i != n; ++i)
  {
    result.push_back(rect);
    rect.y += woff;
  }

  return result;
}

attr_local void load_resource(RC_Mgr *mgr)
{
  using namespace std::string_literals;

  auto bullet_1 = mgr->register_texture(RC_Prefix + "bullet1.png"s, "bullet1");
  auto bullet_2 = mgr->register_texture(RC_Prefix + "bullet2.png"s, "bullet2");
  auto enemy    = mgr->register_texture(RC_Prefix + "enemy.png"s, "enemy", 72, 127, 127);
  // auto bullet_3 = mgr->register_texture(RC_Prefix + "bullet3.png"s, "bullet3");

#define CLIP(id, tcid, ...) mgr->register_tex_clips(id, tcid, clip_rects(__VA_ARGS__))

  CLIP(bullet_1, TCID_PRE_IMG, 80, 0, 32, 32, 32, 8);
  CLIP(bullet_1, TCID_GUN_BULLET, 24, 0, 16, 16, 32, 8);
  CLIP(bullet_1, TCID_GUN_BULLET_FADE, 56, 0, 16, 16, 32, 8);
  CLIP(bullet_1, TCID_BALL_MID, 176, 0, 32, 32, 32, 8);

  CLIP(bullet_2, TCID_STAR_SMALL, 96, 0, 16, 16, 32, 8);
  CLIP(bullet_2, TCID_STAR_BIG, 224, 0, 32, 32, 32, 8);
  CLIP(bullet_2, TCID_GRAIN, 128, 0, 16, 16, 32, 16);
  CLIP(bullet_2, TCID_BALL_SMALL, 176, 0, 16, 16, 32, 8);
  CLIP(bullet_2, TCID_BALL_BIG, 192, 0, 32, 32, 32, 8);

  CLIP(enemy, TCID_Enemy_A, 0, 0, 60, 70, 70, 6);
  CLIP(enemy, TCID_Enemy_B, 60, 0, 60, 70, 70, 6);
  CLIP(enemy, TCID_Enemy_Boss, 129, 0, 400, 683, 0, 1);

  /* todo */ CLIP(bullet_2, TCID_BALL_HUGE, 0, 0, 64, 64, 64, 4);

  /* TODO */
}

Li_Game_Init("reg.load.primary-resource", "SDL-init", inst)
{
  auto handle = inst->get_init_handle();

  ::TTF_Init();

  bootstrap::init_append_early_init(handle, "load.primary-resource",
                                    [inst](bootstrap::Bootstrap_Args &,
                                           const bootstrap::Bootstrap_Reg &)
                                    {
                                      load_resource(inst->get_rc_mgr());
                                    });
}

} // namespace Li

