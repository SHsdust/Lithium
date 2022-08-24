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

#ifndef PROTOTYPE_H_INCLUDED_NAIQSCDZ
#define PROTOTYPE_H_INCLUDED_NAIQSCDZ

#include "../core/object.h"
#include "../core/updateobject.h"
#include "../core/updatectx.h"

namespace Li {

struct Update_Ctx;

class Task : public Update_Object
{
  bool last_exec_result = true;
public:
  bool exec(const Update_Ctx &uc);
private:
  virtual void do_update(const Update_Ctx &uc) override final;
  virtual bool do_exec(const Update_Ctx &uc) = 0;
};

template <typename Underlying_Type>
class Source : public Update_Object
{
  Underlying_Type value;
public:
  using underlying_type = Underlying_Type;
  virtual ~Source() { }

  inline underlying_type get_value() const
  { return value; }

  inline underlying_type eval(const Update_Ctx &uc)
  {
    update(uc);

    return get_value();
  }
private:
  virtual underlying_type do_eval(const Update_Ctx &uc) = 0;

  virtual void do_update(const Update_Ctx &uc) override final
  { value = do_eval(uc); }
};

using U_Source = Source<utick_t>;
using R_Source = Source<real_t>;
using V_Source = Source<vec2_t>;

using U_Source_Ptr = Ref_Ptr<U_Source>;
using R_Source_Ptr = Ref_Ptr<R_Source>;
using V_Source_Ptr = Ref_Ptr<V_Source>;

using u_underlying_t = U_Source::underlying_type;
using r_underlying_t = R_Source::underlying_type;
using v_underlying_t = V_Source::underlying_type;

} // namespace Li


#endif // end of include guard: PROTOTYPE_H_INCLUDED_NAIQSCDZ

