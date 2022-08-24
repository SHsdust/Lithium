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

#ifndef CHAPTER_H_INCLUDED_WTX4VHDW
#define CHAPTER_H_INCLUDED_WTX4VHDW

#include "gamecfg.h"
#include "core/unit.h"
#include "books/prototype.h"

namespace Li {

class Game_Instance;
class Game_Update;

class Chapter : public Task
{
public:
  enum Chapter_Type { CT_Normal
                    , CT_Mid_Boss
                    , CT_Boss
                    };
private:
  Chapter_Type chapter_type;
public:
  Chapter(Chapter_Type chapter_type = CT_Normal)
    : chapter_type(chapter_type)
  { }
public:
  inline Chapter_Type get_chapter_type() const
  { return chapter_type; }

  virtual void on_enter(Game_Update *, Game_Instance *) = 0;
  virtual void on_leave(Game_Update *, Game_Instance *) = 0;
};

template <class T>
class C_Dummy_On_Enter : public T
{
public:
  using T::T;

  virtual void on_enter(Game_Update *, Game_Instance *) override final
  { }
};

template <class T>
class C_Dummy_On_Leave : public T
{
public:
  using T::T;

  virtual void on_leave(Game_Update *, Game_Instance *) override final
  { }
};

namespace details {
attr_export void on_leave_clear(Game_Update *gu,
                                Game_Instance *gi);
} // namespace details

template <class T>
class C_On_Leave_Clear : public T
{
public:
  using T::T;

  virtual void on_leave(Game_Update *gu, Game_Instance *gi) override final
  { details::on_leave_clear(gu, gi); }
};

} // namespace Li

#endif // end of include guard: CHAPTER_H_INCLUDED_WTX4VHDW

