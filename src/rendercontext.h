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

#ifndef RENDERCONTEXT_H_INCLUDED_ANRE82CO
#define RENDERCONTEXT_H_INCLUDED_ANRE82CO

#include "gamecfg.h"
#include "SDLdefs.h"

namespace Li {

struct Render_Context
{
  int full_w, full_h;
  int view_x, view_y, view_w, view_h;

  inline
  ::SDL_Point transform(vec2_t pos) const
  {
    auto RW = GAME_SCENE_W,
         RH = GAME_SCENE_H;

    auto VW = view_w,
         VH = view_h;

    return { int(view_x + (RW / 2 + pos.x) / RW * VW)
      ,      int(view_y + (RH / 2 - pos.y) / RH * VH) };
  }

  inline
  ::SDL_Rect transform(vec2_t pos, vec2_t border) const
  {
    auto RW = GAME_SCENE_W,
         RH = GAME_SCENE_H;

    auto VW = view_w,
         VH = view_h;

    int  BW = border.x / RW * VW,
         BH = border.y / RH * VH;

    return { int(view_x + (RW / 2 + pos.x) / RW * VW) - BW / 2
      ,      int(view_y + (RH / 2 - pos.y) / RH * VH) - BH / 2
      ,      BW
      ,      BH };
  }
};


} // namespace Li

#endif // end of include guard: RENDERCONTEXT_H_INCLUDED_ANRE82CO

