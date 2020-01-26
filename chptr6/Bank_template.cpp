#include <cstdio>
#include <stdexcept>
#include <string>
struct Account {
	virtual std::string to_string() const = 0;
	virtual void set_id(const long id) = 0;
	virtual long get_id() const = 0;
	virtual double get_balance() const = 0;
	virtual void set_balance(const double balance) = 0;
	virtual void add_to_balance(const double balance) = 0;
	virtual void remove_from_balance(const double balance) = 0;
	virtual bool operator==(const Account& other) const = 0;
	virtual ~Account() = default;
};
struct CheckingAccount : Account {
	CheckingAccount(const long id, double amount) :
		id{ id },
		amount{ amount }{
	}
	CheckingAccount(const long id) : id{ id }, amount{} {}

	long get_id() const override {
		return id;
	}
	std::string to_string() const override {
		return std::to_string(get_id());
	}
	void set_id(const long id) override {
		this->id = id;
	}
	double get_balance() const override {
		return amount;
	}
	void set_balance(const double balance) override {
		this->amount = balance;
	}
	void add_to_balance(const double amount) override {
		this->amount += amount;
	}
	void remove_from_balance(const double amount) override {
		if(amount > this->amount)
			throw std::runtime_error("Not enough money in account");
		this->amount -= amount;
	}
	bool operator==(const Account& other) const override {
		return other.get_id()==this->get_id();
	}
private:
	long id;
	double amount;
};
struct SavingsAccount : Account {
	    SavingsAccount(const long id, double amount) :
		    id{ id },
		    amount{ amount }{
	    }
	    SavingsAccount(const long id) : id{ id }, amount{} {}

	    long get_id() const override {
		    return id;
	    }
	    std::string to_string() const override {
		    return std::to_string(get_id());
	    }
	    void set_id(const long id) override {
		    this->id = id;
	    }
	    double get_balance() const override {
		    return amount;
	    }
	    void set_balance(const double balance) override {
		    this->amount = balance;
	    }
	    void add_to_balance(const double amount) override {
		    this->amount += amount;
	    }
	    void remove_from_balance(const double amount) override {
		    if(amount > this->amount)
			    throw std::runtime_error("Not enough money in account");
		    this->amount -= amount;
	    }
	    bool operator==(const Account& other) const override {
		    return other.get_id()==this->get_id();
	    }
    private:
	    long id;
	    double amount;

};
template<typename T>
struct AccountDatabase {
	virtual T* is_in_database(const T *id) const = 0;
	virtual double get_amount(const T *id) const = 0;
	virtual void set_amount( T *id,const double amount) = 0;
    virtual void add_to_balance(T *id,const double amount) = 0;
	virtual void remove_from_balance(T *id,const double amount) = 0;
	virtual ~AccountDatabase() = default;
};

template<typename T>
struct InMemoryAccountDatabase : AccountDatabase<T> {
	InMemoryAccountDatabase(T *accountArray[],const size_t length)  :
		id_table{ accountArray },
		length { length } {
			if(!id_table) throw std::runtime_error("id table is uninitialized");
		}

	T* is_in_database(const T *id) const override {
		if(!id_table)
			throw std::runtime_error("Cannot get/set amount from uninitialized database");
		for(auto i{0};i<length;i++)
			if(*id_table[i] == *id)
				return id_table[i];
		throw std::logic_error("ID not in database");
//		return nullptr;
	}

