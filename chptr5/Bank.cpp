#include <cstdio>
#include <stdexcept>
struct AccountDatabase {
	virtual long is_in_database(const long id) const = 0;
	virtual double get_amount(const long id) const = 0;
	virtual void set_amount(const long id,const double amount) = 0;
	virtual ~AccountDatabase() = default;
};

struct InMemoryAccountDatabase : AccountDatabase {
	InMemoryAccountDatabase(long *ids,double *amounts,const size_t length) : 
		amount_table{ amounts },
		id_table{ ids },
		length {length } {
			if(!id_table) throw std::runtime_error("id table is NULL");
			if(!amount_table) throw std::runtime_error("amount table is NULL");
		}

	long is_in_database(const long id) const override {
		if(!id_table || !amount_table) return -1;
		for(auto i{0};i<length;i++)
			if(id_table[i] == id)
				return i;
		return -1;
	}

	double get_amount(const long id) const override {
		if(!id_table || !amount_table)
			throw std::runtime_error("Cannot get amount from uninitialized database");;
		auto result = this->is_in_database(id);
		if(result <0) 
			throw std::logic_error("ID not in database");
		return amount_table[result];
		
	}
	void set_amount(const long id,const double amount) override {
		if(!id_table || !amount_table)
			throw std::runtime_error("Cannot set amount to uninitialized database");
		auto result = this->is_in_database(id);
		if(result < 0) 
			throw std::logic_error("ID not in database");
		this->amount_table[result] = amount;
	}
private:
	long *id_table;
	double *amount_table;
	size_t length;
};
struct Logger {
	virtual ~Logger() = default;
	virtual void log_transfer(const long from, const long to, const double amount) const = 0;
};
struct ConsoleLogger : Logger {
	ConsoleLogger(const char* tag) : tag{tag} {}
	void log_transfer(const long from,const long to,const double amount) const override {
		printf("%s %ld -> %ld: %f\n",tag,from,to,amount);
	}
private:
	const char* tag;
};
struct FileLogger : Logger {
	void log_transfer(const long from, const long to, const double amount) const override {
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
	void make_transfer(const long from, const long to, const double amount) {
		try{
			this->set_amount(from,this->get_amount(from)-amount);
		}
		catch(std::runtime_error &e){
			printf("%s: %ld\n",e.what(),from);
			return;
		}
		catch(std::logic_error &e){
			printf("%s: %ld\n",e.what(),from);
			return;
		}
		try{
			this->set_amount(to,this->get_amount(to)+amount);
		}
		catch(std::runtime_error &e){
			printf("%s: %ld\n",e.what(),to);
			this->set_amount(from,this->get_amount(from)+amount);
			return;
		}
		catch(std::logic_error &e){
			printf("%s: %ld\n",e.what(),to);
			this->set_amount(from,this->get_amount(from)+amount);
			return;
		}
		if (logger) logger->log_transfer(from,to,amount);
	}
	double get_amount(const long id) const {
		return accountdb.get_amount(id);
	}
	void set_amount(const long id,const double amount) {
		accountdb.set_amount(id,amount);
	}
private:
	Logger* logger;
	AccountDatabase& accountdb;
};

int main(){
	ConsoleLogger console_logger("[from console]");
	FileLogger file_logger;
	long ids[] = { 1000, 2000, 4000, 3, 45355 };
	double start_amounts[] { 10000, 40, 30000.67, 24.95, 83553.34 };
	InMemoryAccountDatabase db1(ids,start_amounts, sizeof(ids)/sizeof(long));
	Bank bank(nullptr,db1);
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