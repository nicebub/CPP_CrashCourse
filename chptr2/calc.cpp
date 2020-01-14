#include <cstdio>

enum class Operation {
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE
};

struct Calculator {
	Calculator(Operation in_op){
		op = in_op;
	}
	Calculator(){
		op = Operation::ADD;
	}
//	~Calculator(){}
	Operation get_op(){
		return op;
	}
	void set_op(Operation in_op){
		op = in_op;
	}
	int calculate(int a, int b){
		switch(op){
			case Operation::ADD: return a+b;
			case Operation::SUBTRACT: return a-b;
			case Operation::MULTIPLY: return a*b;
			case Operation::DIVIDE: {
				if(b==0){
					printf("Cannot Divide by Zero\n");
					return -1;
				}
				return a/b;
			}
		}
	}
private:
	Operation op;
};

int main(){
	Calculator my_calc(Operation::ADD);
	int a{ 25 };
	int b{ 37 };
	printf("Adding numbers %d and %d equals: %d\n",a,b,my_calc.calculate(a,b));
	my_calc.set_op(Operation::SUBTRACT);
	printf("Subtracting numbers %d from %d equals: %d\n",b,a,my_calc.calculate(b,a));
	my_calc.set_op(Operation::MULTIPLY);
	printf("Multiplying numbers %d and %d equals: %d\n",b,a,my_calc.calculate(b,a));
	my_calc.set_op(Operation::DIVIDE);
	printf("Dividing numbers %d from %d equals: %d\n",b,a,my_calc.calculate(b,a));
	printf("Dividing numbers %d from %d equals: %d\n",a,b,my_calc.calculate(a,b));
	b = 0;
	printf("Dividing numbers %d from %d equals: %d\n",b,a,my_calc.calculate(b,a));
	printf("Dividing numbers %d from %d equals: %d\n",a,b,my_calc.calculate(a,b));
	
}