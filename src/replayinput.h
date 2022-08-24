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

#ifndef REPLAYINPUT_H_INCLUDED_B1WHQKKT
#define REPLAYINPUT_H_INCLUDED_B1WHQKKT

#include "inputsource.h"

namespace Li {


class Replay_Input_Source : public Input_Source
{
  struct Frame_State
  {
    uint8_t key_status[KC_Total];
  };

  Seq<Frame_State> fr_stat;
  Seq<int>         rand;

  size_t rand_idx = 0;
  size_t fr_idx   = 0;

public:
  Replay_Input_Source(const Str &rpy_path_prefix);
public:
  virtual bool query_key_state(Key_Code kc) const override;
  virtual int  acquire_random(int from_inclusive,
                              int to_exclusive) override final;
private:
  virtual void next() override final;
};

} // namespace Li

#endif // end of include guard: REPLAYINPUT_H_INCLUDED_B1WHQKKT

