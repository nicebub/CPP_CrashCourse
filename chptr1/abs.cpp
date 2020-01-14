#include <cstdio>
int absolute_value(int x){
	return x>0 ? x : -x;
}

int main(){
	int my_number = -10;
	int my_number2 = 10;
	printf("The absolute value of %d is %d\n", my_number, absolute_value(my_number));
	printf("The absolute value of %d is %d\n", my_number2, absolute_value(my_number2));
}