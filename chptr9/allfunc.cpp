#include <cstddef>
#include <cstdio>
#include <functional>
template <typename In>
struct my_func final {
	my_func(In pred) : predicate_b{pred} {
	}

	bool operator()(const In a) const{
		return a<=predicate_b;
	}
	
private:
	In predicate_b;
};


template <typename Fn, typename In>
constexpr bool all(Fn function, In* input, size_t length){
	if(input && length >0){
		for(auto i{0};i<length;i++){
			if(!function(input[i]))
				return false;
		}
		return true;
	}
	return false;
}


template <typename Fn, typename In>
constexpr bool allrec(Fn function, In* input, size_t length){
	if(input && length >0){
		if(length==1)
			return function(*input);
		else return function(*input) && allrec(function,&input[1],length-1); 
	}
	return false;
}


int main(){
	int data[] { 100, 200, 300, 400, 500 };
	size_t data_len = 5;

	printf("not recursive\n");
	auto all_gt100 = all([](auto x) { return x>100; }, data, data_len);
	if(all_gt100) printf("All elements greater than 100.\n");
	auto all_lte100 = all([](auto x) { return x<=100; }, data, data_len);
	if(all_lte100) printf("All elements less than or equal to 100.\n");
	auto all_gte100 = all([](auto x) { return x>=100; }, data, data_len);
	if(all_gte100) printf("All elements greater than or equal to 100.\n");
	auto all_lte500 = all(my_func(500), data, data_len);
	if(all_lte500) printf("All elements less than or equal to 500.\n");
	my_func blah{600};
	auto all_blah = all(blah, data, data_len);
	if(all_blah) printf("All elements less than or equal to 600.\n");
	my_func blah2{400};
	auto all_blah2 = all(blah, data, data_len);
	if(all_blah2) printf("All elements less than or equal to 600.\n");

	printf("recursive\n");
	auto all_gt100f = allrec([](auto x) { return x>100; }, data, data_len);
	if(all_gt100f) printf("All elements greater than 100.\n");
	auto all_lte100f = allrec([](auto x) { return x<=100; }, data, data_len);
	if(all_lte100f) printf("All elements less than or equal to 100.\n");
	auto all_gte100f = allrec([](auto x) { return x>=100; }, data, data_len);
	if(all_gte100f) printf("All elements greater than or equal to 100.\n");
	auto all_lte500f = allrec(my_func(500), data, data_len);
	if(all_lte500f) printf("All elements less than or equal to 500.\n");
	auto all_blahf = allrec(blah, data, data_len);
	if(all_blahf) printf("All elements less than or equal to 600.\n");
	auto all_blah2f = allrec(blah, data, data_len);
	if(all_blah2f) printf("All elements less than or equal to 600.\n");
	 
}

