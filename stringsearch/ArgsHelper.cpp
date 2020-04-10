#pragma once

#include "ArgsHelper.h"

SearchArgs ArgsHelper::get_args(int argc, char** argv)
{
	SearchArgs m_args;
	try {
		options_description desc{ "Options" };
		desc.add_options()
			("help,h", "Help screen")
			("recurse,r", bool_switch(&m_args.recurse), "Recursive search")
			("path,p", value<std::string>()->default_value("."), "Path to start from")
			("search,s", value<std::string>(),"String to search");

		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);

		if (vm.count("help"))
			std::cout << desc << std::endl;
		if (vm.count("path"))
			m_args.spath = vm["path"].as<std::string>();
		if (vm.count("search"))
			m_args.sstring = vm["search"].as<std::string>();

	}
	catch (const error& ex) {
		std::cerr << ex.what() << '\n';
	}

	return m_args;
}
