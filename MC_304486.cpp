#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <algorithm>
#include <fstream>
#include <limits>
using namespace std;

template <typename T1>
class Pojazd
{
protected:
    string typ;
    string marka;
    string vin;
    string nr_rej;
    string model;
    int rok;

public:
    T1 cecha;

    Pojazd() {}
    ~Pojazd()
    {
        cout << "pojazd koniec\t";
    }

    virtual void wypisz() {}

    const string& getVin() const {
        return vin;
    }

    const string& getNrRej() const {
        return nr_rej;
    }
};

class Samochod : public Pojazd<bool>
{
public:
    Samochod(){}
    Samochod(string mar, string mod, string v, string rej, int r, bool c)
    {
        marka = mar;
        vin = v;
        nr_rej = rej;
        model = mod;
        rok = r;
        cecha = c;
        cout << "samochod poczatek\t\n";
    }
    ~Samochod() { cout << "samochod koniec\t"; }

    void wypisz() override
    {
        if (cecha)
        {
            cout << "Typ pojazdu:\tSamochod skrzynia manualna\n"
                 << "Marka:\t" << marka << "\tModel:\t" << model << "\tNr VIN:\t" << vin << "\tNr rejestracyjny:\t" << nr_rej << "\tRok produkcji:\t" << rok << endl;
        }
        else
        {
            cout << "Typ pojazdu:\tSamochod skrzynia automatyczna\n"
                 << "Marka:\t" << marka << "\tModel:\t" << model << "\tNr VIN:\t" << vin << "\tNr rejestracyjny:\t" << nr_rej << "\tRok produkcji:\t" << rok << endl;
        }
    }
};

class Motor : public Pojazd<string>
{
public:
    Motor(){}
    Motor(string mar, string mod, string v, string rej, int r, string c)
    {
        marka = mar;
        vin = v;
        nr_rej = rej;
        model = mod;
        rok = r;
        cecha = c;
        cout << "motor poczatek\t\n";
    }
    ~Motor() { cout << "Motor koniec\t"; }

    void wypisz() override
    {
        if (cecha == "skuter")
        {
            cout << "Typ pojazdu:\tSkuter\n"
                 << "Marka:\t" << marka << "\tModel:\t" << model << "\tNr VIN:\t" << vin << "\tNr rejestracyjny:\t" << nr_rej << "\tRok produkcji:\t" << rok << endl;
        }
        else
        {
            cout << "Typ pojazdu:\tMotocykl\n"
                 << "Marka:\t" << marka << "\tModel:\t" << model << "\tNr VIN:\t" << vin << "\tNr rejestracyjny:\t" << nr_rej << "\tRok produkcji:\t" << rok << endl;
        }
    }
};

class Baza
{
private:
    vector<variant<Pojazd<bool>*, Pojazd<string>*>> pojazdy;

public:
    Baza() {}
    ~Baza() {
        for (const auto &pojazd : pojazdy)
        {
            visit([](auto &&arg) { delete arg; }, pojazd);
        }
    }

    void dodaj_auto()
    {
        string marka, mod, vin, rej;
        int rok;
        bool cecha;

        cout << "Podaj marke samochodu: ";
        cin >> marka;
        cout << "Podaj model samochodu: ";
        cin >> mod;
        cout << "Podaj nr vin: ";
        cin >> vin;
        cout << "Podaj nr rejestracyjny: ";
        cin >> rej;
        cout << "Podaj nr rocznik: ";
        while (!(cin >> rok)) 
        {
            cin.clear(); // Wyczyszczenie błędu wejścia
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usunięcie złych danych z bufora
            cout << "Błędne dane. Wprowadź liczbę całkowitą ponownie: ";
        }
        cout << "Podaj rodzaj skrzyni biegów (1 - manual, 0 - automatyczna): ";
        while (!(cin >> cecha) || (cecha != 0 && cecha != 1)) 
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Błędne dane. Wprowadź poprawną wartość logiczną (0 lub 1): ";
        }

