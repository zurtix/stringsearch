#include "Searcher.h"

Searcher::Searcher(std::string const& s, std::string const& p, bool const& r) 
	: done(false), m_sstring(s), m_root(p), m_recurse(r) {
	auto numT = std::thread::hardware_concurrency();
	if (numT == 0)
		numT = 1;

	for (unsigned i = 0; i < numT; ++i)
		threads.push_back(std::thread(&Searcher::doWork, this));
}

void Searcher::addWork(int id, std::string const& w) {
	{
		std::lock_guard<std::mutex> g(workQueueMutex);
		workQueue.push(std::pair<int, std::string>(id, w));
	}

	workQueueCond.notify_one();
}

void Searcher::start() {
	if (fs::is_directory(m_root))
		scan_dir(m_root, m_recurse);
	else if (fs::is_regular_file(m_root))
		scan_file(m_root);

	doWork();
}

void Searcher::doWork() {
	while (!done) {
		std::pair<int, std::string> req;

		{
			std::unique_lock<std::mutex> g(workQueueMutex);
			workQueueCond.wait(g, [&]{
				return !workQueue.empty() || done;
				});

			req = workQueue.front();
			workQueue.pop();
		}

		if (workQueue.empty())
			done = true;

		try {
			if (fs::is_directory(req.second))
				scan_dir(req.second, this->m_recurse);
			else
				scan_file(req.second);
		}
		catch (const std::exception& ex) {
			std::cerr << ex.what() << std::endl;
		}
	}
}

void Searcher::scan_file(std::string const& p) {
	unsigned cur = 1;
	std::ifstream fi;
	std::string line;

	fi.open(p);

	if (fi.is_open())
		while (!fi.eof())
			while (getline(fi, line)) {
				if (line.find(this->m_sstring) != std::string::npos)
					safe_display(p + " : " + std::to_string(cur));
				++cur;
			}
	fi.close();
}

void Searcher::safe_display(std::string const& txt) {
	std::lock_guard<std::mutex> g(printMutex);
	std::cout << txt << std::endl;
}

void Searcher::scan_dir(std::string const& p, bool r) {
	for (const auto& e : fs::directory_iterator(p)) {
			addWork(0, e.path().string());
	}
}

Searcher::~Searcher() {
	done = true;

	for (auto& t : threads)
		if(t.joinable())
			t.join();
}