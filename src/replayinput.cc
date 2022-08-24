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

#include "replayinput.h"
#include "support/misc.h"
#include "support/debug.h"

namespace Li {

Replay_Input_Source::Replay_Input_Source(const Str &rpy_path_prefix)
{
  auto fr_stat_file = rpy_path_prefix + "-fr.rpy";

  std::FILE *pf_fr_stat = std::fopen(fr_stat_file.c_str(), "r");

  Li_Assert(pf_fr_stat);
  attr_unused auto close_1 = $defer_with (pf = pf_fr_stat)
  {
    std::fclose(pf);
  };

#define LINES 1024
  Frame_State buff[LINES];

  for (size_t nread; nread = std::fread(buff, sizeof (*buff), LINES, pf_fr_stat); )
  {
    std::copy(buff, buff + nread, std::back_inserter(fr_stat));
  }

  auto rand_file = rpy_path_prefix + "-rand.rpy";

  std::FILE *pf_rand = std::fopen(rand_file.c_str(), "r");

  Li_Assert(pf_rand);
  attr_unused auto close_2 = $defer_with (pf = pf_rand)
  {
    std::fclose(pf);
  };

  int rand_buff[LINES];

  for (size_t nread; nread = std::fread(rand_buff, sizeof (*rand_buff), LINES, pf_rand); )
  {
    std::copy(rand_buff, rand_buff + nread, std::back_inserter(rand));
  }
}


bool Replay_Input_Source::query_key_state(Key_Code kc) const
{
  return fr_stat.at(fr_idx).key_status[kc] != 0;
}

int  Replay_Input_Source::acquire_random(int from_inclusive,
                                         int to_exclusive)
{
  return rand.at(rand_idx++);
}

void Replay_Input_Source::next()
{
  ++fr_idx;
}

} // namespace Li
