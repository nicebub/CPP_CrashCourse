#include <cstdio>

auto read_from(const char* array,size_t length,size_t which){
	if(which < 1 || which >length){
		printf("character out of bounds of array\n");
		return (char)-1;
	}
	return array[which-1];
}

void write_to(char array[],size_t length, char val, size_t which){
	if(which < 1 || which > length){
		printf("character out of bounds of array\n");
		return;
	}
	array[which-1] = val;
}


int main(){
	char lower[] ="abc?e";
	char upper[] ="ABC?E";
	char* upper_ptr = upper;
	
	lower[3] = 'd';
	upper_ptr[3] = 'D';
	
	char letter_d = lower[3];
	char letter_D = upper_ptr[3];
	
	printf("lower: %s\nupper: %s\n",lower,upper);
	
	auto i = 2lu;

	printf("reading from %lu character of array, value: %c\n",
		i,read_from(lower,sizeof(lower)/sizeof(char),i));

	printf("changing character of array, before: %s\n",upper);

	write_to(upper,sizeof(upper)/sizeof(char),'g',5);

	printf("after: %s\n",upper);
	
//	lower[7] = 'g';
}