	double get_amount(const T *id) const override {
		auto result = this->is_in_database(id);
/*		if(!result) 
			throw std::logic_error("ID not in database");*/
		return result->get_balance();
		
	}
	void set_amount( T *id,const double amount) override {
		auto result = this->is_in_database(id);
/*		if(!result) 
			throw std::logic_error("ID not in database");*/
		result->set_balance(amount);
	}
    void add_to_balance(T *id,const double amount) override {
	   auto result = this->is_in_database(id);
	   result->add_to_balance(amount);
    }
    void remove_from_balance(T *id,const double amount) override {
	   auto result = this->is_in_database(id);
	   result->remove_from_balance(amount);
    }
private:
	 T **id_table;
	size_t length;
};
template<typename T>
struct Logger {
	virtual ~Logger() = default;
	virtual void log_transfer(const T *from, const T *to, const double amount) const = 0;
};
template<typename T>
struct ConsoleLogger : Logger<T> {
	ConsoleLogger(const char* tag) : tag{tag} {}
	void log_transfer(const T *from,const T *to,const double amount) const override {
		printf("%s %ld -> %ld: %f\n",tag,from->get_id(),to->get_id(),amount);
	}
private:
	const char* tag;
};
template<typename T>
struct FileLogger : Logger<T> {
	void log_transfer(const T *from, const T *to, const double amount) const override {
			printf("[file] %ld,%ld,%f\n",from->get_id(),to->get_id(),amount);
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
	void make_transfer( T *from,  T *to, const double amount) {
		try{
			this->remove_from_balance(from,amount);
//			printf("get_amount: %f\n",this->get_amount(from));
		}
		catch(std::runtime_error &e){
			printf("runtime error: %s: %ld\n",e.what(),from->get_id());
			return;
		}
		catch(std::logic_error &e){
			printf("logic error:%s: %ld\n",e.what(),from->get_id());
			return;
		}
		try{
			this->add_to_balance(to,amount);
//			printf("get_amount: %f\n",this->get_amount(to));
			if (logger) logger->log_transfer(from,to,amount);
		}
		catch(std::runtime_error &e){
			printf("runtime error: %s: %ld\n",e.what(),to->get_id());
			this->set_amount(from,this->get_amount(from)+amount);
			return;
		}
		catch(std::logic_error &e){
			printf("logic error: %s: %ld\n",e.what(),to->get_id());
			this->set_amount(from,this->get_amount(from)+amount);
			return;
		}
	}
	double get_amount( T *id) const {
		return accountdb.get_amount(id);
	}
	void set_amount( T *id,const double amount) {
		accountdb.set_amount(id,amount);
	}
    void add_to_balance(T *id,const double amount)  {
	   accountdb.add_to_balance(id,amount);
    }
    void remove_from_balance(T *id,const double amount)  {
	   accountdb.remove_from_balance(id,amount);
    }
private:
	Logger<T>* logger;
	AccountDatabase<T>& accountdb;
};

int main(){
	ConsoleLogger<Account> console_logger("[from console]");
	FileLogger<Account> file_logger;
    Account *ids[6];
    CheckingAccount cacct1 {1000,10000};
    CheckingAccount cacct2 {2000,40};
    CheckingAccount cacct3 {4000,30000.67};
    CheckingAccount cacct4 {3,24.95};
    CheckingAccount cacct5 {45355,83553.34};
    SavingsAccount sacct1 { 6767, 20000.54};
    ids[0] = &cacct1;
    ids[1] = &cacct2;
    ids[2] = &cacct3;
    ids[3] = &cacct4;
    ids[4] = &cacct5;
    ids[5] = &sacct1;
	InMemoryAccountDatabase<Account> db1(ids, sizeof(ids)/sizeof(Account));

	Bank<Account> nologger_bank(db1);
	nologger_bank.make_transfer(ids[0],ids[1],49);
    
     Bank<Account> defaultlogger_bank(&console_logger,db1);

	Bank<Account> bank(nullptr,db1);
	bank.set_logger(&console_logger);

	bank.make_transfer(ids[0],ids[1],49.95);

	bank.set_amount(ids[1],49.95);
	printf("setting bank amount for account %ld: %lf\n",ids[1]->get_id(),bank.get_amount(ids[1]));

	bank.set_logger(&file_logger);

    for(auto j{0};j<3;j++){
	   bank.make_transfer(ids[1],ids[2],20.00);
    }

	printf("setting bank amount for account %ld: %lf\n",ids[4]->get_id(),bank.get_amount(ids[4]));
	bank.set_amount(ids[4],20.00);

	auto num = 30;
	printf("setting bank amount for account %ld: %lf\n",ids[4]->get_id(),bank.get_amount(ids[4]));
	bank.add_to_balance(ids[4],num);
    printf("bank amount after adding %d: %lf\n",num, bank.get_amount(ids[4]));

     CheckingAccount acct6 {6000,0};
     CheckingAccount acct7 {3000,50};

     bank.make_transfer(&acct6,&acct7,78.34);
	bank.make_transfer(ids[1],&acct7,78.34);
	bank.make_transfer(ids[0],&acct7,78.34);
	bank.make_transfer(&acct6,ids[3],78.34);
  
    bank.make_transfer(ids[2], ids[5], 10000);
    bank.make_transfer(ids[5],ids[4],10000);
}
