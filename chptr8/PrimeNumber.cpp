#include <cstdio>
#include "PrimeNumberIR.h"

int main(){
	for(const auto i : PrimeNumberRange{ 10010 } ) {
		printf("%d ", i);
	}
	printf("\n");
}