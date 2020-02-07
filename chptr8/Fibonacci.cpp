#include <cstdio>
#include "FibonacciIR.h"

int main(){
	for(const auto i : FibonacciRange{ 5000 } ) {
		printf("%d ", i);
	}
	printf("\n");
}