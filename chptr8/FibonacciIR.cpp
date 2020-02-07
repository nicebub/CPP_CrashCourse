#include "FibonacciIR.h"

	bool FibonacciIterator::operator!=(int x) const {
		return x>=current;
	}

	FibonacciIterator& FibonacciIterator::operator++(){
		const auto temp = current;
		current += last;
		last = temp;
		return *this;
	}
	
	int FibonacciIterator::operator*() const {
		return current;
	}

	 FibonacciRange::FibonacciRange(int max) : max{max} {}

	FibonacciIterator FibonacciRange::begin() const {
		return FibonacciIterator{};
	}

	int FibonacciRange::end() const {
		return max;
	}
