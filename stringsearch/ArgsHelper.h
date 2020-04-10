#pragma once

#ifndef __ARGSHELPER_H__
#define __ARGSHELPER_H__

#include "SearchArgs.h"
#include <iostream>
#include <string>
#include <boost/program_options.hpp>

using namespace boost::program_options;

class ArgsHelper {
public:
	SearchArgs get_args(int, char**);
};

#endif