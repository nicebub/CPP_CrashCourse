#include <cstddef>
#include <cstdio>
#include <stdexcept>
#include <type_traits>
#include <functional>
#include <cstring>
#include <limits>
#undef DEBUG

struct UnsignedBigInteger {

	UnsignedBigInteger() :
	internalLength{},
	internalArray{nullptr} {}

	UnsignedBigInteger(const UnsignedBigInteger& in) :
	internalLength{in.internalLength},
	internalArray{std::strcpy(new char[internalLength+1],in.internalArray)}{}

	UnsignedBigInteger( const char *in) :
	internalLength{ std::strlen(in) },
	internalArray{ std::strcpy(new char[internalLength+1],in)}
	{
		if(getvalueforIndex(0) == '-'){
			cleanup();
			throw std::range_error("cannot make UnsignedBigInteger from signed int");
		}
	}
	UnsignedBigInteger& operator=(const UnsignedBigInteger& in){
		if(this != &in){
			if(this->internalArray)
				delete[] internalArray;
			internalLength = in.getLength();
			this->internalArray = std::strcpy(new char[internalLength+1],in.internalArray);
		}
		return *this;
	}
	const void cleanup(void)  noexcept{
		if(internalArray){
			#ifdef DEBUG
			printf("deleting internal array with address %p\n",internalArray);
			#endif
			delete[] internalArray;
			internalArray = nullptr;
		}
	}
	~UnsignedBigInteger(){
		cleanup();
	}

	const char* tostring(void) const noexcept {
		return internalArray;
	}

	template<typename T>
	operator T() const {
		static_assert(std::is_integral<T>::value,"can only cast to integral type");

		if((getLength() > std::numeric_limits<T>::digits10+2))
 		   throw std::overflow_error("narrowing");
	    T value{0};
	    long long temp{0};
	    for(size_t iptr{0};iptr<getLength();iptr++){
		   temp *= 10;
		   temp += getIntvalueforIndex(iptr);
	    }
	   #ifdef DEBUG
	    printf("found value: %lld",temp);
	   #endif
	    value = (T)temp;
	    if(value != temp)
		   throw std::overflow_error("narrowing");
	    return value;
	}
	const size_t getLength(void) const noexcept {
		return internalLength;
	}
	const void checkIndexIsValid(const size_t index) const{
		if(index >= getLength() || index <0)
			throw std::range_error("index out of bounds");
	}
	const char getvalueforIndex(const size_t index)const noexcept{
		checkIndexIsValid(index);
		return internalArray[index];
	}
	const int getIntvalueforIndex(const size_t index)const noexcept {
		checkIndexIsValid(index);
		return internalArray[index]-'0';
	}
	const void print(void) const noexcept {
		for(auto c{0};c<getLength();c++)
			printf("%c",getvalueforIndex(c));
		printf("\n");
	}
    static const short add2charswithCarry(const short one, const short two, short& carry)  noexcept{
	   int r = one + two + carry;
	   if(r >9){
		   carry = 1;
		   r -= 10;
	   }
	   else
		   carry = 0;
	   return r;
    }
    static const short sub2charswithCarry(const short one, const short two, short& carry)  noexcept {
	   int r = one - two - carry;
	   if(r <0){
		   carry = 1;
		   r += 10;
	   }
	   else
		   carry = 0;
	   return r;
    }
/*
    static const short mul2charswithCarry(const short one, const short two, short& carry)  noexcept {
    	return 0;
    }
    static const short div2charswithCarry(const short one, const short two, short& carry)  noexcept {
    	return 0;
    }
    static const short mod2charswithCarry(const short one, const short two, short& carry)  noexcept {
    	return 0;
    }
*/
    void getresultforFunction(const UnsignedBigInteger &in,
	 	char (*result),const size_t longest,
		const std::function<const short(const short,const short, short&)> func) const{

	   short rsult{}, carry{};
	   size_t curindex{},inindex{};

	   for(curindex=1, inindex=1;curindex<=getLength() && inindex<=in.getLength();
		   curindex++,inindex++){

		  rsult = func(getIntvalueforIndex(getLength()-curindex),
			in.getIntvalueforIndex(in.getLength()-inindex),carry);

		   result[longest-curindex] = (rsult+'0');
	   }

	   if(curindex <= getLength()){
		   #ifdef DEBUG
		   printf("curindex:%lu <= getLength():%lu\n",curindex,getLength());
		   #endif
		   for(;curindex<=getLength();curindex++){
			   result[longest-curindex] = func(getIntvalueforIndex(getLength()-curindex),0,carry)+'0';
			  carry=0;
			   #ifdef DEBUG
			   printf("storing rsult:%d into longest-curindex:%lu\n",
				   getIntvalueforIndex(getLength()-curindex)+carry,longest-curindex);
			   #endif
		   }
	   }

	   else if(inindex <=in.getLength()){
		   #ifdef DEBUG
		   printf("inindex:%lu <= in.getLength():%lu\n",inindex,in.getLength());
		   #endif
		   for(;inindex<=in.getLength();inindex++){
			   result[longest-inindex] = func(in.getIntvalueforIndex(in.getLength()-inindex),0,carry)+'0';
			  carry=0;
			   #ifdef DEBUG
			   printf("storing rsult:%d into longest-inindex:%lu\n",
				   in.getIntvalueforIndex(in.getLength()-inindex)+carry,longest-inindex);
			   #endif
		   }
	   }

	   if(carry){
		   throw std::overflow_error("oveflow");
	   }
	   
	   result[longest]='\0';
	   #ifdef DEBUG
	   printf("%s\n",result);
	   #endif
    }

