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

#include "debug.h"

namespace Li {
namespace details {

Str tracef_text_impl(const Str &header,
                     const Str &content)
{
  constexpr int nice_length = 25;

  auto pad_length  = header.length() < nice_length ? nice_length - header.length() : 0;
  auto nice_header = str_pad_right(header, pad_length) + " :";
  auto cont_line   = str_pad_right("", header.length()) + " |";
  auto lines       = str_split(content, "\n");
  bool first_line  = true;

  Str tracef_text  = "";

  for (auto &&line : lines)
  {
    if (first_line)
      tracef_text += nice_header;
    else
      tracef_text += cont_line;

    if (!line.empty())
      tracef_text += " " + line;

    tracef_text += "\n";

    first_line = false;
  }

  return tracef_text;
}


Str shortify_path(const Str &path)
{
  auto last_slash = path.find_last_of("/");
  if (last_slash == path.npos)
    return path;
  else
    return path.substr(last_slash + 1);
}


} // namespace details
} // namespace Li

