#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

class Transaktion
{
private:
    string datum;
    string typ;
    string namn;
    double belopp;
    int antal_kompisar;
    string *kompisar;

public:
    Transaktion();
    ~Transaktion();
    Transaktion &operator=(const Transaktion &t);
    string hamtaNamn();
    double hamtaBelopp();
    int hamtaAntalKompisar();
    bool finnsKompis(const string &namnet);
    bool lasIn(istream &is);
    void skrivUt(ostream &os);
    void skrivTitel(ostream &os);
};

class Person
{
private:
    string namn;
    double betalat_andras; // ligger ute med totalt
    double skyldig;        // skyldig totalt

public:
    Person();
    Person(const string &namn, double betalat_andras, double skyldig);
    string hamtaNamn();
    double hamtaBetalat();
    double hamtaSkyldig();
    void skrivUt(ostream &os);
};

class PersonLista
{
private:
    int antal_personer;
    Person *personer;

public:
    PersonLista();
    ~PersonLista();
    PersonLista &operator=(const PersonLista &pl);
    void laggTill(Person ny_person);
    void skrivUtOchFixa(ostream &os);
    double summaSkyldig();
    double summaBetalat();
    bool finnsPerson(const string &namn);
};

class TransaktionsLista
{
private:
    int antal_transaktioner;
    Transaktion *transaktioner;

public:
    TransaktionsLista();
    ~TransaktionsLista();
    TransaktionsLista &operator=(const TransaktionsLista &tl);
    void lasIn(istream &is);
    void skrivUt(ostream &os);
    void laggTill(Transaktion &t);
    double totalKostnad();
    double liggerUteMed(const string &namnet);
    double arSkyldig(const string &namnet);
    PersonLista fixaPersoner();
};

int main()
{
    cout << "Startar med att läsa från en fil." << endl;

    TransaktionsLista transaktioner;
    std::ifstream is("resa2.txt");
    transaktioner.lasIn(is);

    int operation = 1;
    while (operation != 0)
    {
        cout << endl;
        cout << "Välj i menyn nedan:" << endl;
        cout << "0. Avsluta. Alla transaktioner sparas på fil." << endl;
        cout << "1. Skriv ut information om alla transaktioner." << endl;
        cout << "2. Läs in en transaktion från tangentbordet." << endl;
        cout << "3. Beräkna totala kostnaden." << endl;
        cout << "4. Hur mycket ärr en viss person skyldig?" << endl;
        cout << "5. Hur mycket ligger en viss person ute med?" << endl;
        cout << "6. Lista alla personer mm och FIXA" << endl;

        cin >> operation;
        cout << endl;

        switch (operation)
        {
        case 1:
        {
            transaktioner.skrivUt(cout);
            break;
        }
        case 2:
        {
            Transaktion transaktion;
            cout << "Ange transaktion i följande format" << endl;
            transaktion.skrivTitel(cout);
            transaktion.lasIn(cin);
            transaktioner.laggTill(transaktion);
            break;
        }
        case 3:
        {
            cout << "Den totala kostnanden för resan var "
                 << transaktioner.totalKostnad() << endl;
            break;
        }
        case 4:
        {
            cout << "Ange personen: ";
            string namn;
            cin >> namn;
            double ar_skyldig = transaktioner.arSkyldig(namn);
            if (ar_skyldig == 0.)
                cout << "Kan inte hitta personen " << namn << endl;
            else
                cout << namn << " är skyldig " << ar_skyldig << endl;
            break;
        }
        case 5:
        {
            cout << "Ange personen: ";
            string namn;
            cin >> namn;
            double ligger_ute_med = transaktioner.liggerUteMed(namn);
            if (ligger_ute_med == 0.)
                cout << "Kan inte hitta personen " << namn << endl;
            else
                cout << namn << " ligger ute med " << ligger_ute_med << endl;
            break;
        }
        case 6:
        {
            cout << "Nu skapar vi en personarray och reder ut det hela!" << endl;
            PersonLista lista = transaktioner.fixaPersoner();
            lista.skrivUtOchFixa(cout);
            break;
        }
        }
    }

    std::ofstream os("transaktioner.txt");
    transaktioner.skrivUt(os);

    return 0;
}

Transaktion::Transaktion()
{
    datum = "";
    typ = "";
    namn = "";
    belopp = 0.0;
    antal_kompisar = 0;
    kompisar = 0;
}

