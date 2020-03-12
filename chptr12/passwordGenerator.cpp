#include <random>
#include <iostream>
#include <sstream>

using namespace std;

class PasswordGenerator{
public:
	PasswordGenerator(size_t length=15) : password_length{length}, 
		rd_engine{}, 
		distribution {0,61 }, 
//		password[password_length]{}
		password{ new char[password_length]}
	{
		initAlphabet();
	}
	~PasswordGenerator(){
		delete [] password;
	}
	void initAlphabet(void){
		for(auto i{0};i<26;i++)
			alphabet[i] = 'a' + i;
		for(auto i{26};i<52;i++)
			alphabet[i] = 'A' + (i-26);
		for(auto i{52};i<62;i++)
			alphabet[i] = '0' + (i-52);
/*
		for(auto c : alphabet)
			cout << c;
*/
	}
	auto getRandom(){
		return distribution(rd_engine);
	}
	auto getPassword(){
		for(auto i{0};i<password_length;i++){
			password[i] = alphabet[getRandom()];
		}
		return password;
	}

private:
	const static size_t num_chars = 62;
	const size_t password_length;
	char alphabet[num_chars];
	std::random_device rd_engine;
	std::uniform_int_distribution<size_t> distribution;
	char* password;
};

int main(int argc, const char ** argv){
	size_t length{};
	if(argc == 2){
		stringstream s{argv[1]};
		s >> length;
	}
	else
		length = 15;
	PasswordGenerator gen{length};
	cout << gen.getPassword() << endl;
	return 0;
}
