
/*
 * Created: 09-01-2018
 * Modified: Thu 11 Jan 2018 11:52:07 GMT
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef ACTION_REGEX_HPP
#define ACTION_REGEX_HPP

#include <boost/program_options.hpp>
#include <irubataru/filesystem/action.hpp>
#include <irubataru/filesystem/find_file.hpp>

namespace po = boost::program_options;
using namespace irubataru;

struct Action_Regex_Options
{
  boost::filesystem::path search_path;
  std::unique_ptr<filesystem::Action> action;
  filesystem::Find_And_Apply_Options find_opt;
  filesystem::Regex_Args regex_args;
};

class nothing_to_do_exception : public std::exception
{
};

inline po::variables_map init_program_options(int argc, char *argv[],
                                              filesystem::Action *action_ptr)
{
  po::options_description main_opts{"Main options"};

  // clang-format off

  main_opts.add_options()
    ("directory", po::value<std::string>(), "the base directory")
    ("find-regex", po::value<std::string>(), "the file search regex");

  if (action_ptr->is_from_to_action()) {
    main_opts.add_options()
      ("match-regex", po::value<std::string>(),
       "the (optional) match to replace regex")
      ("replace-string", po::value<std::string>(),
       "replacement regex string");
  }

  // clang-format on

  po::positional_options_description pod;
  pod.add("directory", 1);
  pod.add("find-regex", 1);

  if (action_ptr->is_from_to_action()) {
    pod.add("match-regex", 1);
    pod.add("replace-string", 1);
  }

  po::options_description search_opts{"Search options"};

  // clang-format off

  search_opts.add_options()
    ("help,h", "produce this help message")
    ("absolute", "use absolute paths for the regex matches")
    ("dry-run,n", "turns on verbose, doesn't actually change anything")
    ("hidden,a", "search in hidden files")
    ("search,s", "match substrings when searching for files")
    ("verbose,v", "print what is being done");

  // clang-format on

  po::options_description cmd_line_options;
  cmd_line_options.add(main_opts).add(search_opts);

  auto var_map = po::variables_map{};

  po::store(po::command_line_parser(argc, argv)
                .options(cmd_line_options)
                .positional(pod)
                .run(),
            var_map);

  if (var_map.count("help") or !var_map.count("directory") or
      !var_map.count("find-regex") or
      (action_ptr->is_from_to_action() and !var_map.count("match-regex"))) {

    std::cout << "Overview: Regex variant of the " << action_ptr->name()
              << " command.\n\n"
              << "Usage: " << action_ptr->name()
              << "-regex <options> [directory] [find-regex] ";

    if (action_ptr->is_from_to_action()) {
      std::cout << "([match-regex]) [replace-string]";
    }
    std::cout << "\n" << cmd_line_options << std::endl;
    throw nothing_to_do_exception{};
  }

  return var_map;
}

inline Action_Regex_Options
parse_program_options(int argc, char *argv[],
                      std::unique_ptr<filesystem::Action> action_ptr)
{
  auto var_map = init_program_options(argc, argv, action_ptr.get());

  auto prog_opts = Action_Regex_Options{};

  prog_opts.search_path =
      boost::filesystem::path{var_map["directory"].as<std::string>()};

  prog_opts.regex_args.find_regex =
      std::regex{var_map["find-regex"].as<std::string>()};

  if (!var_map.count("replace-string")) {
    prog_opts.regex_args.match_regex = prog_opts.regex_args.find_regex;
    prog_opts.regex_args.replace_str = var_map["match-regex"].as<std::string>();

    if (var_map.count("verbose") or var_map.count("dry-run")) {
      std::cout << "Pattern: \"" << var_map["find-regex"].as<std::string>()
                << "\" "
                << "Replace: \"" << var_map["match-regex"].as<std::string>()
                << "\"\n\n";
    }

  } else {
    prog_opts.regex_args.match_regex =
        std::regex{var_map["match-regex"].as<std::string>()};
    prog_opts.regex_args.replace_str =
        var_map["replace-string"].as<std::string>();

    if (var_map.count("verbose") or var_map.count("dry-run")) {
      std::cout << "Search: \"" << var_map["find-regex"].as<std::string>()
                << "\" "
                << "Pattern: \"" << var_map["match-regex"].as<std::string>()
                << "\" "
                << "Replace: \"" << var_map["replace-string"].as<std::string>()
                << "\"\n\n";
    }
  }

  prog_opts.find_opt.follow_symlinks = false;
  prog_opts.find_opt.use_absolute_path = var_map.count("absolute");
  prog_opts.find_opt.use_hidden_files = var_map.count("hidden");

  if (var_map.count("search"))
    prog_opts.find_opt.match_type = filesystem::Match_Type::Search;
  else
    prog_opts.find_opt.match_type = filesystem::Match_Type::Match;

  if (var_map.count("dry-run")) {
    prog_opts.action = std::make_unique<filesystem::Verbose_Action>(
        std::make_unique<filesystem::NOP_Action>(std::move(action_ptr)));
  } else if (var_map.count("verbose")) {
    prog_opts.action =
        std::make_unique<filesystem::Verbose_Action>(std::move(action_ptr));
  } else {
    prog_opts.action = std::move(action_ptr);
  }

  return prog_opts;
}

#endif /* ACTION_REGEX_HPP */
