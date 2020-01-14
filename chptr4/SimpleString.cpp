#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <utility>
#include <stdexcept>
struct SimpleString {
	SimpleString(size_t max_size)
		: max_size{ max_size }
		,length{}
	{
//		printf("Normal Constructor\n");
		buffer = new char[max_size];
	}
	~SimpleString(){
//		printf("Destructor\n");
		if(buffer){
//			printf("Destructed\n");
			delete[] buffer;
		 }
	}
	SimpleString(const SimpleString& simple)
		: max_size{ simple.max_size }
		, length{ simple.length }
	{
//		printf("Copy Constructor\n");
		buffer = new char[max_size];
		std::strncpy(buffer,simple.buffer,length);
	}
	SimpleString& operator=(const SimpleString& simple){
//		printf("Copy Assignment\n");
		if(this != &simple){
			max_size = simple.max_size;
			length = simple.length;
			if(this->buffer) delete[] buffer;
			buffer = new char[max_size];
			std::strncpy(buffer, simple.buffer,length);
		}
		return *this;
	}
	SimpleString(SimpleString&& simple) noexcept
		: max_size{ simple.max_size }
		, length{ simple.length }
		, buffer{ simple.buffer }
	{
//		printf("Move Constructor\n");
		simple.buffer = nullptr;
		simple.length = 0;
		simple.max_size = 0;
	}
	SimpleString& operator=(SimpleString&& simple) noexcept {
//		printf("Move Assignment\n");
		if(this != &simple){
			max_size = simple.max_size;
			length = simple.length;
			if(this->buffer) delete[] buffer;
			buffer = simple.buffer;
			simple.buffer = nullptr;
			simple.length = 0;
			simple.max_size = 0;
		}
		return *this;
	}
	void print(const char* tag) const{
		printf("%s: %s\n",tag,buffer);
	}
	bool append_line(const char* x){
		if(length+strnlen(x,max_size)+1 > max_size){
//			throw (std::exception,"string larger than max size\n");
			return false;
		}
//		printf("found total length(includes 1 for \\n): %d\n",length+strnlen(x,max_size)+1);
		std::strncat(buffer,x,max_size-length);
		length = strnlen(buffer,max_size);
//		printf("string length added: %d\n",strnlen(buffer,max_size));
		return true;
	}
private:
	size_t max_size;
	char* buffer;
	size_t length;
};

int main(){
	SimpleString string1(20);
	SimpleString string2(30);
	SimpleString string3(string2);
	SimpleString string4(std::move(string3));
	string1.append_line("Hello");
	string1.append_line("thisis fifteen");
	string1.print("tag");
	string1.append_line("1");
	string1.print("other tag");
	string2 = std::move(string1);
	string4 = string2;
	string1.print("");
	string2.print("");
	string3.print("");
	string4.print("");
}