Transaktion::~Transaktion()
{
    delete[] kompisar;
    kompisar = 0;
}

Transaktion &Transaktion::operator=(const Transaktion &t)
{
    if (this != &t)
    {
        delete[] kompisar;
        datum = t.datum;
        typ = t.typ;
        namn = t.namn;
        belopp = t.belopp;
        antal_kompisar = t.antal_kompisar;
        kompisar = new string[antal_kompisar];
        for (int i = 0; i < antal_kompisar; i++)
        {
            kompisar[i] = t.kompisar[i];
        }
    }
    return *this;
}

string Transaktion::hamtaNamn() { return namn; }

double Transaktion::hamtaBelopp() { return belopp; }

int Transaktion::hamtaAntalKompisar() { return antal_kompisar; }

bool Transaktion::finnsKompis(const string &namnet)
{
    for (int i = 0; i < antal_kompisar; i++)
    {
        if (kompisar[i].find(namnet) != string::npos)
        {
            return true;
        }
    }
    return false;
}

bool Transaktion::lasIn(istream &is)
{
    delete[] kompisar;

    is >> datum >> typ >> namn >> belopp >> antal_kompisar;

    kompisar = new string[antal_kompisar];

    for (int i = 0; i < antal_kompisar; i++)
    {
        is >> kompisar[i];
    }
    return !is.eof();
}

void Transaktion::skrivUt(ostream &os)
{
    os << datum << "\t" << typ << "\t" << namn << "\t" << belopp << "\t" // \t för tab, blir lättare att tyda
       << antal_kompisar << "\t";

    for (int i = 0; i < antal_kompisar; i++)
    {
        os << kompisar[i] << "\t";
    }

    os << endl;
}

void Transaktion::skrivTitel(ostream &os)
{
    // skriv ut en titel. Tab mellan varje kolumn
    os << "Datum\tTyp\tNamn\tBelopp\tAntal\tLista över kompisar" << endl;
}

Person::Person()
{
    namn = "";
    betalat_andras = 0.0;
    skyldig = 0.0;
}

Person::Person(const string &namn, double betalat_andras, double skyldig)
{
    this->namn = namn;
    this->betalat_andras = betalat_andras;
    this->skyldig = skyldig;
}

string Person::hamtaNamn() { return namn; }

double Person::hamtaBetalat() { return betalat_andras; }

double Person::hamtaSkyldig() { return skyldig; }

void Person::skrivUt(ostream &os)
{
    os << namn << " ligger ute med " << betalat_andras << " och är skyldig "
       << skyldig;

    // Om mer än det skyldiga har betalats
    if (betalat_andras > skyldig)
    {
        os << ". Skall ha " << betalat_andras - skyldig << " från potten!" << endl;
        return;
    }

    os << ". Skall lägga " << skyldig - betalat_andras << " till potten!" << endl;
}

PersonLista::PersonLista()
{
    antal_personer = 0;
    personer = 0;
}

PersonLista::~PersonLista()
{
    delete[] personer;
    personer = 0;
}

PersonLista &PersonLista::operator=(const PersonLista &pl)
{
    if (this != &pl)
    {
        delete[] personer;
        antal_personer = pl.antal_personer;
        personer = new Person[antal_personer];

        for (int i = 0; i < antal_personer; i++)
        {
            personer[i] = pl.personer[i];
        }
    }
    return *this;
}

void PersonLista::laggTill(Person ny_person)
{
    Person *temp = 0;
    temp = new Person[antal_personer + 1];

    // kopierar till nya vektorn
    for (int i = 0; i < antal_personer; i++)
    {
        temp[i] = personer[i];
    }

    // lägg till nya person
    temp[antal_personer] = ny_person;
    antal_personer++;

    // Avallokera minnet
    delete[] personer;
    personer = temp;
}

void PersonLista::skrivUtOchFixa(ostream &os)
{
    for (int i = 0; i < antal_personer; i++)
    {
        personer[i].skrivUt(os);
    }

    double skillnad = 0.000000001;

    if (fabs(summaSkyldig() - summaBetalat()) < skillnad)
    {
        os << "Summan i potten stämmer!" << endl;
        return;
    }

    os << "Någon har tagit ut ellet betalat för mycket!" << endl;
}

double PersonLista::summaSkyldig()
{
    // Summan av vad alla är skyldiga
    double tot = 0.0;

    for (int i = 0; i < antal_personer; i++)
    {
        // hämta alla personers skyldighet
        tot += personer[i].hamtaSkyldig();
    }

    //  returnera totala summan av vad alla personer är skyldiga.
    return tot;
}

