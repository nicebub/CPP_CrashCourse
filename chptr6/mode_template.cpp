#include <cstdio>
//#include <cstring>
//#include <exception>
//#include <stdexcept>
#include <vector>
#include <algorithm>
#include <type_traits>

template<typename valuetype>
concept bool Integer() {
	return std::is_integral<valuetype>::value;
};
/* mode returns most found integer in an already sorted
 * int[] array or throws error
 * arguments are array of values and its length inn members
 */
template<Integer valuetype>
valuetype mode(const valuetype *values, const size_t length) {

    valuetype last_value;			/* last value seen throug loop*/
	
    valuetype mode_value{-1}; 		/* mode found if any
						 * defaults to error code*/
		
    size_t greatest_count{0}; 	/* keep track of most seen number
						 * defaults to 0 */
		
    size_t current_count{0}; 	/* keep track current number occurances
						 * defaults: to 0 */
    bool tie = false;
					   /* check to see if tie between modes
					    * if values is NULL throw an exception */
    if(!values || length <=0) return 0;

	std::vector<valuetype> value_vector(values,values+length);
	std::sort(value_vector.begin(),value_vector.end());

    last_value=value_vector[0];
    mode_value=value_vector[0];
					   /* loop through array given
					    * is this the same as the last number
					    * if so then increase occurance counter
					    */
    for(auto i{0}; i<length;++i){
	   if(last_value == value_vector[i]) current_count++;
					   /* otherwise this isn't the current occured
					    * number, check to see if the current number
					    * has been seen the most out of past read
					    * number and change mode to match
					    */
	   else{
		  if(current_count> greatest_count){
			 greatest_count = current_count;
			 mode_value = last_value;
			 tie = false;
		  }
					   /* tie for greatest number of occurences */
		  else if(current_count == greatest_count) tie = true;
					   /* move on but remember last value seen */
		  last_value = value_vector[i];
		  current_count = 1; /* reset occurence counter */
	   }
    }
    if(current_count> greatest_count){
	   greatest_count = current_count;
	   mode_value = last_value;
	   tie = false;
    }
				    /* return the value to be found as the mode*/
    if(tie) return 0;
    return mode_value;
};

int main(){
    int result;
    size_t num_arrays = 5;
    int *array[num_arrays]; /* an array of int[] pointers */
    int ar[] = {1,2,2,4,4,4,4,4,4,5,5,5,5}; /* sorted */
    int ar2[] = {1,2,2}; /* sorted */
    int ar3[] = {98, 98, 75, 75, 67 };
    array[0] = ar;
    array[1] = nullptr;
    array[2] = ar2;
    array[3] = ar2;
    array[4] = ar3;

    size_t lengths[num_arrays]; /* array of the lengths of the arrays above */
    lengths[0] = 13;
    lengths[1] = 0;
    lengths[2] = 3;
    lengths[3] = 0;
    lengths[4] = 5;
    /* number of arrays: sizeof(array)/sizeof(int*)*/
    for(auto i{0};i<num_arrays;++i){
		  if((result = mode(array[i], lengths[i])))
			 printf("array: %d mode: %d\n", i,result);
	   else
		  printf("array: %d none\n",i);
    }
	float fl_array[] = {1.3,1.3,1.4,4.5,4.5,4.5,6.7,6.7,8.9,12.45,12.45,4.5,12.45,21.09};
	float fl_array2[] = {1.3,1.3,1.4,4.5,4.5,4.5,6.7,6.7,8.9,12.45,12.45,12.45,21.09};
	float fresult;
  	if((fresult = mode(fl_array, sizeof(fl_array)/sizeof(float))))
		 printf("float array mode: %f\n",fresult);
	 else
	 	 printf("float array none\n");
   	if((fresult = mode(fl_array2, sizeof(fl_array2)/sizeof(float))))
 		 printf("float array mode: %f\n",fresult);
 	 else
 	 	 printf("float array none\n");
	
}
