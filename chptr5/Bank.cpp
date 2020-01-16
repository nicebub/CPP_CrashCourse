#include <cstdio>
struct AccountDatabase {
	virtual long get_amount() const = 0;
	virtual void set_amount(const long id) = 0;
	virtual ~AccountDatabase() = default;
};

struct InMemoryAccountDatabase : AccountDatabase {
	InMemoryAccountDatabase(long id) : id{ id } {}
	long get_amount() const override {
		return id;
	}
	void set_amount(const long id) override {
		this->id = id;
	}
	~InMemoryAccountDatabase(){
	}
private:
	long id;
};
struct Logger {
	virtual ~Logger() = default;
	virtual void log_transfer(long from, long to, double amount) = 0;
};
struct ConsoleLogger : Logger {
	ConsoleLogger(const char* tag) : tag{tag} {}
	void log_transfer(long from, long to, double amount) override {
		printf("%s %ld -> %ld: %f\n",tag,from,to,amount);
	}
private:
	const char* tag;
};
struct FileLogger : Logger {
	void log_transfer(long from, long to, double amount) override {
			printf("[file] %ld,%ld,%f\n",from,to,amount);
		}
};
struct Bank {
	Bank(Logger* logger,AccountDatabase& accountdb) : logger{logger},
		accountdb{accountdb} {}
	Bank(AccountDatabase& accountdb) : logger{}, accountdb{accountdb} {}
	void set_logger(Logger* new_logger) {
		logger = new_logger;
	}
	void make_transfer(long from, long to, double amount) {
		if (logger) logger->log_transfer(from,to,amount);
	}
	long get_amount() const {
		return accountdb.get_amount();
	}
	void set_amount(const long id) {
		accountdb.set_amount(id);
	}
private:
	Logger* logger;
	AccountDatabase& accountdb;
};

int main(){
	ConsoleLogger console_logger("[from console]");
	FileLogger file_logger;
	InMemoryAccountDatabase db1(24);
	Bank bank(nullptr,db1);
	bank.set_logger(&console_logger);
	bank.make_transfer(1000,2000,49.95);
	bank.set_amount(49.95);
	printf("bank amount: %ld\n",bank.get_amount());
	bank.set_logger(&file_logger);
	bank.make_transfer(2000,4000,20.00);
	bank.set_amount(20.00);
	printf("bank amount: %ld\n",bank.get_amount());
	long num = 30;
	bank.set_amount(bank.get_amount()+num);
	printf("bank amount after adding %ld: %d\n",num, bank.get_amount());
}