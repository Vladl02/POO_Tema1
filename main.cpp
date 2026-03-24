#include <iostream>
#include <cstring>
const int recordCapacity = 10;



char* create_string_copy(const char* word_init){
    char* word_dest = new char[strlen(word_init)+1];
    word_dest = strcpy(word_dest, word_init);
    return word_dest;
}


struct Record {
    int capacity;
    int filled;
    char* date;
    char** currencies;
    double* price_selling;
    double* price_buying;

    Record(){
        filled = 0;
        capacity = recordCapacity;
        date = nullptr;
        currencies = nullptr;
        price_buying = nullptr;
        price_selling = nullptr;
    }
    Record(char* date){
        filled = 0;
        capacity = recordCapacity;
        this->date = date;
        currencies = new char*[recordCapacity];
        price_buying = new double[recordCapacity];
        price_selling = new double[recordCapacity];
    }

    ~Record(){
        delete[] date;
        for (int j=0; j<filled; j++){
            delete[] currencies[j];
        }
        delete[] currencies;
        delete[] price_buying;
        delete[] price_selling;
    }
};



void add_currency(Record* record, char* currency, double priceBuy, double priceSell){
    int ix = record->filled;
    if (ix >= record->capacity) return;
    record->currencies[ix] = currency;
    record->price_buying[ix] = priceBuy;
    record->price_selling[ix] = priceSell;
    record->filled += 1;
}



class Currency{
private:
    char* name;
    double total;
public:

    Currency();
    Currency(const char* name);
    Currency(const char* name, const double& total);

    char* getName() const { return name; }
    double getTotal() const { return total; }


    void setName(const char* name);
    void setTotal(const double& total);

    ~Currency();
};

Currency::Currency(){ 
    name = nullptr; total = 0; 
}
Currency::Currency(const char* name) { 
    this->name = create_string_copy(name);
    total = 0;
}
Currency::Currency(const char* name, const double& total){ 
    this->name = create_string_copy(name);
    this->total = total; 
};

void Currency::setName(const char* name){
    if (this->name != nullptr){
        delete[] this->name;
    }
    this->name = new char[strlen(name)+1];
    this->name = strcpy(this->name, name);
}
void Currency::setTotal(const double& total){
    this->total = total;
}

Currency::~Currency(){
    delete[] name;
    std::cout << "deleted curency" << '\n';
}




class CurrencyHistory{
private:
    const Currency& mainCurrency;
    int historyCapacity;
    int filledHistory;
    Record** records;
public:
    CurrencyHistory(const Currency& c, int h);

    double getSellingPrice(const Currency& currency, const char* date) const;
    double getBuyingPrice(const Currency& currency, const char* date) const;
    char* getLastDate() const;
    char* getMainCurrencyName() const;
    double getMainCurrencyTotal() const;
    
    void setCurrencyPrices(const Currency& currency, const char* date_string, const double& priceBuy, const double& priceSell);

    friend std::ostream& operator<<(std::ostream& out, CurrencyHistory& history);
    ~CurrencyHistory();
};



CurrencyHistory::CurrencyHistory(const Currency& c, int h)
    : mainCurrency(c), historyCapacity(h), filledHistory(0){ 
    this->records = new Record*[historyCapacity];
    for (int i = 0; i < historyCapacity; i++) {
        this->records[i] = nullptr;
    }
}



CurrencyHistory::~CurrencyHistory(){
    for (int i=0; i<filledHistory; i++){
        delete records[i];
    }
    delete[] records;
    std::cout << "cleaned history\n";
}

