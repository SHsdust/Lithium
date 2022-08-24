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

#ifndef HISTORYBOOK_H_INCLUDED_D5RPPHYA
#define HISTORYBOOK_H_INCLUDED_D5RPPHYA

#include "../config.h"

namespace Li {

template <typename T>
class Interval_Timer
{
  T      span;
  T      timer;
public:
  Interval_Timer(T span, T timer = 0)
    : span(span)
    , timer(timer)
  { }

  inline bool set_span(T t)
  {
    span = t;
    return is_timed_out();
  }

  inline bool set(T t)
  {
    timer = t;
    return is_timed_out();
  }

  inline void set_timed_out()
  { timer = span; }

  inline T get_span() const
  { return span; }

  inline bool forward_unless_timed_out(T dt)
  {
    if (is_timed_out())
      return true;

    return forward(dt);
  }

  inline bool forward(T dt)
  {
    if ((timer += dt) > span)
      return true;
    else
      return false;
  }

  inline bool next(T dt)
  {
    if (forward(dt))
    {
      timer = 0;
      return true;
    }

    return false;
  }

  inline bool is_timed_out() const
  { return timer > span; }

  inline bool reset()
  {
    bool to = is_timed_out();
    timer = 0;
    return to;
  }

  inline T get() const
  { return timer; }
};

} // namespace Li

#endif // end of include guard: HISTORYBOOK_H_INCLUDED_D5RPPHYA