	const UnsignedBigInteger op_UBIhelper(const UnsignedBigInteger &in,
			const std::function<const short(const short,const short, short&)> func) const {
//				int carry{};
				size_t longest{};

				if(getLength() > in.getLength())
					longest = getLength();
				else
					longest = in.getLength();
				char result[longest+1];
			   #ifdef DEBUG
			    printf("getLength():%lu in.getLength():%lu longest: %lu\n",getLength(),in.getLength(),longest);
			   #endif
			     getresultforFunction(in,result,longest,func);
				char *nresult{};
				nresult = result;
	    while(*nresult == '0')
			nresult++;
	    return UnsignedBigInteger(nresult);
	}

	template<typename T>
    const void convertTypetoArray(const T& in,char* result) const {
	   long long templong{in};
	   short remainder{0};
	   char *iptr{};
	   char temp[std::numeric_limits<T>::digits10+2];
	   temp[std::numeric_limits<T>::digits10+1]='\0';
	   iptr = &temp[std::numeric_limits<T>::digits10];
	   while(templong != 0){
		  remainder = templong % 10;
		  *iptr = remainder + '0';
		  templong /= 10;
		  iptr--;
	   }
	   iptr++;
	   std::strcpy(result,iptr);
    }

    template<typename T>
    const UnsignedBigInteger op_INThelper(const T& in) const {
		static_assert(std::is_integral<T>::value,"can only cast to integral type");
	   char real[std::numeric_limits<T>::digits10+2];
	   convertTypetoArray(in,real);
	   return UnsignedBigInteger(real);
/*	   UnsignedBigInteger t{real};
	   return t;
	   */
    }
	template<typename T>
	const UnsignedBigInteger operator+(const T& in) const {
	    return *this + op_INThelper(in);
	}
	template<typename T>
    const UnsignedBigInteger operator-(const T& in) const  {
	   return *this - op_INThelper(in);
    }
    const UnsignedBigInteger operator-(const UnsignedBigInteger& in) const {
		return op_UBIhelper(in,sub2charswithCarry);
    }
	const UnsignedBigInteger operator+(const UnsignedBigInteger& in) const {
		return op_UBIhelper(in,add2charswithCarry);
	}
/*
	const UnsignedBigInteger operator*(const UnsignedBigInteger& in) const {
		return op_UBIhelper(in,mul2charswithCarry);
	}
	const UnsignedBigInteger operator/(const UnsignedBigInteger& in) const {
		return op_UBIhelper(in,div2charswithCarry);
	}
	const UnsignedBigInteger operator%(const UnsignedBigInteger& in) const {
		return op_UBIhelper(in,mod2charswithCarry);
	}
*/

private:
	size_t internalLength;
	const char *internalArray;
};

