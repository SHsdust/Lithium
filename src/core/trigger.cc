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

#include "trigger.h"
#include "tag.h"
#include "updatectx.h"

namespace Li {

void On_Damage_Capacity_Destroy::on_damage_capacity(const Update_Ctx &uc)
{
  auto *tag = uc.target.tag->as<Tag>();
  tag->model.status = Tag::Dying;
}

void On_Hit_Vanish::on_hit(const Update_Ctx &uc, Unit *)
{
  auto *tag = uc.target.tag->as<Tag>();
  tag->model.status = Tag::Vanishing;
}

void On_Hit_By_Destroy::on_hit_by(const Update_Ctx &uc, Unit *)
{
  auto *tag = uc.target.tag->as<Tag>();
  tag->model.status = Tag::Dying;
}

void On_Hit_By_Vanish::on_hit_by(const Update_Ctx &uc, Unit *)
{
  auto *tag = uc.target.tag->as<Tag>();
  tag->model.status = Tag::Vanishing;
}

} // namespace Li

