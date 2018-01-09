
/*
 * Created: 09-01-2018
 * Modified: Tue 09 Jan 2018 15:00:12 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include <action-regex.hpp>

int main(int argc, char **argv)
{
  try {
    auto options = parse_program_options(
        argc, argv, std::make_unique<filesystem::Copy_Action>());
    filesystem::find_and_apply(filesystem::boost_fs::path{"."},
                               options.regex_args, *(options.action),
                               options.find_opt);
  } catch (nothing_to_do_exception &) {
    return 0;
  }
}
