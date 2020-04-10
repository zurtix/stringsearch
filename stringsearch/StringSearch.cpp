#pragma once

#include "StringSearch.h"

void StringSearch::execute() {

	if (this->m_args.recurse)
		std::cout << "Recursive Search: ON ";
	else
		std::cout << "Recursive Search: OFF ";

	std::cout << "Searching for: " << this->m_args.sstring;
	std::cout << " In Directory: " << this->m_args.spath;
	std::cout << std::endl;

	Searcher s(m_args.sstring, m_args.spath, m_args.recurse);
	s.doWork();
}
 