        pojazdy.push_back(new Samochod(marka, mod, vin, rej, rok, cecha));
    }

    void dodaj_motor()
    {
        string marka, mod, vin, rej, cecha;
        int rok;

        cout << "Podaj marke motoru: ";
        cin >> marka;
        cout << "Podaj model motoru: ";
        cin >> mod;
        cout << "Podaj nr vin: ";
        cin >> vin;
        cout << "Podaj nr rejestracyjny: ";
        cin >> rej;
        cout << "Podaj nr rocznik: ";
        while (!(cin >> rok)) 
        {
            cin.clear(); // Wyczyszczenie błędu wejścia
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usunięcie złych danych z bufora
            cout << "Błędne dane. Wprowadź liczbę całkowitą ponownie: ";
        }
        cout << "Podaj rodzaj motocykla (skuter/motocykl): ";
        cin >> cecha;

        pojazdy.push_back(new Motor(marka, mod, vin, rej, rok, cecha));
    }

    void wypisz_pojazdy()
    {
        for (const auto &pojazd : pojazdy)
        {
            visit([](auto &&arg) { arg->wypisz(); }, pojazd);
        }
    }

    int szukaj(bool ref, string szukane)
    {
        auto znaleziony = find_if(pojazdy.begin(), pojazdy.end(), [szukane, ref](const variant<Pojazd<bool>*, Pojazd<string>*>& pojazd_variant) {
            if (auto* pojazd_ptr = get_if<Pojazd<bool>*>(&pojazd_variant)) {
                return ref == 0 && (*pojazd_ptr)->getVin() == szukane;
            } else if (auto* pojazd_ptr = get_if<Pojazd<string>*>(&pojazd_variant)) {
                return ref == 1 && (*pojazd_ptr)->getNrRej() == szukane;
            }
            return false;
        });

        if (znaleziony != pojazdy.end())
        {
            // Oblicz indeks znalezionego elementu
            int indeks = distance(pojazdy.begin(), znaleziony);
            return indeks;
        }
        else
        {
            // Jeśli nie znaleziono, zwróć -1 (lub inną wartość oznaczającą brak znalezienia)
            return -1;
        }
    }

    void wypisz_szukane()
    {
        bool ref;
        cout << "Wybierz po czym chcesz szukać: 0 - nr VIN, 1 - nr rejestracyjny";
        cin >> ref;

        string szukane;
        cout << "Podaj nr VIN lub rejestracyjny: ";
        cin >> szukane;

        int indeks = szukaj(ref, szukane);

        if (indeks != -1)
        {
            visit([indeks](auto &&arg) { arg->wypisz(); }, pojazdy[indeks]);
        }
        else
        {
            cout << "Pojazd nieznaleziony." << endl;
        }
    }
    void usun()
    {
        bool ref;
        cout << "Wybierz po czym chcesz szukać poajzdu do usunięcia: 0 - nr VIN, 1 - nr rejestracyjny";
        cin >> ref;
        string szukane;
        cout << "Podaj nr VIN lub rejestracyjny: ";
        cin >> szukane;

        int indeks = szukaj(ref,szukane);
        if (indeks != -1)
        {
            auto it = pojazdy.begin() + indeks;
            visit([](auto &&arg) { delete arg; }, *it);
            pojazdy.erase(it);
            cout << "Pojazd usunięty." << endl;
        }
        else
        {
            cout << "Pojazd nie znaleziony." << endl;
        }
    }

    void zapisz_do_pliku()
    {
        ofstream outputFile("baza_pojazdow.txt");

        if (!outputFile.is_open()) {
            cerr << "Nie można otworzyć pliku do zapisu." << endl;
            return;
        }

        streambuf* originalCoutBuffer = cout.rdbuf();  // Zapamiętanie oryginalnego bufora cout
        cout.rdbuf(outputFile.rdbuf());  // Przekierowanie cout do pliku

        wypisz_pojazdy();  // Użyj istniejącej metody wypisz_pojazdy do wypisania do pliku

        cout.rdbuf(originalCoutBuffer);  // Przywrócenie oryginalnego bufora cout

        outputFile.close();  // Plik zostanie automatycznie zamknięty przy zakończeniu programu

        cout << "Dane zostały zapisane do pliku txt.\n" << endl;

        ofstream outputFile_bin("baza_pojazdow.foo", ios::binary);

        if (!outputFile_bin.is_open()) {
            cerr << "Nie można otworzyć pliku binarnego do zapisu." << endl;
            return;
        }
         // Zapisywanie rozmiaru wektora do pliku binarnego
        size_t rozmiarWektora = pojazdy.size();
        outputFile_bin.write(reinterpret_cast<char*>(&rozmiarWektora), sizeof(rozmiarWektora));

        // Zapisywanie każdego obiektu z wektora do pliku binarnego
        for (const auto& pojazd : pojazdy) {
            if (holds_alternative<Pojazd<bool>*>(pojazd)) {
                int typPojazdu = 0;  // 0 oznacza Samochod
                outputFile_bin.write(reinterpret_cast<const char*>(&typPojazdu), sizeof(typPojazdu));
                Samochod* samochod = dynamic_cast<Samochod*>(get<Pojazd<bool>*>(pojazd));
                outputFile_bin.write(reinterpret_cast<const char*>(samochod), sizeof(Samochod));
            } else if (holds_alternative<Pojazd<string>*>(pojazd)) {
                int typPojazdu = 1;  // 1 oznacza Motor
                outputFile_bin.write(reinterpret_cast<const char*>(&typPojazdu), sizeof(typPojazdu));
                Motor* motor = dynamic_cast<Motor*>(get<Pojazd<string>*>(pojazd));
                outputFile_bin.write(reinterpret_cast<const char*>(motor), sizeof(Motor));
            }
        }

        // Zamykanie pliku binarnego
        outputFile_bin.close();
        cout << "Dane zostały zapisane do pliku binarny.\n" << endl;
    }

    // void odczyt_binarny()
    // {
    //     ifstream inputFile_bin("baza_pojazdow.foo", ios::binary);

    //     if (!inputFile_bin.is_open()) {
    //         cerr << "Nie można otworzyć pliku binarnego do odczytu." << endl;
    //         return;
    //     }

    //     // Usuwanie istniejących obiektów w wektorze pojazdy
    //     for (auto& pojazd : pojazdy) {
    //         visit([](auto&& p) { delete p; }, pojazd);
    //     }
    //     pojazdy.clear();

    //     // Odczytywanie rozmiaru wektora z pliku binarnego
    //     size_t rozmiarWektora;
    //     inputFile_bin.read(reinterpret_cast<char*>(&rozmiarWektora), sizeof(rozmiarWektora));

    //     // Odczytywanie każdego obiektu z pliku binarnego i dodawanie do wektora
    //     for (size_t i = 0; i < rozmiarWektora; ++i) {
    //         int typPojazdu;
    //         inputFile_bin.read(reinterpret_cast<char*>(&typPojazdu), sizeof(typPojazdu));

    //         if (typPojazdu == 0) {
    //             // Samochod
    //             Samochod samochod;
    //             inputFile_bin.read(reinterpret_cast<char*>(&samochod), sizeof(Samochod));
    //             pojazdy.push_back(new Samochod(samochod));
    //         } else if (typPojazdu == 1) {
    //             // Motor
    //             Motor motor;
    //             inputFile_bin.read(reinterpret_cast<char*>(&motor), sizeof(Motor));
    //             pojazdy.push_back(new Motor(motor));
    //         }
    //     }

    //     inputFile_bin.close();
    //     cout << "Dane zostały wczytane z pliku binarnego.\n" << endl;
    // }

    void uruchom_baze()
    {
        int a;
        int b;
        while(1)
        {
            cout << "Wybierz operacje: \n\t1 - dodaj auto\n\t2 - dodaj motor\n\t3 - zapisz do pliku\n\t4 - szukaj pojazdu w bazie\n\t5 - wypisz pojazdy\n\t6 - usuń pojazd z bazy\n\t7 - zakończ pracę w bazie\n\t";
            while (!(cin >> a)) 
            {
                cin.clear(); // Wyczyszczenie błędu wejścia
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usunięcie złych danych z bufora
                cout << "Błędne dane. Wprowadź liczbę całkowitą ponownie: ";
            }

            if(a == 1)
            {
                dodaj_auto();
            }
            else if(a == 2)
            {
                dodaj_motor();
            }
            else if(a == 3)
            {
                zapisz_do_pliku();
            }
            else if(a == 4)
            {
                wypisz_szukane();
            }
            else if(a == 5)
            {
                wypisz_pojazdy();
            }
            else if(a == 6)
            {
                usun();
            }
            else if(a == 7)
            {
                cout << "Czy zapisać bazę:\n0 - TAK\n1 - NIE";
                while (!(cin >> b)) 
                {
                    cin.clear(); // Wyczyszczenie błędu wejścia
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Usunięcie złych danych z bufora
                    cout << "Błędne dane. Wprowadź liczbę całkowitą ponownie: ";
                }
                if(b == 0)
                {
                    zapisz_do_pliku();
                    exit( 0 );
                }
                else if(b == 1)
                {
                    break;
                }
            }
        }
    }
};

int main()
{
    Baza baza;

    baza.uruchom_baze();
    return 0;
}
