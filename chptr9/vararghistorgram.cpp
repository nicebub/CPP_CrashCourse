#include <cstdarg>
#include <cstdio>

#define MAX_WORDSIZE 100

struct WordHistogram final{

	constexpr const void createhistogram(const int argc, const char** argv)  noexcept{
		for(auto i{1};i<argc;i++)
			histogram[countchars(argv[i])-1] += 1;
	}
	constexpr const void print(void) const noexcept {
		for(auto i{0};i<MAX_WORDSIZE;i++){
			if(histogram[i] !=0){
				for(auto space{i+1};space<MAX_WORDSIZE; space*=10)
					printf(" ");
				printf("%d: ",i+1);
				for(auto j{0};j<histogram[i];j++)
					printf("+");
				printf("\n");
			}
		}
	}
	constexpr const int countchars(const char* in) const noexcept{
		if(*in == '\0')
			return  0;
		else
			return 1 + countchars(in+1);
	}


private:
	size_t histogram[MAX_WORDSIZE]{};
};


int main(int argc, const char**argv){
	WordHistogram hist{};
	hist.createhistogram(argc,argv);
	hist.print();
}