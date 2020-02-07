#include <cstdio>
#include "FibonacciIR.h"
#include "PrimeNumberIR.h"

int main(){
	for(const auto i : FibonacciRange{ 5000 } ) {
		printf("%d ", i);
	}
	printf("\n");
	for(const auto i : PrimeNumberRange{ 5000 } ) {
		printf("%d ", i);
	}
	printf("\n");
}