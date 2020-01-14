#include <cstdio>
int sum(int x, int y){
	return x+y;
}

int main(){
	int my_number = -10;
	int my_number2 = 10;
	int my_number3 = 255;
	printf("The sum of %d and %d is %d\n", my_number, my_number2, sum(my_number,my_number2));
	printf("The sum of %d and %d is %d\n", my_number, my_number3, sum(my_number,my_number3));
	printf("The sum of %d and %d is %d\n", my_number2, my_number3, sum(my_number2,my_number3));

}