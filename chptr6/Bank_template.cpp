#include <cstdio>
#include <stdexcept>
#include <string>
struct Account {
	Account(const long id, double amount) :
		id{ id },
		amount{ amount }{
	}
	Account(const long id) : id{ id }, amount{} {}
	long get_id() const {
		return id;
	}
	std::string to_string() const{
		return std::to_string(get_id());
	}
	void set_id(const long id){
		this->id = id;
	}
	double get_balance() const {
		return amount;
	}
	void set_balance(const double balance) {
		this->amount = balance;
	}
	void add_to_balance(const double amount){
		this->amount += amount;
	}
	void remove_from_balance(const double amount){
		if(amount > this->amount)
			throw std::runtime_error("Not enough money in account");
		this->amount -= amount;
	}
	bool operator==(const Account& other) const {
		return other.get_id()==this->get_id();
	}
private:
	long id;
	double amount;
};
template<typename T>
struct AccountDatabase {
	virtual T* is_in_database(const T id) const = 0;
	virtual double get_amount(const T id) const = 0;
	virtual void set_amount(const T id,const double amount) = 0;
	virtual ~AccountDatabase() = default;
};

template<typename T>
struct InMemoryAccountDatabase : AccountDatabase<T> {
	InMemoryAccountDatabase(T *accountArray,const size_t length)  : 
		id_table{ accountArray },
		length {length } {
			if(!id_table) throw std::runtime_error("id table is NULL");
		}

	T* is_in_database(const T id) const override {
		if(!id_table)
			throw std::runtime_error("Cannot get/set amount from uninitialized database");
		for(auto i{0};i<length;i++)
			if(id_table[i] == id)
				return &id_table[i];
		throw std::logic_error("ID not in database");
//		return nullptr;
	}

	double get_amount(const T id) const override {
		auto result = this->is_in_database(id);
/*		if(!result) 
			throw std::logic_error("ID not in database");*/
		return result->get_balance();
		
	}
	void set_amount(const T id,const double amount) override {
		T* result = this->is_in_database(id);
/*		if(!result) 
			throw std::logic_error("ID not in database");*/
		result->set_balance(amount);
	}
private:
	 T *id_table;
	size_t length;
};
template<typename T>
struct Logger {
	virtual ~Logger() = default;
	virtual void log_transfer(const T from, const T to, const double amount) const = 0;
};
template<typename T>
struct ConsoleLogger : Logger<T> {
	ConsoleLogger(const char* tag) : tag{tag} {}
	void log_transfer(const T from,const T to,const double amount) const override {
		printf("%s %ld -> %ld: %f\n",tag,from.get_id(),to.get_id(),amount);
	}
private:
	const char* tag;
};
template<typename T>
struct FileLogger : Logger<T> {
	void log_transfer(const T from, const T to, const double amount) const override {
			printf("[file] %ld,%ld,%f\n",from.get_id(),to.get_id(),amount);
		}
};
template<typename T>
struct Bank {
	Bank(Logger<T>* logger,AccountDatabase<T>& accountdb) : logger{logger},
		accountdb{accountdb} {}
	Bank(AccountDatabase<T>& accountdb) : logger{}, accountdb{accountdb} {}
	void set_logger(Logger<T>* new_logger) {
		logger = new_logger;
	}
	void make_transfer(const T from, const T to, const double amount) {
		try{
			this->set_amount(from,this->get_amount(from)-amount);
			printf("get_amount: %f\n",this->get_amount(from));
		}
		catch(std::runtime_error &e){
			printf("runtime error: %s: %ld\n",e.what(),from);
			return;
		}
		catch(std::logic_error &e){
			printf("logic error:%s: %ld\n",e.what(),from);
			return;
		}
		try{
			this->set_amount(to,this->get_amount(to)+amount);
			printf("get_amount: %f\n",this->get_amount(to));
			if (logger) logger->log_transfer(from,to,amount);
		}
		catch(std::runtime_error &e){
			printf("runtime error: %s: %ld\n",e.what(),to);
			this->set_amount(from,this->get_amount(from)+amount);
			return;
		}
		catch(std::logic_error &e){
			printf("logic error: %s: %ld\n",e.what(),to);
			this->set_amount(from,this->get_amount(from)+amount);
			return;
		}
	}
	double get_amount(const T id) const {
		return accountdb.get_amount(id);
	}
	void set_amount(const T id,const double amount) {
		accountdb.set_amount(id,amount);
	}
private:
	Logger<T>* logger;
	AccountDatabase<T>& accountdb;
};

int main(){
	ConsoleLogger<Account> console_logger("[from console]");
	FileLogger<Account> file_logger;
	Account ids[] {{1000,10000}, {2000,40}, {4000,30000.67}, {3,24.95}, {45355,83553.34} };
//	long ids[] = { 1000, 2000, 4000, 3, 45355 };
//	double start_amounts[] { 10000, 40, 30000.67, 24.95, 83553.34 };
	InMemoryAccountDatabase<Account> db1(ids, sizeof(ids)/sizeof(Account));

	Bank<Account> temp_bank(db1);
	printf("--temp bank--\n");

	printf("before transfer from %ld to %ld:\nfrom: %f to: %f\n",
	ids[0],ids[1],temp_bank.get_amount(ids[0]),temp_bank.get_amount(ids[1]));

	temp_bank.make_transfer(1000,2000,49);

	printf("after transfer from %ld to %ld:\nfrom: %f to: %f\n",
	ids[0],ids[1],temp_bank.get_amount(ids[0]),temp_bank.get_amount(ids[1]));

	printf("--temp bank--\n");

	Bank<Account> bank(nullptr,db1);
	bank.set_logger(&console_logger);

	printf("before transfer from %ld to %ld:\nfrom: %f to: %f\n",
	ids[0],ids[1],bank.get_amount(ids[0]),bank.get_amount(ids[1]));

	bank.make_transfer(1000,2000,49.95);

	printf("after transfer from %ld to %ld:\nfrom: %f to: %f\n",
	ids[0],ids[1],bank.get_amount(ids[0]),bank.get_amount(ids[1]));

	bank.set_amount(2000,49.95);
	printf("setting bank amount for account %ld: %lf\n",ids[1],bank.get_amount(2000));

	bank.set_logger(&file_logger);

	printf("before transfer from %ld to %ld:\nfrom: %f to: %f\n",
	ids[1],ids[2],bank.get_amount(ids[1]),bank.get_amount(ids[2]));

	bank.make_transfer(2000,4000,20.00);

	printf("after transfer from %ld to %ld:\nfrom: %f to: %f\n",
	ids[1],ids[2],bank.get_amount(ids[1]),bank.get_amount(ids[2]));

	bank.set_amount(3,20.00);
	printf("setting bank amount for account %ld: %lf\n",ids[3],bank.get_amount(3));

	long num = 30;
	bank.set_amount(ids[4],bank.get_amount(ids[4])+num);
	printf("setting bank amount for account %ld: %lf\n",ids[4],bank.get_amount(ids[4]));

	printf("bank amount after adding %ld: %lf\n",num, bank.get_amount(ids[4]));
	bank.make_transfer(6000,3000,78.34);
	bank.make_transfer(2000,3000,78.34);
	bank.make_transfer(6000,3,78.34);
}