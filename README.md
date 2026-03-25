# Casa de schimb valutar

### Clasa 1: Currency

Obiectul care reprezinta valuta (numele si totalul resurselor valutei detinute de casa de schimb valutar).

getteri:
- getName()
- getTotal()

setteri:
- setName()
- setTotal()

operatori:
- operator<<
- operator=

### Clasa 2: CurrencyHistory

Obiectul care reprezinta istoricul preturilor setate de casa de schimb valutar (inclusiv cele mai recente preturi).
Istoricul este initializat cu referinta la obiectul valutei de baza.

getteri:
- getBuyingPrice() - returneaza pretul de cumparare in functie de data si valuta indicata
- getSellingPrice() - returneaza pretul de vanzare in functie de data si valuta indicata
- getLastDate() - returneaza ultima data introdusa in istoric (considerata data curenta)
- getMainCurrencyName() - returneaza valuta de baza a cursului valutar
- getMainCurrencyTotal() - returneaza totalul valutei de baza in posesia casei

setteri:
- setCurrencyPrices() - seteaza preturile de cumparare si vanzare ale unei valute intr-o anumita zi

alte metode:
- verifyCurrency() - verifica prezenta datelor despre o anumita valuta la o anumita data

### Clasa 3: Transaction

Obiectul care reprezinta tranzactia casei de schimb valutar.
La initializare, trebuie sa primeasca referinte la obiectele de tip:
1. CurrencyHistory - de unde obtine cursurile valutelor
2. Currency - valuta banilor adusi de client
3. Currency - valuta in care clientul doreste sa schimbe banii

getteri:
- getClientName() - returneaza numele clientului care initiaza tranzactia
- getSumIn() - returneaza suma introdusa de client
- getSumOut() - returneaza suma pe care clientul o obtine la tranzactie
- getDate() - returneaza data tranzactiei

setteri:
- setClientName() - seteaza numele clientului care initiaza tranzactia
- setSumIn() - seteaza cantitatea de bani pe care o da clientul

alte functii:
- initiate() - initiaza tranzactia
- verifyTransactionInfo() - verifica daca tranzactia are suficienta informatie pentru a fi executata
