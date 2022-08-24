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

#include "texclip.h"

namespace Li {

void Tex_Clip::render_clip(::SDL_Renderer *renderer,
                           ::SDL_Rect      dst,
                           Clip_Index      ci) const
{
  ::SDL_RenderCopy(renderer, texture.texture.get(), &clips.at(ci), &dst);
}

void Tex_Clip::render_clip(::SDL_Renderer *renderer,
                           ::SDL_Rect      dst,
                           Clip_Index      ci,
                           real_t          rot) const
{
  ::SDL_RenderCopyEx(renderer,
                     texture.texture.get(),
                     &clips.at(ci),
                     &dst,
                     -rot / M_PI * 180,
                     nullptr,
                     SDL_FLIP_NONE);
}

} // namespace Li

