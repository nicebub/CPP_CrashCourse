#include <cstddef>
#include <cstdio>
template <typename Fn, typename In, typename Out>
constexpr Out fold(Fn function, In* input, size_t length, Out initial) {
	if(input && length>0){
		if(length == 1)
			return function(input[0],0);
		else{
			return function(input[initial], fold(function,&input[initial+1],length-1,0));
		}
	}
	return static_cast<Out>(0);
}

template <typename Fn, typename In, typename Out>
constexpr Out foldnorec(Fn function, In* input, size_t length, Out initial) {
	if(input && length>0){
		Out result{};
		for(auto i{initial};i<length;i++){
			result = function(input[i],result);
		}
		return result;
	}
	return static_cast<Out>(0);
}

int main(){
	constexpr long long data[]{ 100LL, 200LL, 300LL, 400LL, 500LL };
	constexpr size_t data_len = 5;
	constexpr auto sum = fold([](auto x, auto y) constexpr {return x + y; }, data, data_len, 0);
	constexpr long long  mult = fold([](auto x, auto y) constexpr -> long long {return y!=0?x*y:x; }, data, data_len, 0);
	constexpr auto max = fold([](auto x, auto y) constexpr {return x>y?x:y; }, data, data_len, 0);
	constexpr auto min = fold([](auto x, auto y) constexpr {return y!=0?x<y?x:y:x; }, data, data_len, 0);
	constexpr auto num200 = fold([](auto x, auto y) constexpr {return x>=200?1+y:y; } , data, data_len, 0);


	printf("Recursive\n");

	printf("Sum: %d\n", sum);
	printf("Multple: %lld\n",mult);
	printf("Max: %d\n",max);
	printf("Min: %d\n",min);
	printf("Amount of numbers 200 and over: %d\n",num200);
	constexpr auto sum2 = foldnorec([](auto x, auto y) constexpr {return x + y; }, data, data_len, 0);
	constexpr long long  mult2 = foldnorec([](auto x, auto y) constexpr -> long long {return y!=0?x*y:x; }, data, data_len, 0);
	constexpr auto max2 = foldnorec([](auto x, auto y) constexpr {return x>y?x:y; }, data, data_len, 0);
	constexpr auto min2 = foldnorec([](auto x, auto y) constexpr {return y!=0?x<y?x:y:x; }, data, data_len, 0);
	constexpr auto num2002 = foldnorec([](auto x, auto y) constexpr {return x>=200?1+y:y; } , data, data_len, 0);
	printf("No recursion\n");
	printf("Sum: %d\n", sum2);
	printf("Multple: %lld\n",mult2);
	printf("Max: %d\n",max2);
	printf("Min: %d\n",min2);
	printf("Amount of numbers 200 and over: %d\n",num2002);


}

