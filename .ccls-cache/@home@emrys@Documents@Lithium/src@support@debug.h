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

#ifndef DEBUG_H_INCLUDED_TXLJIZBE
#define DEBUG_H_INCLUDED_TXLJIZBE

#include "format.h"
#include "string.h"

namespace Li {

struct Src_Location
{
  unsigned    line;
  const char *file;
  const char *func;
  const char *pretty_func;

  inline Str dump() const
  {
    using namespace std::string_literals;

    return "[ File: "s + file
      +    ", Line: "s + std::to_string(line)
      +    ", Func: "s + func
      +    " ]";
  }
};

namespace details {

static inline
Str operator <<(const Formatter &,
                const Src_Location &sl)
{
  using namespace std::string_literals;

  return "[ "s + sl.file
    +    ", "s + std::to_string(sl.line)
    +    ", "s + sl.func
    +    " ]"s;
}

} // namespace details
} // namespace Li

#define Li_Src_Location           \
  Li_Prefix Src_Location {        \
    __LINE__, __FILE__, __func__, \
    __PRETTY_FUNCTION__  }

namespace Li {
namespace details {

Str tracef_text_impl(const Str &, const Str &);

template <typename ...Args>
static inline
Str tracef_text(const Str  &header,
                const Args &...args)
{ return tracef_text_impl(header, format(args...)); }

template <typename ...Args>
static inline
Str tracef(const Str &header, const Args &...fmt_and_args)
{
  auto text = tracef_text(header, fmt_and_args...);
  std::fprintf(stderr, "%s", text.c_str());
  return text;
}

Str shortify_path(const Str &path);

} // namespace details
} // namespace Li

#define Li_TraceF_Default_Header(L)       Li_Prefix \
  format("[%s] %-15s %3d", L, Li_Impl_Prefix shortify_path(__FILE__), __LINE__)
#define Li_TraceF_With_Level(L, ...)      Li_Impl_Prefix tracef(Li_TraceF_Default_Header(L), __VA_ARGS__)
#define Li_TraceF_Text_With_Level(L, ...) Li_Impl_Prefix tracef_text(Li_TraceF_Default_Header(L), __VA_ARGS__)

#define Li_TraceF(...) Li_TraceF_With_Level("I", __VA_ARGS__)
#define Li_ErrorF(...) Li_TraceF_With_Level("E", __VA_ARGS__)
#define Li_WarnF(...)  Li_TraceF_With_Level("W", __VA_ARGS__)

#define Li_TraceF_Text(...) Li_TraceF_Text_With_Level("I", __VA_ARGS__)
#define Li_ErrorF_Text(...) Li_TraceF_Text_With_Level("E", __VA_ARGS__)
#define Li_WarnF_Text(...)  Li_TraceF_Text_With_Level("W", __VA_ARGS__)

#define Li_PanicF(...) throw std::runtime_error("Li_PanicF called!\n" + Li_TraceF_Text_With_Level("Panic", __VA_ARGS__))

#define $Li_Assertion_Failed_Fmt(...)                      \
  "Assertion failed: [%s]", #__VA_ARGS__

#define Li_Runtime_Assert(...) do {                        \
    if ((__VA_ARGS__)) { }                                 \
    else Li_PanicF($Li_Assertion_Failed_Fmt(__VA_ARGS__)); \
  } while (false)

#define Li_Runtime_Ensure(...) do {                        \
    if ((__VA_ARGS__)) { }                                 \
    else Li_ErrorF($Li_Assertion_Failed_Fmt(__VA_ARGS__)); \
  } while (false)

#ifndef NDEBUG
#define Li_LogI(...) Li_TraceF(__VA_ARGS__)
#define Li_LogE(...) Li_ErrorF(__VA_ARGS__)
#define Li_LogW(...) Li_WarnF(__VA_ARGS__)
#define Li_Assert(...) Li_Runtime_Assert(__VA_ARGS__)
#define Li_Ensure(...) Li_Runtime_Ensure(__VA_ARGS__)
#else
#define Li_LogI(...)   do { } while (false)
#define Li_LogE(...)   do { } while (false)
#define Li_LogW(...)   do { } while (false)
#define Li_Assert(...) do { } while (false)
#define Li_Ensure(...) do { } while (false)
#endif

#endif // end of include guard: DEBUG_H_INCLUDED_TXLJIZBE