double CurrencyHistory::getBuyingPrice(const Currency& currencyName, const char* date) const{
    for (int i=0; i<filledHistory; i++){
        if (strcmp(records[i]->date, date) == 0){
            Record* passing_record = records[i];
            for (int j=0; j< passing_record->filled ; j++){
                if (strcmp(passing_record->currencies[j], currencyName.getName()) == 0){
                    return passing_record->price_buying[j];
                }
            }
        }
    }
    return 0.0;
}
double CurrencyHistory::getSellingPrice(const Currency& currencyName, const char* date) const{
    for (int i=0; i<filledHistory; i++){
        if ((strcmp(records[i]->date, date) == 0)){
            Record* passing_record = records[i];
            for (int j=0; j< passing_record->filled ; j++){
                if (strcmp(passing_record->currencies[j], currencyName.getName()) == 0){
                    return passing_record->price_selling[j];
                }
            }
        }
    }
    return 0.0f;;
}
char* CurrencyHistory::getLastDate() const{
    if (filledHistory >= 1) {
        return records[filledHistory-1]->date;
    } else {
        return nullptr;
    }
}

char* CurrencyHistory::getMainCurrencyName() const{
    return mainCurrency.getName();
}
double CurrencyHistory::getMainCurrencyTotal() const{
    return mainCurrency.getTotal();
}

std::ostream& operator<<(std::ostream& out, CurrencyHistory& history){
    Record* curr_record;
    for (int i=0; i<history.filledHistory; i++){
        curr_record = history.records[i];
        out << "#### "<< "Date " << curr_record->date<< " contains this prices:" << '\n';
        for (int j=0; j< curr_record->filled ; j++){
            out << "-" << curr_record->currencies[j] << "   "<< "buy: " << curr_record->price_buying[j] << "   " << "sell: " << curr_record->price_selling[j] << '\n';
        }
    }
    return out;
}


void CurrencyHistory::setCurrencyPrices(const Currency& currency, const char* date_string, const double& priceBuy, const double& priceSell){
    char* currencyName = create_string_copy(currency.getName());
    char* date = create_string_copy(date_string);
    bool found_date = false;
    bool found_currency = false;
    for (int i=0; i<filledHistory; i++){
        if (strcmp(records[i]->date, date) == 0){
            found_date = true;

            Record* passing_record = records[i];
            for (int j=0; j< passing_record->filled ; j++){
                if (strcmp(passing_record->currencies[j], currencyName) == 0){
                    found_currency = true;
                    passing_record->price_buying[j] = priceBuy;
                    passing_record->price_selling[j] = priceSell;
                    break;
                }
            }

            if (!found_currency){
                add_currency(records[i], currencyName, priceBuy, priceSell);
            } else {
                delete[] currencyName;
            }
            break;
        }
    }
    if (!found_date){
        if (filledHistory >= historyCapacity) return;
        records[filledHistory] = new Record(date);
        add_currency(records[filledHistory], currencyName, priceBuy, priceSell);
        filledHistory++;
    }
    else{
        delete[] date;
    }
}



class Transaction{
private:
    Currency& currency_in;
    Currency& currency_out;
    const CurrencyHistory& history;
    char* client_name;
    char* date;
    char* currency_in_name;
    char* currency_out_name;
    bool executed;
    
    double sum_in;
    double sum_out;
public:
    static int tranzaction_id;
    Transaction(const CurrencyHistory& history, Currency& currency_in, Currency& currency_out);
    Transaction(const CurrencyHistory& history, Currency& currency_in, Currency& currency_out, const char* client_name);
    Transaction(const CurrencyHistory& history, Currency& currency_in, Currency& currency_out, const char* client_name, const double& sum_in);
    

    char* getClientName() const;
    double getSumIn() const;
    double getSumOut() const;
    char* getDate() const;

    void setClientName(const char* client_name);
    void setSumIn(const double& sum_in);

    void initiate();

    ~Transaction();
};



