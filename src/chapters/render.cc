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

#include "config.h"
#include "render.h"
#include "../SDLdefs.h"
#include "../core/tag.h"
#include "../core/particle.h"
#include "../core/direction.h"
#include "../RCmanager.h"
#include "../rendercontext.h"
#include "../support/debug.h"
#include "../support/math.h"

namespace Li {


void SDL_Debug_Render::render(const Render_Context &rc,
                        const Update_Ctx &uc)
{
  auto renderer = get_renderer();

  auto status   = uc.target.tag->as<Tag>()->model.status;
  auto pos      = uc.target.particle->as<Particle>()->get_position();
  auto obj_rect = rc.transform(pos, { 16.0f * 2, 16.0f * 2 });

  switch (status)
  {
  case Tag::Dying:
    ::SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    break;
  case Tag::Vanishing:
    ::SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    break;
  case Tag::Spawning:
    ::SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
    break;
  default:
    ::SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  }

  if (status != Tag::Alive)
    ::SDL_RenderDrawRect(renderer, &obj_rect);

  auto t = get_ticks() % 4;

  // ::SDL_RenderCopy(renderer, texture.texture.get(), &src_rect, &obj_rect);

  auto tex_clip = uc.world.game_instance->get_rc_mgr()->query_tex_clip(TCID_STAR_BIG);

  tex_clip->render_clip(renderer, obj_rect, Clip_Index(t));
}

SDL_Texture_Render::SDL_Texture_Render(Game_Instance *inst,
                                       Tex_Clip_ID tcid,
                                       Clip_Index clip_index,
                                       V_Source_Ptr &&size)
  : SDL_Texture_Render(inst->get_rc_mgr()->query_tex_clip(tcid),
                       clip_index,
                       std::move(size))
{ }

void SDL_Texture_Render::render(const Render_Context &rc, const Update_Ctx &uc)
{
  auto renderer = get_renderer();
  auto status   = uc.target.tag->as<Tag>()->model.status;
  auto rot      = uc.target.direction->as<Direction>()->get_direction(uc);
  auto pos      = uc.target.particle->as<Particle>()->get_position();
  auto box      = size->eval(uc);
  auto mgr      = uc.world.game_instance->get_rc_mgr();
  auto ang      = angle(rot, -M_PI / 2);
  auto obj_rect = rc.transform(pos, box);

  if (status == Tag::Dying)
    return;

  if (status == Tag::Vanishing || status == Tag::Spawning)
  {
    auto *vanishing_tex = mgr->query_tex_clip(TCID_PRE_IMG);

    return vanishing_tex->render_clip(renderer, obj_rect, clip_index);
  }

  if (auto *dstat = uc.target.target->qcomp(Damage_Stat))
  {
    if (dstat->health_status() < 0.3f && uc.ticktack.global_tick % 4 == 0)
      return;
  }

  tex_clip->render_clip(renderer, obj_rect, clip_index, ang);
}



} // namespace Li

