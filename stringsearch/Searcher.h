#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <queue>
#include <condition_variable> 
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class Searcher {

private:
	bool done;
	std::vector<std::thread> threads;
	std::condition_variable_any workQueueCond;
	std::mutex workQueueMutex;
	std::queue<std::pair<int, std::string>> workQueue;
	std::string m_sstring, m_root;
	bool m_recurse;
public:
	Searcher(std::string const&, std::string const&, bool const&);
	void addWork(int, std::string const&);
	void doWork();
private:
	void scan_dir(std::string const&, bool r);
	void scan_file(std::string const&);
public:
	~Searcher();
};

#endif // !__THREADPOOL_H__
