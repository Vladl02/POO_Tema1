#include <iostream>

const int recordCapacity = 10;

int word_length(const char word[]);


struct Word {
    int length;
    char* word;

    Word(): length(0), word(nullptr){}

    Word(char word_string[]){
        length = word_length(word_string);
        word = new char[length+1];

        for (int i=0; i<length+1; i++){
            word[i] = word_string[i];
        }
    }
    ~Word(){
        delete[] word;
    }
};

void delete_word(Word* w);

struct Record {
    int capacity;
    int filled;
    Word* date;
    Word** currencies;
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
    Record(Word* date){
        filled = 0;
        capacity = recordCapacity;
        this->date = date;
        currencies = new Word*[recordCapacity];
        price_buying = new double[recordCapacity];
        price_selling = new double[recordCapacity];
    }

    ~Record(){
        delete_word(date);
        for (int j=0; j<filled; j++){
            delete_word(currencies[j]);
        }
        delete[] currencies;
        delete[] price_buying;
        delete[] price_selling;
    }
};


Word* create_word(int length, const char* word_array){
    Word* w = new Word;
    w->length = length;
    w->word = new char[length+1];

    for (int i=0; i<length+1; i++){
        w->word[i] = word_array[i];
    }

    return w;
}
void delete_word(Word* w){
    if (w == nullptr) return;
    delete w;
}
bool verify_word_match(Word* word1, Word* word2){
    if (word1->length != word2->length) return 0;
    for (int i=0; i< word1->length; i++){
        if (word1->word[i] != word2->word[i]) return 0;
    }
    return 1;
}
int word_length(const char word[]){
    int i = 0;
    while (word[i] != '\0') {
        i++;
    }
    return i;
}

Record* create_record(Word* date){
    Record* rec = new Record;
    rec->filled = 0;
    rec->date = date;
    rec->capacity = recordCapacity;
    rec->currencies = new Word*[recordCapacity];
    rec->price_buying = new double[recordCapacity];
    rec->price_selling = new double[recordCapacity];
    return rec;
}

void add_currency(Record* record, Word* currency, double priceBuy, double priceSell){
    int ix = record->filled;
    record->currencies[ix] = currency;
    record->price_buying[ix] = priceBuy;
    record->price_selling[ix] = priceSell;
    record->filled += 1;
}


void delete_record(Record* record){
    delete record;
}



class Currency{
private:
    Word* name;
    double total;
public:

    Currency() { name = nullptr; total = 0; }
    Currency(char name[]);
    Currency(char name[], double total);

    Word* getName() const { return name; }
    double getTotal() const { return total; }


    void setName(const char name[]){
        if (this->name != nullptr){
            delete_word(this->name);
        }

        this->name = create_word(word_length(name), name);
    }
    void setTotal(const double & total){
        this->total = total;
    }

    ~Currency(){
        delete_word(name);
        std::cout << "deleted curency" << '\n';
    }
};

Currency::Currency(char name[]) { 
    this->name = create_word(word_length(name), name); 
    total = 0;
}
Currency::Currency(char name[], double total){ 
    this->name = create_word(word_length(name), name);  
    this->total = total; 
};



class CurrencyHistory{
private:
    Word* mainCurrency;
    int historyCapacity;
    int filledHistory;
    Record** records;
public:
    CurrencyHistory();
    CurrencyHistory(int historyCapacity);
    CurrencyHistory(char currency[], int historyCapacity);

    double getSellingPrice(Word* currencyName, Word* date) const;
    double getBuyingPrice(Word* currencyName, Word* date) const;

    void setCurrencyPrices(char curr_name[], char date_string[], double priceBuy, double priceSell);



    friend std::ostream& operator<<(std::ostream& out, CurrencyHistory& history);
    ~CurrencyHistory();
};

CurrencyHistory::CurrencyHistory(): mainCurrency(nullptr), historyCapacity(20), filledHistory(0){
    this->records = new Record*[historyCapacity];
    for (int i = 0; i < historyCapacity; i++) {
        this->records[i] = nullptr;
    }
}
CurrencyHistory::CurrencyHistory(int historyCapacity): mainCurrency(nullptr), filledHistory(0)  { 
    this->historyCapacity=historyCapacity; 
    this->records = new Record*[historyCapacity];
    for (int i = 0; i < historyCapacity; i++) {
        this->records[i] = nullptr;
    }
}

CurrencyHistory::CurrencyHistory(char currency[], int historyCapacity): filledHistory(0){ 
    mainCurrency = create_word(word_length(currency), currency);
    this->historyCapacity=historyCapacity;
    this->records = new Record*[historyCapacity];
    for (int i = 0; i < historyCapacity; i++) {
        this->records[i] = nullptr;
    }
}
CurrencyHistory::~CurrencyHistory(){
    delete_word(mainCurrency);
    for (int i=0; i<filledHistory; i++){
        delete_record(records[i]);
    }
    delete[] records;
    std::cout << "cleaned history\n";
}

