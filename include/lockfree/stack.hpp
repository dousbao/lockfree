#ifndef __LOCKFREE_STACK_HPP__
#define __LOCKFREE_STACK_HPP__

#include <optional>
#include <exception>
#include <cxxhazard/hazard.hpp>

namespace lockfree {

template <typename T>
class stack : private cxxhazard::enable_hazard_from_this {
public:
	using value_type = T;
	struct node {
		node *_next;
		value_type *_data;
	};

public:
	stack(void) : _top(nullptr) {}

	~stack(void) noexcept
	{
		auto top = _top.load();

		while (top != nullptr) {
			auto next = top->_next;

			delete top->_data;
			delete top;

			top = next;
		}
	}

	stack(const stack &) = delete;
	stack(stack &&) = delete;

	stack &operator=(const stack &) = delete;
	stack &operator=(stack &&) = delete;

public:
	void push(value_type data)
	{
		auto new_node = new node;

		try {
			new_node->_data = new value_type(std::move(data));
			new_node->_next = _top.load();

			while (!_top.compare_exchange_weak(new_node->_next, new_node))
				;
		} catch (...) { delete new_node; throw std::current_exception(); }
	}

	auto pop(void) -> std::optional<value_type>
	{
		auto hazard = make_hazard();
		node *old_top = _top.load();

		do {
			if (old_top = hazard.protect(_top); old_top == nullptr)
				return std::nullopt;
		} while (!_top.compare_exchange_strong(old_top, old_top->_next));
		hazard.unprotect();

		auto res = *old_top->_data;

		retire(old_top, [old_top]() noexcept {
			delete old_top->_data;
			delete old_top;
		});

		return std::make_optional(res);
	}

	auto peek(void) -> std::optional<value_type>
	{
		auto hazard = make_hazard();

		if (auto top = hazard.protect(_top); top == nullptr)
			return std::nullopt;
		else
			return std::make_optional(*top->_data);
	}

private:
	std::atomic<node *> _top;
};

} // namespace lockfree

#endif // __LOCKFREE_STACK_HPP__