int main(){
	try{
		printf("Testing exception due to signed integer: ");
		UnsignedBigInteger c{"-1"};
	}catch(std::range_error &e){
		printf("range exception: %s: %d\n",e.what(),-1);
	}

	char num[] {"123"};
	char num2[] { "2147483648"};
	printf("Testing char* conststructor\n");
	UnsignedBigInteger a{num2};
	UnsignedBigInteger b{num};
	a.print();
	b.print();

	printf("Testing Copy Constructor: %s\n",a.tostring());
	UnsignedBigInteger d{a};
	printf("New UnsignedBigInteger value: %s\n",d.tostring());

	printf("Testing getLength(): length of a: %ld\n",a.getLength());
	printf("Testing getLength(): length of b: %ld\n",b.getLength());

	printf("Testing print() fuction for a: ");
	a.print();
	printf("Testing print() function for b: ");
	b.print();
	UnsignedBigInteger acopy{};
	acopy = a;
	printf("Testing copy assignment operator\n");
	printf("a:%s should equal acopy: %s\n",a.tostring(),acopy.tostring());

	printf("Testing operator+ overload UnsignedBigInteger: %s + UnsignedBigInteger:%s = %s\n",

	a.tostring(),b.tostring(),(a+b).tostring());

	try{
		UnsignedBigInteger e{"877"};
		printf("Testing overflow of operator+ with 2 UnsignedBigIntegers: %s + %s\n",
			e.tostring(),b.tostring());
		(e+b).print();
	}catch(std::overflow_error &e){
		printf("overflow exception: %s\n",e.what());
	}
	UnsignedBigInteger f{"876"};
	printf("Testing operator+ overload without overflow %s + %s = %s\n",
		f.tostring(),b.tostring(),(f+b).tostring());
	
	printf("Testing operator+ overload of UnsignedBigInteger:%s + int:%d = %s\n",
		b.tostring(),27,(b+static_cast<int>(27)).tostring());

	printf("Testing operator+ overload UnsignedBigInteger:%s + int:%d = %s\n",f.tostring(),27,(f+27).tostring());

	printf("Testing operator int() implicit cast overload of UnsignedBigInteger: %d + %s: %d\n",
		 27,f.tostring(),27+static_cast<int>(f));

	printf("%d\n",std::numeric_limits<int>::max());
	UnsignedBigInteger g{"2147483647"};
	printf("Testing operator int() cast overload of UnsignedBigInteger:%s  as an integer: %d\n",
		 g.tostring(),static_cast<int>(g));

	UnsignedBigInteger h{"2147483648"};
	try{
		printf("Testing operator int() explicit cast overload with narrowing: %s\n",h.tostring());
		  int r = static_cast<int>(h);
		  r++;
	}catch(std::overflow_error &e){
		printf("overflow error: %s: %s\n",e.what(),h.tostring());
	}
	try{
		printf("Testing operator int() implicit cast overload with narrowing: %s\n",h.tostring());
		  int s = h;
		  s++;
	}catch(std::overflow_error &e){
		printf("overflow error: %s: %s\n",e.what(),h.tostring());
	}
	
	UnsignedBigInteger s1{"450"};
	UnsignedBigInteger s2{"400"};
	printf("450 - 400 = %s\n",(s1-s2).tostring());
	try{
	printf("400 - 450 = %s\n",(s2-s1).tostring());
	}
	catch(std::overflow_error &e){
		printf("overflow error: %s: %s - %s\n",e.what(),s2.tostring(),s1.tostring());
	}
	printf("Testing add to long: 2147483648L:%ld\n",2147483648L);
	(s1+2147483648L).print();
	printf("Testing add to implicit long cast (long)%ld\n",static_cast<long>(45));
	(s2+static_cast<long>(45)).print();
	printf("Testing add to implicit short cast (short)%d\n",static_cast<short>(5));
	(s1+ static_cast<short>(5)).print();
}
