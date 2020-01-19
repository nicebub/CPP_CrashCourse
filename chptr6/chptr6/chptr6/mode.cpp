#include <cstdio>
#include <exception>
int mode(int values[], size_t length){
	int last_value=values[0];
	int mode_value=values[0];
    int greatest_count{};
    int current_count{};

	try {
	 	for(auto i{1}; i<length;++i){
			if(last_value == values[i]){
				current_count++;
			}
			else{
				if(current_count> greatest_count){
					greatest_count = current_count;
					mode_value = values[i];
				}
				else if(current_count == greatest_count){
					
				}
				else {
				
				}
				current_count = 0;
			}		
		}
	
	} catch(std::exception e)//, e.what());
	{
		printf("%s\n", e.what());
	}
	return 0;
}

int main(){
	printf("The mode of the specified values is: %d\n", mode({1,2,3,4,5}, 0));
}
