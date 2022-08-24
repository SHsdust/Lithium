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

#ifndef TRIGGER_H_INCLUDED_F4TW6BGX
#define TRIGGER_H_INCLUDED_F4TW6BGX

#include "component.h"

namespace Li {

class Trigger : public Component
{
public:
  virtual void on_damage(const Update_Ctx &uc, real_t dmg) = 0;
  virtual void on_vanish(const Update_Ctx &uc)             = 0;
  virtual void on_destroy(const Update_Ctx &uc)            = 0;
  virtual void on_damage_capacity(const Update_Ctx &uc)    = 0;

  virtual void on_event(const Update_Ctx &uc, upattern_t e) = 0;
  virtual void on_hit(const Update_Ctx &uc, Unit *that)     = 0;
  virtual void on_hit_by(const Update_Ctx &uc, Unit *that)  = 0;
};

template <class C, class T>
class T_On_Damage : public C
{
public:
  using C::C;

  virtual void on_damage(const Update_Ctx &uc, real_t dmg) override final
  { T::on_damage(uc, dmg); }
};

template <class C, class T>
class T_On_Vanish : public C
{
public:
  using C::C;

  virtual void on_vanish(const Update_Ctx &uc) override final
  { T::on_vanish(uc); }
};

template <class C, class T>
class T_On_Desroy : public C
{
public:
  using C::C;

  virtual void on_destroy(const Update_Ctx &uc) override final
  { T::on_destroy(uc); }
};

template <class C, class T>
class T_On_Damage_Capacity : public C
{
public:
  using C::C;

  virtual void on_damage_capacity(const Update_Ctx &uc) override final
  { T::on_damage_capacity(uc); }
};

template <class C, class T>
class T_On_Event : public C
{
public:
  using C::C;

  virtual void on_event(const Update_Ctx &uc, upattern_t e) override final
  { T::on_event(uc, e); }
};

template <class C, class T>
class T_On_Hit : public C
{
public:
  using C::C;

  virtual void on_hit(const Update_Ctx &uc, Unit *that) override final
  { T::on_hit(uc, that); }
};

template <class C, class T>
class T_On_Hit_By : public C
{
public:
  using C::C;

  virtual void on_hit_by(const Update_Ctx &uc, Unit *that) override final
  { T::on_hit_by(uc, that); }
};

struct Do_Nothing
{
  static void on_damage(const Update_Ctx &, real_t)    { }
  static void on_vanish(const Update_Ctx &)            { }
  static void on_destroy(const Update_Ctx &)           { }
  static void on_damage_capacity(const Update_Ctx &)   { }
  static void on_event(const Update_Ctx &, upattern_t) { }
  static void on_hit(const Update_Ctx &, Unit *)       { }
  static void on_hit_by(const Update_Ctx &, Unit *)    { }
};

struct On_Damage_Capacity_Destroy
{ static void on_damage_capacity(const Update_Ctx &); };

struct On_Hit_Vanish
{ static void on_hit(const Update_Ctx &, Unit *that); };

struct On_Hit_By_Destroy
{ static void on_hit_by(const Update_Ctx &, Unit *that); };

struct On_Hit_By_Vanish
{ static void on_hit_by(const Update_Ctx &, Unit *that); };

class Dummy_Trigger : public $chain( Trigger
                                   , (Copy_Via_CCTOR, Dummy_Trigger)
                                   , (Pretty_Runtime_Type, Dummy_Trigger)
                                   , (Default_Pre_Update)
                                   , (Default_Post_Update)
                                   , (T_On_Damage, Do_Nothing)
                                   , (T_On_Vanish, Do_Nothing)
                                   , (T_On_Desroy, Do_Nothing)
                                   , (T_On_Event, Do_Nothing)
                                   , (T_On_Damage_Capacity, Do_Nothing)
                                   , (T_On_Hit, Do_Nothing)
                                   , (T_On_Hit_By, Do_Nothing))
{
public:
  using Super::Super;
};

class Default_Trigger : public $chain( Trigger
                                     , (Copy_Via_CCTOR, Default_Trigger)
                                     , (Pretty_Runtime_Type, Default_Trigger)
                                     , (Default_Pre_Update)
                                     , (Default_Post_Update)
                                     , (T_On_Damage, Do_Nothing)
                                     , (T_On_Vanish, Do_Nothing)
                                     , (T_On_Desroy, Do_Nothing)
                                     , (T_On_Event, Do_Nothing)
                                     , (T_On_Damage_Capacity, Do_Nothing)
                                     , (T_On_Hit, On_Hit_Vanish)
                                     , (T_On_Hit_By, On_Hit_By_Destroy))
{
public:
  using Super::Super;
};

} // namespace Li

#endif // end of include guard: TRIGGER_H_INCLUDED_F4TW6BGX

