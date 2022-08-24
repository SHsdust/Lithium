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

#ifndef INPUTSOURCE_H_INCLUDED_BUO1EI2C
#define INPUTSOURCE_H_INCLUDED_BUO1EI2C

#include "gamecfg.h"

namespace Li {

class Input_Source
{
public:
  virtual ~Input_Source();

  virtual bool query_key_state(Key_Code kc) const = 0;

  virtual int  acquire_random(int from_inclusive,
                              int to_exclusive) = 0;


  /**
   * 精确到0.01
   */
  virtual bool try_luck(real_t p)
  {
    if (p > 1.0f)
      p = 1.0f;

    auto t = int(p * 100 * 100);

    return acquire_random(0, 100 * 100) < t;
  }

  friend class Game_Update;
private:
  virtual void next() = 0;
};

} // namespace Li

#endif // end of include guard: INPUTSOURCE_H_INCLUDED_BUO1EI2C

