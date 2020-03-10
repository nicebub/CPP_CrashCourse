#include <cstdio>
#include <memory>

using FileGuard = std::shared_ptr<FILE>;

struct Hal {
	Hal(std::shared_ptr<FILE> file);
	~Hal();
	void write_status();

	std::shared_ptr<FILE> file;
};

Hal::Hal(std::shared_ptr<FILE> file) : file{file}{}

Hal::~Hal(){
	fprintf(file.get(),"Stop, Dave");
	
}

void Hal::write_status() {
	fprintf(file.get(), "I'm completely operational.");
}

void say_hello(FileGuard file) {
	fprintf(file.get(), "HELLO DAVE");
}

int main(){
	auto file = fopen("HAL9000", "w");
	if(!file) return errno;
	FileGuard file_guard{ file, fclose };
	Hal h1{file_guard};
	Hal h2{file_guard};
	Hal h3{file_guard};
	Hal h4{file_guard};
	h1.write_status();
	h2.write_status();
	h3.write_status();
	h4.write_status();
	
	return 0;
}