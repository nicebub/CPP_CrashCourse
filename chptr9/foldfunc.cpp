#include <cstddef>
#include <cstdio>
template <typename Fn, typename In, typename Out>
constexpr Out fold(Fn function, In* input, size_t length, Out initial){
	if(input && length>0){
		if(length == 1)
			return function(input[0],0);
		else{
			return function(input[initial], fold(function,&input[initial+1],length-1,0));
		}
	}
	return static_cast<Out>(0);
}


int main(){
	int data[]{ 100, 200, 300, 400, 500 };
	size_t data_len = 5;
	auto sum = fold([](auto x, auto y) {return x + y; }, data, data_len, 0);
	auto mult = fold([](auto x, auto y) {return y!=0?x*y:x; }, data, data_len, 0);
	auto max = fold([](auto x, auto y) {return x>y?x:y; }, data, data_len, 0);
	auto min = fold([](auto x, auto y) {return y!=0?x<y?x:y:x; }, data, data_len, 0);
	auto num200 = fold([](auto x, auto y) {return x>=200?1+y:y; } , data, data_len, 0);



	printf("Sum: %d\n", sum);
	printf("Multple: %d\n",mult);
	printf("Max: %d\n",max);
	printf("Min: %d\n",min);
	printf("Amount of numbers 200 and over: %d\n",num200);
}

