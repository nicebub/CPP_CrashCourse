#include <cstdarg>
#include <cstdio>

#define MAX_WORDSIZE 100

struct WordHistogram final{

	constexpr const void createhistogram(const int argc, const char** argv, bool recurse)  noexcept{
		if(recurse)
			for(auto i{1};i<argc;i++)
				histogram[countcharsrec(argv[i])-1] += 1;
		else
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
	constexpr const void createhistogramnorec(const int argc, const char** argv)  noexcept{
		for(auto i{1};i<argc;i++)
			histogram[countchars(argv[i])-1] += 1;
	}

	constexpr const int countcharsrec(const char* in)  noexcept{
		if(*in == '\0')
			return  0;
		else
			return 1 + countchars(in+1);
	}

	constexpr const int countchars(const char* in)  noexcept{
		int result{};
		const char* temp = in;
		while(*temp++){
			result += 1;
		}
		return result;
	}


private:
	size_t histogram[MAX_WORDSIZE]{};
};


int main(int argc, const char**argv){
	WordHistogram hist{};
	WordHistogram hist2{};
	printf("recursive\n");
	hist.createhistogram(argc,argv,true);
	hist.print();
	printf("not recursive\n");
	hist2.createhistogram(argc,argv,false);
	hist2.print();
}