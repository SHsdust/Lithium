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

#include "render.h"
#include "support.h"
#include "SDLdefs.h"
#include "rendercontext.h"

namespace Li {


namespace components {

void SDL_Render::render(const Render_Context &rc,
                        const Update_Info &ui)
{
  auto renderer = rc.renderer;

  auto status = ui.target.tag->as<Tag>()->model.status;

  switch (status)
  {
  case Tag::Alive:
    ::SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    break;
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

  auto pos = ui.target.particle->as<Particle>()->get_position();

  auto Wr = ui.world.stage_width;
  auto Hr = ui.world.stage_height;

  int x = (pos.x + Wr / 2) / Wr * rc.view_w + rc.view_x;
  int y = (pos.y + Hr / 2) / Hr * rc.view_h + rc.view_y;

  int w = 4, h = 4;

  ::SDL_Rect obj_rect = { x, y, w, h };
  ::SDL_RenderDrawRect(renderer, &obj_rect);
}

} // namespace components
} // namespace Li

