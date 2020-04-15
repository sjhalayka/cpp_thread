#include <atomic>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <string>
#include <iostream>
#include <chrono>
using namespace std;

void thread_func(atomic_bool& stop, atomic_bool &thread_done, vector<string>& vs, mutex& m)
{
	thread_done = false;

	while (!stop)
	{
		m.lock();
		vs.push_back("test");
		m.unlock();
	}

	thread_done = true;
}



int main(void)
{
	atomic_bool stop = false;
	atomic_bool thread_done = false;

	mutex m;
	vector<string> vs;
	thread t(thread_func, ref(stop), ref(thread_done), ref(vs), ref(m));

	auto start_time = std::chrono::system_clock::now();
	auto end_time = start_time + std::chrono::seconds(2);

	while (!stop)
	{
		auto curr_time = std::chrono::system_clock::now();

		if (curr_time >= end_time)
			stop = true;

		cout << "Printing log:" << endl;
		
		m.lock();

		for (vector<string>::const_iterator ci = vs.begin(); ci != vs.end(); ci++)
			cout << *ci << endl;

		vs.clear();

		cout << endl;

		m.unlock();
	}

	while (!thread_done)
	{
		cout << "Waiting for thread" << endl;
		// draw graphics
	}

	cout << "Joining" << endl;

	t.join();

	cout << "Done" << endl;

	return 0;

}

