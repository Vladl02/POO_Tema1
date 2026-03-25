# Casă de schimb valutar


### Clasa 1: Currency

Obiectul ce reprezintă valuta (numele și totalul resurselor valutei date deținute de casa de schimba valutar)


getteri:__
--getName()
--getTotal()

setteri:
-setName()
-setTotal()

-operator <<, operator =


### Clasa 2: CurrencyHistory

Obiectul ce reprezintă istoricul prețurilor setate de casa de schimb valutar(inclusiv cele mai recente prețuri),
Istoricul obligator este inițializat cu referință la obiectul valutei de bază;

getteri:
-getBuyingPrice - returneaza pretul de vanzare in dependenta de data si valuta indicata
-getSellingPrice - returneaza pretul de cumparare in dependenta de data si valuta indicata
-getLastDate - returneaza ultima dată introdusă in istoric(aceasta fiind data de azi)
-getMainCurrency - returneaza valuta de bază a cursului valutar
-getMainCurrencyTotal - returneaza totalul valutei de bază in posesia cursului valutar

setteri:
-SetCurrencyPrices - seteaza preturile de cumparat si vinzare a unei valute intr-o anumita zi

alte Metode:
-verifyCurrency - verifică prezența datelor despre o anumită valută pe o anumita dată


### Clasa 3: Transaction

Obiectul tranzactia casei de schimb valutare
La inițializare, obligatoriu trebuie să obțină referința obiectele de tip:
1) CurrencyHistory , de unde obține cursurile valutelor
2) Currency, care resprezinta valuta banilor aduși de client
3) Currency, care reprezintă valuta în care clientul dorește să schimbe banii


getteri:
-getClientName - returneaza numele ce clientului ce inițiază tranzacția
-getSumIn - returneaza suma introdusa de client
-getSumOut - returneaza suma pe clientul obține la tranzacție
-getDate - returnează data tranzacției

setteri:
-setClientName - seteaza numele ce clientului ce inițiază tranzacția
-setSumIn - seteaza cantitatea de bani pe care o dă clientul


alte funcții:
-initiate - initiază tranzacția
-verifyTransactionInfo - verifica daca tranzactia are destula informatie pentru a fie executata
