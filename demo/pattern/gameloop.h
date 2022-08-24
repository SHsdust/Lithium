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

#ifndef GAMELOOP_H_INCLUDED_TOXNZBFO
#define GAMELOOP_H_INCLUDED_TOXNZBFO

#include "li.h"
#include "SDLdefs.h"

namespace Li {

class Game_Loop
{
  struct Game_Loop_Impl;
  Ptr<Game_Loop_Impl> pimpl;
public:
  Game_Loop(real_t game_stage_w = 1000.0f, real_t game_stage_h = 1000.0f);
  ~Game_Loop();

  void add_unit(Ref_Ptr<Unit> &&unit);

  void update(real_t dt, utick_t ticks);

  void render(::SDL_Renderer *renderer, int view_x, int view_y, int view_w, int view_h);
};

} // namespace Li

#endif // end of include guard: GAMELOOP_H_INCLUDED_TOXNZBFO