Transaction::Transaction(const CurrencyHistory& h, Currency& c_in, Currency& c_out)
 : history(h), currency_in(c_in), currency_out(c_out){
    this->client_name = nullptr;
    date = create_string_copy(history.getLastDate());
    currency_in_name = create_string_copy(currency_in.getName());
    currency_out_name = create_string_copy(currency_out.getName());
    sum_in=0;
    sum_out=0;
    executed=false;
}
Transaction::Transaction(const CurrencyHistory& h, Currency& c_in, Currency& c_out, const char* client_name)
 : history(h), currency_in(c_in), currency_out(c_out){
    this->client_name = create_string_copy(client_name);
    date = create_string_copy(history.getLastDate());
    currency_in_name = create_string_copy(c_in.getName());
    currency_out_name = create_string_copy(c_out.getName());
    sum_in=0;
    sum_out=0;
    executed=false;
}
Transaction::Transaction(const CurrencyHistory& h, Currency& c_in, Currency& c_out, const char* client_name, const double& sum_in)
 : history(h), currency_in(c_in), currency_out(c_out){
    this->client_name = create_string_copy(client_name);
    date = create_string_copy(history.getLastDate());
    currency_in_name = create_string_copy(c_in.getName());
    currency_out_name = create_string_copy(c_out.getName());
    this->sum_in=sum_in;
    sum_out=0;
    executed=false;
}

Transaction::~Transaction(){
    if (client_name != nullptr) delete[] client_name;
    if (date != nullptr) delete[] date;
    if (currency_in_name != nullptr) delete[] currency_in_name;
    if (currency_out_name != nullptr) delete[] currency_out_name;
}

char* Transaction::getClientName() const{
    return client_name;
}
double Transaction::getSumIn() const{
    return sum_in;
}
double Transaction::getSumOut() const{
    return sum_out;
}
char* Transaction::getDate() const{
    return date;
}

void Transaction::setClientName(const char* client_name){
    if (this->client_name != nullptr){
        delete[] this->client_name;
    }
    this->client_name = create_string_copy(client_name);
}
void Transaction::setSumIn(const double& sum_in){
    this->sum_in = sum_in;
}

void Transaction::initiate(){

    if (executed) {std::cout<<"Already processed tranzaction\n"; return;};

    double payment;
    
    if (strcmp(currency_in.getName(), history.getMainCurrencyName()) == 0){
        payment = sum_in / history.getBuyingPrice(currency_out, date);

        if (payment < currency_out.getTotal()){
            currency_in.setTotal(currency_in.getTotal() + sum_in);
            currency_out.setTotal(currency_out.getTotal() - payment);
            sum_out = payment;
            std::cout << "Tranzaction succeded\n";
            executed = true;
        } else {
            std::cout << "Tranzaction failed: insuficient money to pay\n";
            return;
        }
    } else if(strcmp(currency_out.getName(), history.getMainCurrencyName()) == 0){
        payment = sum_in * history.getSellingPrice(currency_in, date);

        if (payment <= currency_out.getTotal()){
            currency_in.setTotal(currency_in.getTotal() + sum_in);
            currency_out.setTotal(currency_out.getTotal() - payment);
            sum_out = payment;
            std::cout << "Tranzaction succeded\n";
            executed = true;
        } else {
            std::cout << "Tranzaction failed: insuficient money to pay\n";
        }
    } else {
        double payment_intermid;
        payment_intermid = sum_in * history.getSellingPrice(currency_in, date);
        payment = payment_intermid / history.getBuyingPrice(currency_out, date);
        if (payment < currency_out.getTotal()){
            currency_in.setTotal(currency_in.getTotal() + sum_in);
            currency_out.setTotal(currency_out.getTotal() - payment);
            sum_out = payment;
            std::cout << "Tranzaction succeded\n";
            executed = true;
        }
    }
}


int main(){

    Currency RON("RON");
    Currency USD("USD");
    Currency MDL("MDL");
    Currency EUR("EUR");

    RON.setTotal(2000);
    USD.setTotal(1000);
    MDL.setTotal(20000);
    EUR.setTotal(2000);



    CurrencyHistory history(RON, 100);


    history.setCurrencyPrices(USD, "2026-10-13", 4.38, 4.50);
    history.setCurrencyPrices(MDL, "2026-10-13", 0.245, 0.265);
    history.setCurrencyPrices(EUR, "2026-10-13", 5.05, 5.15);

    operator<<(std::cout, history);
    

    Transaction t1(history, RON, USD);
    t1.setClientName("Anton");
    t1.setSumIn(1000);
    t1.initiate();

    return 0;
}









/*

Add the constructor de copiere, = operator
<< constructors for all the classes



*/



