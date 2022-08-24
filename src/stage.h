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

#ifndef STAGE_H_INCLUDED_MHZUPPE0
#define STAGE_H_INCLUDED_MHZUPPE0

#include "gamecfg.h"
#include "core/unit.h"
#include "books/historybook.h"

namespace Li {

class Game_Instance;
class Game_Update;
class Chapter;

struct Render_Context;

class Stage : public $chain( Unit
                           , (Pretty_Runtime_Type, Stage)
                           , (Copy_Via_CCTOR, Stage))
{
  Str                      prefix;     ///< 依据此前缀从工厂中加载chapters
  size_t                   idx;        ///< 当前chapter
  Game_Instance           *game_instance;

  Seq<Ref_Wrapper<Chapter>> chapters;   ///< 加载了的chapters

  int                      state;
  Interval_Timer<utick_t>  sleep;

  int                      bg_selected = 0;
  int                      bg_total    = 12;
  int                      bg_yoff     = 0;

  uid_t                    bg_first_id;
public:
  Stage(const Str &prefix, Game_Instance *);
  ~Stage();

  inline bool finished() const { return state == 'F'; }
  virtual void render_stage(const Update_Ctx &uc, const Render_Context &rc);
private:
  virtual void do_update(const Update_Ctx &uc) override /* non-final */;
};

} // namespace Li

#endif // end of include guard: STAGE_H_INCLUDED_MHZUPPE0

