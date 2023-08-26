#include <thread>
#include <vector>
#include <lockfree/stack.hpp>

std::vector<std::thread> pool;
lockfree::stack<int> stack;

int main(void)
{
	for (auto i = std::thread::hardware_concurrency(); i > 0; --i)
		pool.emplace_back([](){
			for (int j = 0; j < 1000; ++j)
				stack.push(j);
		});

	for (auto i = std::thread::hardware_concurrency(); i > 0; --i)
		pool.emplace_back([](){
			while (stack.pop() != std::nullopt)
				;
		});

	for (auto i = std::thread::hardware_concurrency(); i > 0; --i)
		pool.emplace_back([](){
			while (stack.peek() != std::nullopt)
				;
		});


	for (auto &thread : pool)
		if (thread.joinable())
			thread.join();
}
