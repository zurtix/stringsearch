#pragma once

#ifndef __STRINGSEARCH_H__
#define __STRINGSEARCH_H__

#include "Searcher.h"
#include "SearchArgs.h"
#include <iostream>
#include <stdexcept>

class StringSearch {
private:
	SearchArgs m_args;
private:
	static StringSearch& getImpl(SearchArgs* const args = nullptr) {
		static StringSearch instance{ args };
		return instance;
	}

	StringSearch(SearchArgs* const args) : m_args(args ? std::move(*args) : SearchArgs()) {
		if (&args == nullptr)
			throw std::runtime_error{"Arguments not initialized"};
	}
public:
	static StringSearch& getInstance() {
		return getImpl();
	}
	static void init(SearchArgs args) {
		getImpl(&args);
	}
	void execute();
private:
	StringSearch(StringSearch const&) = delete;
	void operator=(StringSearch const&) = delete;
};


#endif