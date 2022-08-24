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

#ifndef SDLINPUT_H_INCLUDED_2JG5GMIB
#define SDLINPUT_H_INCLUDED_2JG5GMIB

#include "inputsource.h"

namespace Li {

class SDL_Input_Source : public Input_Source
{
public:
  // TODO:  2016-07-27 17:50:34
   SDL_Input_Source() { }
  ~SDL_Input_Source() { }

  virtual bool query_key_state(Key_Code kc) const override final;
  virtual int  acquire_random(int from_inclusive,
                              int to_exclusive) override final;
private:
  virtual void next() override final;
};

} // namespace Li

#endif // end of include guard: SDLINPUT_H_INCLUDED_2JG5GMIB

