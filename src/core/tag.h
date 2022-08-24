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

#ifndef TAG_H_INCLUDED_RTHASNVB
#define TAG_H_INCLUDED_RTHASNVB

#include "component.h"
#include "../books/prototype.h"

namespace Li {

class Tag : public $chain( Component
                         , (Copy_Via_CCTOR, Tag)
                         , (Pretty_Runtime_Type, Tag)
                         , (Default_Pre_Update)
                         , (Default_Post_Update))
{
public:
  enum Status { Alive
              , Spawning
              , Dying
              , Vanishing
              , Dead
              , Vanished
              , Phantom
              };

public:
  constexpr static real_t OFF_SCREEN_COLLECT_SPAN_DEFAULT = 5.0f;
  constexpr static real_t COLLECT_SPAN_DEFAULT  = 20.0f;

  struct Model
  {
    Status      status = Spawning;

    real_t      spawning_duration  = 0.5f;
    real_t      dying_duration     = 0.5f;
    real_t      vanishing_duration = 0.5f;

    bool   off_screen_auto_collect = true;
    real_t off_screen_collect_span = OFF_SCREEN_COLLECT_SPAN_DEFAULT;

    bool   auto_collect = false;
    real_t collect_span = COLLECT_SPAN_DEFAULT;
  };

  Model model;

  struct Model_Status
  {
    real_t off_screen_collect_timer = 0;
    real_t collect_timer            = 0;
    real_t spawning_timer           = 0;
    real_t dying_timer              = 0;
    real_t vanishing_timer          = 0;
  };

  Model_Status model_status;
public:
  Tag(const Tag &tag)
    : model(tag.model)
    , model_status(tag.model_status)
  { }

  Tag(const Model &model,
      const Model_Status model_status)
    : model(model)
    , model_status(model_status)
  { }
private:
  virtual void do_update(const Update_Ctx &ui) override final;
};

attr_export Ref_Ptr<Task> c_destory();
attr_export Ref_Ptr<Task> c_vanish();

} // namespace Li

#endif // end of include guard: TAG_H_INCLUDED_RTHASNVB

