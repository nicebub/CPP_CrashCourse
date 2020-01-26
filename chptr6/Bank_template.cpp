#include <cstdio>
#include <stdexcept>
struct AccountDatabase {
	virtual double get_amount() const = 0;
	virtual void set_amount(const double amount) = 0;
	virtual ~AccountDatabase() = default;
};
struct InMemoryAccountDatabase : AccountDatabase {
	InMemoryAccountDatabase(double amount) : amount{ amount } {}
	double get_amount() const override {
		return amount;
	}
	void set_amount(const double amount) override {
		this->amount = amount;
	}
private:
	double amount;
};

struct Account {
	Account(long id,AccountDatabase & accountdb)
		 : id{ id },
			accountdb{ accountdb }{
	}
	long get_id() const {
		return id;
	}
	void set_id(const long id){
		this->id = id;
	}
	double get_balance() const {
		return accountdb.get_amount();
	}
	void set_balance(const double balance) {
		accountdb.set_amount(balance);
	}
	void add_to_balance(const double amount){
		accountdb.set_amount(accountdb.get_amount() + amount);
	}
	void remove_from_balance(const double amount){
		if(amount > accountdb.get_amount())
			throw std::runtime_error("Not enough money in account");
		accountdb.set_amount(accountdb.get_amount() - amount);
	}
private:
	long id;
	AccountDatabase& accountdb;
};

template<typename T>
struct Logger {
	virtual ~Logger() = default;
	virtual void log_transfer(T from, T to, double amount) = 0;
};
template<typename T>
struct ConsoleLogger : Logger<T> {
	ConsoleLogger(const char* tag) : tag{tag} {}
	void log_transfer(T from, T to, double amount) override {
		printf("%s %ld -> %ld: %f\n",tag,from,to,amount);
	}
private:
	const char* tag;
};
template<typename T>
struct FileLogger : Logger<T> {
	void log_transfer(T from, T to, double amount) override {
			printf("[file] %ld,%ld,%f\n",from,to,amount);
		}
};
template<typename T>
struct Bank {
	Bank(Logger<T>* logger,AccountDatabase& accountdb) : logger{logger},
		accountdb{accountdb} {}
	Bank(AccountDatabase& accountdb) : logger{}, accountdb{accountdb} {}
	void set_logger(Logger<T>* new_logger) {
		logger = new_logger;
	}
	void make_transfer(T from, T to, double amount) {
		if (logger) logger->log_transfer(from,to,amount);
	}
	double get_amount() const {
		return accountdb.get_amount();
	}
	void set_amount(const double id) {
		accountdb.set_amount(id);
	}
private:
	Logger<T>* logger;
	AccountDatabase& accountdb;
};

int main(){
	ConsoleLogger<long> console_logger("[from console]");
	FileLogger<long> file_logger;
	InMemoryAccountDatabase db1(24);
	Bank<long> bank(nullptr,db1);
	bank.set_logger(&console_logger);
	bank.make_transfer(1000,2000,49.95);
	bank.set_amount(49.95);
	printf("bank amount: %lf\n",bank.get_amount());
	bank.set_logger(&file_logger);
	bank.make_transfer(2000,4000,20.00);
	bank.set_amount(20.00);
	printf("bank amount: %lf\n",bank.get_amount());
	long num = 30;
	bank.set_amount(bank.get_amount()+num);
	printf("bank amount after adding %ld: %f\n",num, bank.get_amount());
	
	ConsoleLogger<Account> account_logger("[account]");

	Bank<Account> bank2(&account_logger,db1);
	
	bank2.set_logger(&account_logger);
	Account checking{1000,db1};
	Account savings{2000,db1};
	bank2.make_transfer(checking,savings,49.95);
	
}