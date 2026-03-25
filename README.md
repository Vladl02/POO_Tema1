# Casa de schimb valutar

### Clasa 1: Currency

Obiectul care reprezintă valuta (numele și totalul resurselor valutei deținute de casa de schimb valutar).

getteri:
- getName()
- getTotal()

setteri:
- setName()
- setTotal()

operatori:
- <<
- =

conține constructor de copier și destructor(eliberează memoria alocată dinamic)

### Clasa 2: CurrencyHistory

Obiectul care reprezinta istoricul prețurilor setate de casa de schimb valutar (inclusiv cele mai recente prețuri).
Istoricul este inițializat cu referința la obiectul valutei de bază.

getteri:
- getBuyingPrice() - returnează prețul de cumparare in funcție de data și valuta indicată
- getSellingPrice() - returneaza prețul de vânzare în funcție de data și valuta indicată
- getLastDate() - returnează ultima data introdusă în istoric (considerată data curentă)
- getMainCurrencyName() - returneaza valuta de baza a cursului valutar
- getMainCurrencyTotal() - returneaza totalul valutei de baza in posesia casei

setteri:
- setCurrencyPrices() - seteaza prețurile de cumpărare și vânzare ale unei valute într-o anumită zi

alte metode:
- verifyCurrency() - verifică prezența datelor despre o anumită valuta la o anumita dată

operatori:
- <<


conține constructor de copiere și destructor(eliberează memoria alocată dinamic);


### Clasa 3: Transaction

Obiectul care reprezintă tranzacția casei de schimb valutar.
La initializare, trebuie să primeasca referințe la obiectele de tip:
1. CurrencyHistory - de unde obține cursurile valutelor
2. Currency - valuta banilor aduși de client
3. Currency - valuta în care clientul dorește să schimbe banii

getteri:
- getClientName() - returneaza numele clientului care inițiaza tranzacția
- getSumIn() - returneaza suma introdusă de client
- getSumOut() - returneaza suma pe care clientul o obține la tranzacție
- getDate() - returnează data tranzacției

setteri:
- setClientName() - setează numele clientului care inițiaza tranzacția
- setSumIn() - setează cantitatea de bani pe care o dă clientul

alte functii:
- initiate() - inițiază tranzacția
- verifyTransactionInfo() - verifică dacă tranzacția are suficientă informație pentru a fi executată

operatori:
- <<

conține destructor(eliberează memoria alocată dinamic);


### Intefață

Utilizez o interfață de terminal. Interfata e din perspectiva a case de schimb valutar, ci nu din perspectiva clientului. Prin urmare, ea permite:
- vizualizarea istoriei cursului 
- initializarii de tranzactii 
- afisari ale sumelor de bani pe care le are casa de schimb valutar