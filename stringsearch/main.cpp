#include "StringSearch.h"
#include "ArgsHelper.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main(int argc, char** argv) {
	ArgsHelper helper;
	SearchArgs args = helper.get_args(argc, argv);

	StringSearch::init(args);
	StringSearch::getInstance().execute();

	_CrtDumpMemoryLeaks();
}