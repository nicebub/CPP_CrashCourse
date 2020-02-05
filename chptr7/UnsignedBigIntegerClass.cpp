#include <cstddef>
#include <cstdio>
#include <stdexcept>
#include <string>
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
			if(internalArray){
				#ifdef DEBUG
			    printf("deleting internal array with address %p\n",internalArray);
				#endif
			    delete[] internalArray;
				internalArray = nullptr;
			}
			throw std::range_error("cannot make UnsignedBigInteger from signed int");
		}
	}
	~UnsignedBigInteger(){
		if(internalArray){
			 #ifdef DEBUG
			printf("deleting internal array with address %p\n",internalArray);
			 #endif
		    delete[] internalArray;
			internalArray = nullptr;
		}
	}
	operator int() const {
	    if(getLength() > std::numeric_limits<int>::digits10+2)
		   throw std::range_error("narrowing");
		   int value;
	    sscanf(internalArray,"%d",&value);
	    if(value < 0)
		   throw std::range_error("narrowing");
	    return value;
	}
	const size_t getLength(void) const noexcept {
		return internalLength;
	}
	const char getvalueforIndex(const size_t index)const {
		if(index >= getLength() || index <0)
			throw std::range_error("index out of bounds");
		return internalArray[index];
	}
	const int getIntvalueforIndex(const size_t index)const {
		if(index >= getLength() || index <0)
			throw std::range_error("index out of bounds");
		return internalArray[index]-'0';
	}
	void print(void) const noexcept {
		for(auto c{0};c<getLength();c++)
			printf("%c",getvalueforIndex(c));
		printf("\n");
	}
	const UnsignedBigInteger operator+(const int& in) const {
		char temp[std::numeric_limits<int>::digits10+2];
		sprintf(temp,"%d",in);
	    char real[strlen(temp)+1];
	    std::strcpy(real,temp);
		UnsignedBigInteger t{real};
	    return *this+t;
	}
	const UnsignedBigInteger operator+(const UnsignedBigInteger& in) const{
		int rsult{}, carry{};
		size_t curindex{},inindex{},longest{};
		if(getLength() > in.getLength())
			longest = getLength();
		else
			longest = in.getLength();
		char result[longest+1];
		for(curindex=1, inindex=1;
			curindex<=getLength() && inindex<=in.getLength();
			curindex++,inindex++){
			rsult = getIntvalueforIndex(getLength()-curindex)
				 + in.getIntvalueforIndex(in.getLength()-inindex)
				 + carry;
			if(rsult >9){
				carry = 1;
				rsult -= 10;
			}
			else
				carry = 0;
			#ifdef DEBUG
			printf("storing rsult:%d into longest-c:%lu\n",rsult,longest-curindex);
			#endif
			result[longest-curindex] = (rsult+'0');
		}
		if(curindex <= getLength()){
			#ifdef DEBUG
			printf("curindex:%lu <= getLength():%lu\n",curindex,getLength());
			#endif
			for(;curindex<=getLength();curindex++){
				result[longest-curindex] = getvalueforIndex(getLength()-curindex)+carry;
			    carry=0;
				#ifdef DEBUG
				printf("storing rsult:%d into longest-curindex:%lu\n",
					getIntvalueforIndex(getLength()-curindex),longest-curindex+carry);
				#endif
			}
		}
		else if(inindex <=in.getLength()){
			#ifdef DEBUG
			printf("inindex:%lu <= in.getLength():%lu\n",inindex,in.getLength());
			#endif
			for(;inindex<=in.getLength();inindex++){
				result[longest-inindex] = in.getvalueforIndex(in.getLength()-inindex) + carry;
			    carry=0;
				#ifdef DEBUG
				printf("storing rsult:%d into longest-inindex:%lu\n",
					in.getIntvalueforIndex(in.getLength()-inindex),longest-inindex+carry);
				#endif
			}
		}
		if(carry){
			throw std::range_error("oveflow");
		}
		result[longest]='\0';
		#ifdef DEBUG
		printf("%s\n",result);
		#endif
		UnsignedBigInteger r{result};
		return r;
	}
private:
	const size_t internalLength;
	const char *internalArray;
};

int main(){
	char num[] = {"123"};
	char num2[] { "5654372719297472928372823048"};
	printf("Testing char* conststructor\n");
	UnsignedBigInteger a{num2};
	UnsignedBigInteger b{num};

    try{
		printf("Testing exception due to signed integer: ");
    	UnsignedBigInteger c{"-1"};
    }catch(std::range_error &e){
	   printf("range exception: %s\n",e.what());
    }
	
    printf("Testing getLength(): length of a: %ld\n",a.getLength());
	printf("Testing getLength(): length of b: %ld\n",b.getLength());

	printf("Testing print() fuction for a: ");
	a.print();
	printf("Testing print() function for b: ");
	b.print();

	printf("Testing operator+ overload UnsignedBigInteger + UnsignedBigInteger\n");
	printf("Testing Copy Constructor at same time\n");
    UnsignedBigInteger d{a+b};
    d.print();

	try{
		printf("Testing overflow of operator+ with 2 UnsignedBigIntegers\n");
		UnsignedBigInteger e{"877"};
		(e+b).print();
	}catch(std::range_error &e){
		printf("range exception: %s\n",e.what());
	}
	UnsignedBigInteger f{"876"};
	printf("Testing operator+ overload without overflow\n");
	(f+b).print();
	
	b.print();
	printf("Testing operator+ overload of UnsignedBigInteger and int\n");
	(b+27).print();

	f.print();
    printf("Testing operator+ overload UnsignedBigInteger and int: f + 27: ");
    (f+27).print();
    printf("Testing operator int() implicit cast overload of UnsignedBigInteger: 27 + f: %d\n",27+f);
    printf("%d\n",std::numeric_limits<int>::max());
    UnsignedBigInteger g{"2147483647"};
    printf("Testing operator int() cast overload of UnsignedBigInteger: g as an integer: %d\n",(int)g);
    UnsignedBigInteger h{"2147483648"};
    try{
		printf("Testing operator int() explicist cast overload with narrowing\n");
		  int r = (int)h;
		  r++;
    }catch(std::range_error &e){
	   printf("range error: %s\n",e.what());
    }
    try{
		printf("Testing operator int() implicit cast overload with narrowing\n");
		  int s = h;
		  s++;
    }catch(std::range_error &e){
	   printf("range error: %s\n",e.what());
    }
}
