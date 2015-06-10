#include <iostream>

#include <stack>

template<typename T> class MinTree{

	MinTree(void){};
	virtual ~MinTree(void){};

	T& top(void);
	const T& top(void) const;

	void push(const T& value);
	void pop(void);

	const T& min(void) const;

	bool empty() const;
	size_t size() const ;

private:
	std::stack<T> datastack;
	std::stack<T> minstack;

};


