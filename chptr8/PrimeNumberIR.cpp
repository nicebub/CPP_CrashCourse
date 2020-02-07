#include "PrimeNumberIR.h"
#include <cstdio>
	bool PrimeNumberIterator::operator!=(int x) const {
		return x>=current;
	}

	PrimeNumberIterator& PrimeNumberIterator::operator++(){
		while(1){
			current++;
			int i{2};
			for(;i<=current/2;i++){
				if(current % i == 0)
					break;
			}
			if(i==current/2+1)
				return *this;
		}
	}
	
	int PrimeNumberIterator::operator*() const {
		return current;
	}

	 PrimeNumberRange::PrimeNumberRange(int max) : max{max} {}

	PrimeNumberIterator PrimeNumberRange::begin() const {
		return PrimeNumberIterator{};
	}

	int PrimeNumberRange::end() const {
		return max;
	}
