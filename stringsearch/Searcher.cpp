#include "Searcher.h"

Searcher::Searcher(std::string const& s, std::string const& p, bool const& r) 
	: done(false), m_sstring(s), m_root(p), m_recurse(r) {
	auto numT = std::thread::hardware_concurrency();
	if (numT == 0)
		numT = 1;

	for (unsigned i = 0; i < numT; ++i)
		threads.push_back(std::thread(&Searcher::doWork, this));

	if (fs::is_directory(p))
		scan_dir(p, r);
	else if (fs::is_regular_file(p))
		scan_file(p);
}

void Searcher::addWork(int id, std::string const& w) {
	std::lock_guard<std::mutex> g(workQueueMutex);
	workQueue.push(std::pair<int, std::string>(id, w));
	workQueueCond.notify_one();
}

void Searcher::doWork() {
	while (!done) {
		std::pair<int, std::string> req;

		{
			std::unique_lock<std::mutex> g(workQueueMutex);
			workQueueCond.wait(g, [&] {
				return !workQueue.empty() || done;
				});

			req = workQueue.front();
			workQueue.pop();
		}

		try {
			if (fs::is_directory(req.second))
				scan_dir(req.second, this->m_recurse);
			else if (fs::is_regular_file(req.second))
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
					std::cout << p << " : " << cur << std::endl;
				++cur;
			}
}

void Searcher::scan_dir(std::string const& p, bool r) {
	for (const auto& e : fs::directory_iterator(p)) {
			addWork(0, e.path().string());
	}
}

Searcher::~Searcher() {
	done = true;

	for (auto& t : threads)
		if (t.joinable())
			t.join();
}