double CurrencyHistory::getBuyingPrice(Word* currencyName, Word* date) const{
    for (int i=0; i<filledHistory; i++){
        if (verify_word_match(records[i]->date, date)){
            Record* passing_record = records[i];
            for (int j=0; j< passing_record->filled ; j++){
                if (verify_word_match(passing_record->currencies[j], currencyName)){
                    return passing_record->price_buying[j];
                }
            }
        }
    }
    return 0.0f;
}

double CurrencyHistory::getSellingPrice(Word* currencyName, Word* date) const{
    for (int i=0; i<filledHistory; i++){
        if (verify_word_match(records[i]->date, date)){
            Record* passing_record = records[i];
            for (int j=0; j< passing_record->filled ; j++){
                if (verify_word_match(passing_record->currencies[j], currencyName)){
                    return passing_record->price_selling[j];
                }
            }
        }
    }
    return 0.0f;;
}

std::ostream& operator<<(std::ostream& out, CurrencyHistory& history){
    Record* curr_record;
    for (int i=0; i<history.filledHistory; i++){
        curr_record = history.records[i];
        out << "#### "<< "Date " << curr_record->date->word << " contains this prices:" << '\n';
        for (int j=0; j< curr_record->filled ; j++){
            out << "-" << curr_record->currencies[j]->word << "   "<< "buy: " << curr_record->price_buying[j] << "   " << "sell: " << curr_record->price_selling[j] << '\n';
        }
    }
    return out;
}


void CurrencyHistory::setCurrencyPrices(char curr_Name[], char date_string[], double priceBuy, double priceSell){
    Word* currencyName = create_word(word_length(curr_Name), curr_Name);
    Word* date = create_word(word_length(date_string), date_string);
    bool found_date = false;
    bool found_currency = false;
    for (int i=0; i<filledHistory; i++){
        if (verify_word_match(records[i]->date, date)){
            found_date = true;

            Record* passing_record = records[i];
            for (int j=0; j< passing_record->filled ; j++){
                if (verify_word_match(passing_record->currencies[j], currencyName)){
                    found_currency = true;
                    passing_record->price_buying[j] = priceBuy;
                    passing_record->price_selling[j] = priceSell;
                    break;
                }
            }

            if (!found_currency){
                add_currency(records[i], currencyName, priceBuy, priceSell);
            } else {
                delete currencyName;
            }
            break;
        }
    }
    if (!found_date){
        records[filledHistory] = create_record(date);
        add_currency(records[filledHistory], currencyName, priceBuy, priceSell);
        filledHistory++;
    }
    else{
        delete date;
    }
}


/*

class Client{
private:
    std::string name;
    std::string currencyGives;
    std::string currencyGets;
    int moneyGives;
    int moneyGets;
public:
    Client(std::string name) { this->name = name; }
    Client(std::string name, std::string currencyGives, std::string currencyGets) { 
        this->name = name; 
        this->currencyGives=currencyGives;
        this->currencyGets=currencyGets;
    }
};


*/


class Transaction{
private:
    Currency* currency_in;
    Currency* currency_out;
    bool executed;
    Word* client_name;
    double sum_in;
    double sum_out;
public:
    static int tranzaction_id;
    Transaction();
    Transaction(char client_name[], CurrencyHistory* history, Currency* currency_in, Currency* currency_out, double sum_in);
    Transaction(char client_name[], CurrencyHistory* history, Currency* currency_in, Currency* currency_out);
    Transaction(char client_name[], CurrencyHistory* history);
    Transaction(char client_name[]);
    


};





int main(){
    char name1[] = "LEU";
    char name2[] = "USD";
    char name3[] = "MDL";
    char name4[] = "EUR";


    Currency c1(name1);

    std::cout << c1.getName()->word << '\n';
    std::cout << c1.getTotal() << '\n';
    c1.setName(name2);
    c1.setTotal(2000);
    std::cout << c1.getName()->word << '\n';
    std::cout << c1.getTotal() << '\n';

    char mainCurr[] = "LEU";
    CurrencyHistory history(mainCurr, 100);
    history.setCurrencyPrices(name2, "2026-10-12", 4.95, 4.99);
    history.setCurrencyPrices(name3, "2026-10-12", 5.95, 5.99);
    history.setCurrencyPrices(name2, "2026-10-13", 5.95, 5.99);
    operator<<(std::cout, history);
    history.setCurrencyPrices(name2, "2026-10-13", 5.45, 5.69);
    operator<<(std::cout, history);
 

    return 0;
}









/*
valute
istoric valute
clienti
tranzactii

valuta:
-denumire
-cantitate_totala

totalitatea sumei accesibile pentru cumparare/vanzare fiecare valuta


constructuri de initializare:
(nume_valuta, suma_valuta_cumparare, suma_valuta_vanzare)




istoric valute
-returneaza curs-vindere, curs-cumparare (pe baza la parametru data)


constructuri de initializare:
(valuta, pret_vanzare, pret_cumparare, data)




client
-returneaza totalitatea clientilor
-returneaza istoria clientului
-permite vinderea/cumpararea valutei de catre client
-


constructuri de initializare:
(nume_client)



tranzactii: ????
-istoria tranzactii(client, ...)
-







-------
creez totalurile:




creez clientii.



clientii sunt initializati()




tranzactia are: data, client, 










*/



