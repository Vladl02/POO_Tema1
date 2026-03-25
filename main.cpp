#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
const int recordCapacity = 10;


char* create_string_copy(const char* word_init){
    if (word_init == nullptr) return nullptr;
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
    Record(const Record& r){
        filled = r.filled;
        capacity = r.capacity;
        date = create_string_copy(r.date);
        currencies = new char*[r.capacity];
        price_buying = new double[r.capacity];
        price_selling = new double[r.capacity];

        for (int i=0; i<filled; i++){
            currencies[i] = create_string_copy(r.currencies[i]);
        }
        for (int i=0; i<filled; i++){
            price_buying[i] = r.price_buying[i];
        }
        for (int i=0; i<filled; i++){
            price_selling[i] = r.price_selling[i];
        }
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


bool add_currency(Record* record, char* currency, double priceBuy, double priceSell){
    int ix = record->filled;
    if (ix >= record->capacity) return false;
    record->currencies[ix] = currency;
    record->price_buying[ix] = priceBuy;
    record->price_selling[ix] = priceSell;
    record->filled += 1;
    return true;
}




class Currency{
private:
    char* name;
    double total;
public:

    Currency();
    Currency(const Currency& currency);
    Currency(const char* name);
    Currency(const char* name, const double& total);

    const char* getName() const { return name; }
    double getTotal() const { return total; }


    void setName(const char* name);
    void setTotal(const double& total);

    friend std::ostream& operator<<(std::ostream& out, Currency& history);
    Currency& operator=(const Currency& c);

    ~Currency();
};

Currency::Currency(){ 
    name = nullptr; total = 0; 
}
Currency::Currency(const Currency& currency){
    name = create_string_copy(currency.name);
    total = currency.total;
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
    this->name = create_string_copy(name);
}
void Currency::setTotal(const double& total){
    this->total = total;
}

std::ostream& operator<<(std::ostream& out, Currency& currency){
    out << "Currency: " << currency.name << " Total: " << std::fixed << std::setprecision(2) << currency.total << '\n';
    return out;
}


Currency& Currency::operator=(const Currency& c){
    if (this->name != nullptr){
        delete[] this->name;
    }
    name = create_string_copy(c.name);
    total = c.total;
    return (*this);
}


Currency::~Currency(){
    delete[] name;
}




class CurrencyHistory{
private:
    const Currency& mainCurrency;
    int historyCapacity;
    int filledHistory;
    Record** records;
public:
    CurrencyHistory(const Currency& c, int h);
    CurrencyHistory(const CurrencyHistory& mainCurrency);

    double getSellingPrice(const Currency& currency, const char* date) const;
    double getBuyingPrice(const Currency& currency, const char* date) const;
    const char* getLastDate() const;
    const char* getMainCurrencyName() const;
    double getMainCurrencyTotal() const;
    
    void setCurrencyPrices(const Currency& currency, const char* date_string, const double& priceBuy, const double& priceSell);

    bool verifyCurrency(const Currency& currency, const char* date) const;

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
CurrencyHistory::CurrencyHistory(const CurrencyHistory& history)
    : mainCurrency(history.mainCurrency), historyCapacity(history.historyCapacity), filledHistory(history.filledHistory){
    this->records = new Record*[historyCapacity];
    for (int i = 0; i < historyCapacity; i++) {
        this->records[i] = nullptr;
    }
    for (int i = 0; i < filledHistory; i++){
        records[i] = new Record(*history.records[i]);
    }
}


CurrencyHistory::~CurrencyHistory(){
    for (int i=0; i<filledHistory; i++){
        delete records[i];
    }
    delete[] records;
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
const char* CurrencyHistory::getLastDate() const{
    if (filledHistory >= 1) {
        return records[filledHistory-1]->date;
    } else {
        return nullptr;
    }
}

const char* CurrencyHistory::getMainCurrencyName() const{
    return mainCurrency.getName();
}
double CurrencyHistory::getMainCurrencyTotal() const{
    return mainCurrency.getTotal();
}

std::ostream& operator<<(std::ostream& out, CurrencyHistory& history){
    Record* curr_record;
    out << "---------------  Currency History  ------------------\n";
    for (int i=0; i<history.filledHistory; i++){
        curr_record = history.records[i];
        out << "### "<< "Date " << curr_record->date<< " contains this prices:" << '\n';
        for (int j=0; j< curr_record->filled ; j++){
            out << "-" << curr_record->currencies[j] << "   "<< "buy: "  << std::fixed << std::setprecision(2) << curr_record->price_buying[j] << "   " << "sell: " << curr_record->price_selling[j] << '\n';
        }
    }
    out << "-----------------------------------------------------\n";
    out << '\n';
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
                if (!add_currency(records[i], currencyName, priceBuy, priceSell)){
                    delete[] currencyName;
                    delete[] date;
                    return;
                }
             
            } else {
                delete[] currencyName;
            }
            break;
        }
    }
    if (!found_date){
        if (filledHistory >= historyCapacity) {
            delete[] date;
            delete[] currencyName;
            return;
        };
        records[filledHistory] = new Record(date);
        add_currency(records[filledHistory], currencyName, priceBuy, priceSell);
            
        filledHistory++;
    }
    else{
        delete[] date;
    }
}
bool CurrencyHistory::verifyCurrency(const Currency& currency, const char* date) const{
    for (int i=0; i<filledHistory; i++){
        if (strcmp(records[i]->date, date) == 0){
            Record* passing_record = records[i];
            for (int j=0; j< passing_record->filled ; j++){
                if (strcmp(passing_record->currencies[j], currency.getName()) == 0){
                    return true;
                }
            }
        }
    }
    return false;
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
    Transaction(const CurrencyHistory& history, Currency& currency_in, Currency& currency_out);
    Transaction(const CurrencyHistory& history, Currency& currency_in, Currency& currency_out, const char* client_name);
    Transaction(const CurrencyHistory& history, Currency& currency_in, Currency& currency_out, const char* client_name, const double& sum_in);
    

    const char* getClientName() const;
    double getSumIn() const;
    double getSumOut() const;
    const char* getDate() const;

    void setClientName(const char* client_name);
    void setSumIn(const double& sum_in);

    void initiate();
    bool verifyTransactionInfo();

    friend std::ostream& operator<<(std::ostream& out, Transaction& transaction);


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

const char* Transaction::getClientName() const{
    return client_name;
}
double Transaction::getSumIn() const{
    return sum_in;
}
double Transaction::getSumOut() const{
    return sum_out;
}
const char* Transaction::getDate() const{
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

bool Transaction::verifyTransactionInfo(){
    if (client_name == nullptr){
        std::cout << "Client name is not specified\n";
        return false;
    }
    if (date == nullptr){
        std::cout << "Date is not specified\n";
        return false;
    }
    if (currency_in_name == nullptr){
        std::cout << "Currency in is not specified in Transaction\n";
        return false;
    }
    if (currency_out_name == nullptr){
        std::cout << "Currency out is not specified in Transaction\n";
        return false;
    }
    return true;
}


void Transaction::initiate(){

    if (executed) {std::cout<<"Already processed transaction\n"; return;};
    if (!verifyTransactionInfo()) return;
    double payment;
    
    if (strcmp(currency_in.getName(), history.getMainCurrencyName()) == 0){

        if (!history.verifyCurrency(currency_out, date)){
            std::cout << "Currency data is absent in history\n";
            return;
        }

        payment = sum_in / history.getSellingPrice(currency_out, date);

        if (payment <= currency_out.getTotal()){
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
        
        if (!history.verifyCurrency(currency_in, date)){
            std::cout << "Currency data is absent in history\n";
            return;
        }
        
        
        payment = sum_in * history.getBuyingPrice(currency_in, date);

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

        if (!history.verifyCurrency(currency_in, date) || !history.verifyCurrency(currency_out, date)){
            std::cout << "Currency data is absent in history\n";
            return;
        }


        double payment_intermid;
        payment_intermid = sum_in * history.getBuyingPrice(currency_in, date);
        payment = payment_intermid / history.getSellingPrice(currency_out, date);
        if (payment <= currency_out.getTotal()){
            currency_in.setTotal(currency_in.getTotal() + sum_in);
            currency_out.setTotal(currency_out.getTotal() - payment);
            sum_out = payment;
            std::cout << "Tranzaction succeded\n";
            executed = true;
        }
    }
}


std::ostream& operator<<(std::ostream& out, Transaction& transaction){
    
    if (!transaction.verifyTransactionInfo()) return out;
    out << '\n';
    out << "--------  Transaction info  ---------\n";
    out << "- Client name: " << transaction.client_name << '\n';
    out << "- Date: " << transaction.date << '\n';
    out << "- Currencies: " << transaction.currency_in_name << " -> " << transaction.currency_out_name << '\n';
    out << "- Money in: " << std::fixed << std::setprecision(2) << transaction.sum_in << " " << transaction.currency_in_name << '\n';
    out << "- Money out: ";
    if (!transaction.executed){
        out << "??? " << transaction.currency_in_name << '\n';
        out << "- Execution state: " << "Pending" << '\n';
    } else {
        out << std::fixed << std::setprecision(2) << transaction.sum_out << " " << transaction.currency_out_name << '\n';
        out << "- Execution state: " << "Succesful" << '\n';
    }
    out << "-------------------------------------\n";
    out << '\n';
    return out;
}




int main(){
    bool terminal = false;

    // inițializări valute
    Currency RON("RON", 2000);
    Currency RON_init(RON);
    Currency USD("USD");
    Currency MDL("MDL");
    Currency EUR;

    RON.setTotal(2000);
    USD.setTotal(1000);
    MDL.setTotal(20000);

    EUR.setTotal(2000);
    EUR.setName("EUR");



    CurrencyHistory history(RON, 100);

    history.setCurrencyPrices(EUR, "18-03-2026", 5.04, 5.15);
    history.setCurrencyPrices(USD, "18-03-2026", 4.35, 4.48);
    history.setCurrencyPrices(MDL, "18-03-2026", 0.247, 0.260);

    history.setCurrencyPrices(EUR, "19-03-2026", 5.05, 5.15);
    history.setCurrencyPrices(USD, "19-03-2026", 4.38, 4.51);
    history.setCurrencyPrices(MDL, "19-03-2026", 0.247, 0.260);

    CurrencyHistory historyBackUp(history);


    history.setCurrencyPrices(EUR, "20-03-2026", 5.04, 5.16);
    history.setCurrencyPrices(USD, "20-03-2026", 4.35, 4.48);
    history.setCurrencyPrices(MDL, "20-03-2026", 0.247, 0.259);



    
    
    if (!terminal){

        operator<<(std::cout, history);
        operator<<(std::cout, historyBackUp);

        // getteri la CurrencyHistory
        std::cout << "Main currency: " << history.getMainCurrencyName() << ", total: ";
        std::cout << history.getMainCurrencyTotal() << '\n';
        std::cout << "EUR buying price: " << history.getBuyingPrice(EUR, "20-03-2026") << '\n';
        std::cout << "EUR buying price: " << history.getSellingPrice(EUR,  "20-03-2026") << '\n';
        

        // initializari de tranzactii
        Transaction t1(history, RON, USD, "Anton", 1000);


        Transaction t2(history, USD, RON);
        t2.setClientName("Anton");

        

        t1.initiate();
        t2.setSumIn(t1.getSumOut());
        t2.initiate();

        // info t1
        operator<<(std::cout, t1);
        
        // info t2
        std::cout << "t2 info: \n";
        std::cout << "Client name: " << t2.getClientName() << '\n';
        std::cout << "Sum in: " << t2.getSumIn() << '\n';
        std::cout << "Sum out: " << t2.getSumOut() << '\n';
        std::cout << "Date: " << t2.getDate() << '\n' << '\n';

        operator<<(std::cout, RON);
        std::cout << RON_init.getName() << ": " << RON_init.getTotal() << '\n';


    }
    
    
    

    char option1;
    char option2;
    char option3;
    char* name_input;
    double sum_in;
    Transaction* t = nullptr;
    Currency* currency_in = nullptr;
    Currency* currency_out = nullptr;
    while (terminal){
        std::cout << "### Alege Optiune: \n";
        std::cout << "a - Obtine istoric curs valutar\n";
        std::cout << "b - Initiaza tranzactie\n";
        std::cout << "c - Afiseaza totalurile valutelor\n";
        std::cout << "d - opreste program\n";
        std::cout << "Introdu: ";
        
    
        std::cin >> option1;
        system("cls"); 

        
        switch (option1)
        {
        case 'a':
            operator<<(std::cout, history);
            break;
        case 'b':
            std::cout << "Alege valuta de intrare ";
            std::cout << " [a]RON [b]USD [c]EUR [d]MDL : ";
            std::cin >> option2;
            switch (option2)
            {
            case 'a':
                currency_in = &RON;
                break;
            case 'b':
                currency_in = &USD;
                break;
            case 'c':
                currency_in = &EUR;
                break;
            case 'd':
                currency_in = &MDL;
                break;
            default:
                break;
            }

            std::cout << "Alege valuta de Iesire ";
            std::cout << " [a]RON [b]USD [c]EUR [d]MDL : ";
            std::cin >> option3;
            switch (option3)
            {
            case 'a':
                currency_out = &RON;
                break;
            case 'b':
                currency_out = &USD;
                break;
            case 'c':
                currency_out = &EUR;
                break;
            case 'd':
                currency_out = &MDL;
                break;
            default:
                break;
            }


            if (currency_in == nullptr || currency_out == nullptr) break;

            if (option2 == option3){
                std::cout << "A fost introdusa aceeasi valută de intrare si iesire, tranzactia nu poate fi infaptuita";
                break;
            }

            t = new Transaction(history, *currency_in, *currency_out);
            name_input = new char[20];

            std::cout << "Introdu nume client: ";
            std::cin >> name_input;
            t->setClientName(name_input);
            std::cout << "Introdu suma intrare: ";
            std::cin >> sum_in;
            t->setSumIn(sum_in);

            system("cls"); 
            t->initiate();


            
            operator<<(std::cout, (*t));

            delete[] name_input;
            delete t;


            break;
        case 'c':
            operator<<(std::cout, RON);
            operator<<(std::cout, USD);
            operator<<(std::cout, EUR);
            operator<<(std::cout, MDL);
            break;
        case 'd':
            terminal = false;
            break;
        default:
            break;
        }
    }





    return 0;
}





