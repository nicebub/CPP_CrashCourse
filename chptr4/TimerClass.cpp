#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <utility>
#include <sys/time.h>
struct TimerClass {
	TimerClass(const char* name)
	{
		const char* name_ptr = "default_name";
		#ifdef DEBUG
			printf("Constructor\n");
		#endif
		if(name != nullptr)
			name_ptr = name;
		this->name = new char[strlen(name_ptr)+1];
		strncpy(this->name,name_ptr,strlen(name_ptr)+1);
		gettimeofday(&timestamp,NULL);
		#ifdef DEBUG
			printf("%s: timestamp %ld %d\n",this->name,timestamp.tv_sec,timestamp.tv_usec);
		#endif
	}
	~TimerClass()
	{
		#ifdef DEBUG
			printf("Destructor\n");
		#endif
		struct timeval cur;
		gettimeofday(&cur,NULL);
		if(timestamp.tv_sec != 0 && timestamp.tv_usec !=0 && name!=nullptr){
			printf("%s: %ld seconds %d microseconds\n"
				, name
				,cur.tv_sec - timestamp.tv_sec
					,abs(cur.tv_usec - timestamp.tv_usec));
			delete[] name;
		}
	}
	TimerClass(const TimerClass& in) 
		: timestamp{in.timestamp}
		  ,name{} 
	{
		#ifdef DEBUG
			printf("Copy Constructor\n");
		#endif
		if(name != nullptr) delete[] name;
		name = new char[strlen(in.name)+1];
		strncpy(name,in.name,strlen(in.name)+1);
		#ifdef DEBUG
			printf("%s: in timestamp %ld %d\n"
				,in.name
				,in.timestamp.tv_sec
				,in.timestamp.tv_usec);
			printf("%s: this timestamp %ld %d\n"
				,name,timestamp.tv_sec
				,timestamp.tv_usec);
		#endif
	}
	
	TimerClass& operator=(const TimerClass& in)
	{
		if(this != &in){
			#ifdef DEBUG
				printf("Copy Assignment\n");
			#endif
			timestamp = in.timestamp;
			if(name != nullptr) delete[] name;
			name = new char[strlen(in.name)+1];
			strncpy(name,in.name,strlen(in.name)+1);
			#ifdef DEBUG
				printf("%s: in timestamp %ld %d\n"
					,in.name
					,in.timestamp.tv_sec
					,in.timestamp.tv_usec);
				printf("%s: this timestamp %ld %d\n"
					,name
					,timestamp.tv_sec
					,timestamp.tv_usec);
			#endif
		}
		return *this;
	}
	
	TimerClass(TimerClass&& in) noexcept
		: timestamp{in.timestamp}
		  ,name{}
	{
			#ifdef DEBUG
				printf("Move Constructor\n");
			#endif
			if(name != nullptr) delete(name);
			name = in.name;
			in.name = nullptr;
			in.timestamp.tv_sec = 0;
			in.timestamp.tv_usec = 0;
	}

	TimerClass& operator=(TimerClass&& in) noexcept {
		if(this != &in){
			#ifdef DEBUG
				printf("Move Assignment\n");
			#endif
			timestamp = in.timestamp;
			if(name != nullptr) delete(name);
			name = in.name;
			in.name = nullptr;
			in.timestamp.tv_sec = 0;
			in.timestamp.tv_usec = 0;
		}
		return *this;
	}
private:
	struct timeval timestamp;
	char* name;
};

auto fib(TimerClass& in_timer, long long num){
	TimerClass timer = std::move(in_timer);
	if(num == 0 || num == 1) return num;
	long long old{0};
	long long last{1};
	long long result{1};
	for(auto i=2;i<num;++i){
		old = last;
		last = result;
		result = old + last;
	}
	return result;
}

auto fib_helper(long long num){
	if(num <=2) return 1;
	return fib_helper(num-1)+fib_helper(num-2);
}

auto fib_recur(TimerClass& in_timer, long long num){
	TimerClass timer = std::move(in_timer);
	return fib_helper(num);
}

int main(){
	TimerClass my_default_timer(nullptr);
	TimerClass my_timer("timer1");
	TimerClass my_timer2("timer2");
	my_timer2 = my_timer;
	TimerClass my_timer3(my_timer);
//	TimerClass my_timer4(std::move(my_timer3));
	TimerClass my_timer5("timer5");
//	my_timer5 = std::move(my_timer4);
	TimerClass my_new_default("timer6");
//	my_new_default = std::move(my_default_timer);
//	TimerClass timer_fib("fibinocci timer");
	for(auto j=1;j<=46;++j){
		TimerClass timer_fib("fibinocci timer");
		printf("The %dth fibinocci number is: %d\n"
			,j
			,fib(timer_fib,j));
	}
	for(auto j=1;j<=46;++j){
		TimerClass timer_fib("fibinocci recursive timer");
		printf("The %dth fibinocci number is: %d\n"
			,j
			,fib_recur(timer_fib,j));
	}
}