double PersonLista::summaBetalat()
{
    // Summan av betalningar från alla
    double tot = 0.0;

    for (int i = 0; i < antal_personer; i++)
    {
        tot += personer[i].hamtaBetalat();
    }

    return tot;
}

bool PersonLista::finnsPerson(const string &namn)
{
    for (int i = 0; i < antal_personer; i++)
    {
        // om namnet finns returneras true, annars false
        if (personer[i].hamtaNamn().find(namn) != string::npos)
        {
            return true;
        }
    }
    return false;
}

TransaktionsLista::TransaktionsLista()
{
    antal_transaktioner = 0;
    transaktioner = 0;
}

TransaktionsLista::~TransaktionsLista()
{
    delete[] transaktioner;
    transaktioner = 0;
}

TransaktionsLista &TransaktionsLista::operator=(const TransaktionsLista &tl)
{
    if (this != &tl)
    {
        delete[] transaktioner;
        antal_transaktioner = tl.antal_transaktioner;
        transaktioner = new Transaktion[antal_transaktioner];

        for (int i = 0; i < antal_transaktioner; i++)
        {
            transaktioner[i] = tl.transaktioner[i];
        }
    }
    return *this;
}

void TransaktionsLista::lasIn(istream &is)
{
    Transaktion t;

    while (t.lasIn(is))
    {
        laggTill(t);
    }
}

void TransaktionsLista::skrivUt(ostream &os)
{
    os << "Antal transaktioner = " << antal_transaktioner << endl;
    os << "Datum\tTyp\tNamn\tBelopp\tAntal\tLista över kompisar" << endl;

    for (int i = 0; i < antal_transaktioner; i++)
    {
        transaktioner[i].skrivUt(os);
    }
}

void TransaktionsLista::laggTill(Transaktion &t)
{
    // Allokering
    Transaktion *temp = 0;
    // Skapar ny vektor
    temp = new Transaktion[antal_transaktioner + 1];

    // kopiera till nya vektorn
    for (int i = 0; i < antal_transaktioner; i++)
    {
        temp[i] = transaktioner[i];
    }

    // lägg till nya transaktionen
    temp[antal_transaktioner] = t;
    antal_transaktioner++;

    // Avallokering
    delete[] transaktioner;
    transaktioner = temp;
}

double TransaktionsLista::totalKostnad()
{
    double tot = 0.0;

    for (int i = 0; i < antal_transaktioner; i++)
    {
        tot += transaktioner[i].hamtaBelopp();
    }

    return tot;
}

double TransaktionsLista::liggerUteMed(const string &namnet)
{
    double tot = 0.0;

    for (int i = 0; i < antal_transaktioner; i++)
    {
        // hoppar över om det är fel person
        if (transaktioner[i].hamtaNamn() != namnet)
        {
            continue;
        }

        double hamtaAntalKompisar = transaktioner[i].hamtaAntalKompisar() + 1;
        double hamtaBelopp = transaktioner[i].hamtaBelopp();

        tot += hamtaBelopp * (1.0 - 1.0 / hamtaAntalKompisar);
    }
    return tot;
}

double TransaktionsLista::arSkyldig(const string &namnet)
{
    double tot = 0.0;

    for (int i = 0; i < antal_transaktioner; i++)
    {
        // Hoppar över om det är fel person
        if (!transaktioner[i].finnsKompis(namnet))
        {
            continue;
        }

        double hamtaBelopp = transaktioner[i].hamtaBelopp();
        double hamtaAntalKompisar = transaktioner[i].hamtaAntalKompisar() + 1;

        tot += (hamtaBelopp / hamtaAntalKompisar);
    }
    return tot;
}

PersonLista TransaktionsLista::fixaPersoner()
{
    PersonLista personList;

    for (int i = 0; i < antal_transaktioner; i++)
    {
        string namn = transaktioner[i].hamtaNamn();

        // om personen finns, skippa
        if (personList.finnsPerson(namn))
        {
            continue;
        }

        // annars skapa personen
        string personNamn = transaktioner[i].hamtaNamn();
        double personBetalat = liggerUteMed(personNamn);
        double personSkyldig = arSkyldig(personNamn);

        Person nyPerson(personNamn, personBetalat, personSkyldig);
        personList.laggTill(nyPerson);
    }
    return personList;
}