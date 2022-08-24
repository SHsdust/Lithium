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

#ifndef OBJECT_H_INCLUDED_LIOUJ5ZM
#define OBJECT_H_INCLUDED_LIOUJ5ZM

#include "../config.h"
#include "../support/inhchain.h"
#include "refptr.h"

namespace Li {

/**
 * game object base.
 */
class Object
{
public:
  /**
   * required.
   */
  virtual ~Object();

  /**
   * prototype.
   */
  virtual Object *clone() const = 0;

  /**
   * for debug
   */
  virtual Str get_runtime_type() const = 0;


  /**
   * shortcut for <code>static_cast<BLAHBLAH *>(foo->query_component(BLAH))</code>.
   *
   * just <code>foo->query_component(BLAH)->as<BLAHBLAH>()</code>
   */
  template <class Target>
  inline Target *as()
  { return static_cast<Target *>(this); }

  /**
   * const version of above.
   */
  template <class Target>
  inline const Target *as() const
  { return static_cast<const Target *>(this); }

  /**
   * dynamic_cast version of above.
   */
  template <class Target>
  inline Target *ensure()
  { return dynamic_cast<Target *>(this); }

  /**
   * const version of above.
   */
  template <class Target>
  inline const Target *ensure() const
  { return dynamic_cast<const Target *>(this); }

  /**
   * config version of above.
   */
  template <class Target>
  inline auto ensure_if_debug()
  {
#ifdef Li_Debug_Compile
    return ensure<Target>();
#else
    return as<Target>();
#endif
  }

  /**
   * const version of above.
   */
  template <class Target>
  inline auto ensure_if_debug() const
  {
#ifdef Li_Debug_Compile
    return ensure<Target>();
#else
    return as<Target>();
#endif
  }

  template <class Target>
  inline auto clone_as() const
  { return clone()->as<Target>(); }
};

template <class B, class D>
class Copy_Via_CCTOR : public B
{
public:
  virtual Object *clone() const override final
  { return new D(static_cast<const D &>(*this)); }
};

namespace details { Str runtime_type_helper(const std::type_info &ti); }

template <class B, class D>
class Pretty_Runtime_Type : public B
{
public:
  virtual Str get_runtime_type() const override final
  { return details::runtime_type_helper(typeid (D)); }
};

} // namespace Li


#endif // end of include guard: OBJECT_H_INCLUDED_LIOUJ5ZM

