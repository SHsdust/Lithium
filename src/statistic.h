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

#ifndef STATISTIC_H_INCLUDED_KMBXOQQY
#define STATISTIC_H_INCLUDED_KMBXOQQY

#include "config.h"

namespace Li {

struct Player_Resource
{
  uint8_t  power                 = 0;
  uint8_t  power_max             = 128;
  uint8_t  specials              = 0;
  uint8_t  special_pieces        = 0;
  uint8_t  special_pieces_extend = 5;
  uint8_t  specials_max          = 12;
  uint8_t  life                  = 5;
  uint8_t  life_pieces           = 0;
  uint8_t  life_pieces_extend    = 0;
  uint8_t  life_max              = 12;
};

struct Difficulty_Config
{
  uint8_t  initial_ships     = 0;
  uint8_t  initial_specials  = 0;
  uint8_t  difficulty        = 0; // 0 : Easy, 1 : Normal, 2 : Hard, 3 : Lunatic
};

struct Statistic
{
  uint32_t game_point            = 0;
  uint32_t destroyed_enemy_ships = 0;
  uint32_t extended_ships        = 0;
  uint32_t devastated_ships      = 0;
  uint32_t extended_specials     = 0;
  uint32_t used_specials         = 0;
  real_t   pass_time             = 0;
  real_t   pass_time_real        = 0;
  utick_t  total_ticks           = 0;

  attr_must_use_ret inline
  Statistic join(const Statistic &s) const
  {
    return Statistic { game_point            + s.game_point,
                       destroyed_enemy_ships + s.destroyed_enemy_ships,
                       extended_ships        + s.extended_ships,
                       devastated_ships      + s.devastated_ships,
                       extended_specials     + s.extended_specials,
                       used_specials         + s.used_specials,
                       pass_time             + s.pass_time,
                       pass_time_real        + s.pass_time_real,
                       total_ticks           + s.total_ticks
                     };
  }
};


} // namespace Li

#endif // end of include guard: STATISTIC_H_INCLUDED_KMBXOQQY

