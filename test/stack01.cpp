#include <lockfree/stack.hpp>

int main(void)
{
	lockfree::stack<int> stack;

	for (int i = 0; i < 1; ++i)
		stack.push(i);

	stack.peek();

	while (stack.pop() != std::nullopt)
		;
}
