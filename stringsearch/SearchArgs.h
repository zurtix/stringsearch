#pragma once


#ifndef __SEARCHARGS_H__
#define __SEARCHARGS_H__

#include <string>

class SearchArgs {
public:
	std::string sstring;
	std::string spath;
	bool recurse = false;
};

#endif