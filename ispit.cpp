#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <sstream>
using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

const char *crt = "\n-------------------------------------------\n";
const char *nedozvoljena_operacija = "Nedozvoljena operacija";
const char *not_set = "NOT_SET";
const int min_polgavlja = 3;
const int min_karaktera_po_poglavlju = 30;

char *AlocirajNizKaraktera(const char *sadrzaj)
{
    if (sadrzaj == nullptr)
    {
        return nullptr;
    }
    int vel = strlen(sadrzaj) + 1;
    char *temp = new char[vel];
    strncpy(temp, sadrzaj, vel);
    return temp;
}

template <class T1, class T2>
class Kolekcija
{
    T1 *_elementi1;
    T2 *_elementi2;
    int _trenutno;

public:
    Kolekcija() : _trenutno(0), _elementi1(nullptr), _elementi2(nullptr) {}
    Kolekcija(const Kolekcija &obj)
    {
        _trenutno = obj.getTrenutno();
        _elementi1 = new T1[_trenutno];
        _elementi2 = new T2[_trenutno];

        for (int i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = obj._elementi1[i];
            _elementi2[i] = obj._elementi2[i];
        }
    }
    Kolekcija(Kolekcija &&obj) noexcept
    {
        _trenutno = obj._trenutno;
        _elementi1 = obj._elementi1;
        _elementi2 = obj._elementi2;

        obj._trenutno = 0;
        obj._elementi1 = nullptr;
        obj._elementi2 = nullptr;
    }
    Kolekcija &operator=(const Kolekcija &obj)
    {
        if (this != &obj)
        {
            delete[] _elementi1;
            delete[] _elementi2;

            _trenutno = obj.getTrenutno();
            _elementi1 = new T1[_trenutno];
            _elementi2 = new T2[_trenutno];

            for (int i = 0; i < _trenutno; i++)
            {
                _elementi1[i] = obj._elementi1[i];
                _elementi2[i] = obj._elementi2[i];
            }
        }

        return *this;
    }
    ~Kolekcija()
    {
        delete[] _elementi1;
        _elementi1 = nullptr;
        delete[] _elementi2;
        _elementi1 = nullptr;
    }
    T1 *getElementi1Pok() { return _elementi1; }
    T2 *getElementi2Pok() { return _elementi2; }
    T1 &getElement1(int lokacija) const { return _elementi1[lokacija]; }
    T2 &getElement2(int lokacija) const { return _elementi2[lokacija]; }
    int getTrenutno() const { return _trenutno; }

    void DodajElemente(T1 elementi1, T2 elementi2)
    {
        T1 *temp1 = _elementi1;
        T2 *temp2 = _elementi2;
        _elementi1 = new T1[_trenutno + 1];
        _elementi2 = new T2[_trenutno + 1];

        for (int i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = temp1[i];
            _elementi2[i] = temp2[i];
        }

        delete[] temp1;
        delete[] temp2;
        temp1 = nullptr;
        temp2 = nullptr;

        _elementi1[_trenutno] = elementi1;
        _elementi2[_trenutno] = elementi2;
        _trenutno++;
    }

    friend ostream &operator<<(ostream &COUT, const Kolekcija<T1, T2> &obj)
    {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
};

class Poglavlje
{
    char *_naslov;
    char *_sadrzaj;
    bool _prihvaceno;
    int _ocjena; // da bi se poglavlje smatralo prihvacenim ocjena mora biti u opsegu od 6 - 10
public:
    Poglavlje(const char *naslov = nullptr, const char *sadrzaj = nullptr)
        : _ocjena(0), _prihvaceno(false)
    {
        _naslov = AlocirajNizKaraktera(naslov);
        _sadrzaj = AlocirajNizKaraktera(sadrzaj);
        _prihvaceno = false;
        _ocjena = 0;
    }
    Poglavlje(const Poglavlje &obj)
    {
        _naslov = AlocirajNizKaraktera(obj.GetNaslov());
        _sadrzaj = AlocirajNizKaraktera(obj.GetSadrzaj());
        _prihvaceno = obj._prihvaceno;
        _ocjena = obj._ocjena;
    }
    Poglavlje(Poglavlje &&obj) noexcept
    {
        _naslov = obj._naslov;
        _sadrzaj = obj._sadrzaj;
        _prihvaceno = obj._prihvaceno;
        _ocjena = obj._ocjena;

        obj._naslov = nullptr;
        obj._sadrzaj = nullptr;
        obj._prihvaceno = false;
        obj._ocjena = 0;
    }
    Poglavlje &operator=(const Poglavlje &obj)
    {
        if (this != &obj)
        {
            delete[] _naslov;
            delete[] _sadrzaj;

            _naslov = AlocirajNizKaraktera(obj.GetNaslov());
            _sadrzaj = AlocirajNizKaraktera(obj.GetSadrzaj());
            _prihvaceno = obj.GetPrihvaceno();
            _ocjena = obj.GetOcjena();
        }

        return *this;
    }
    ~Poglavlje()
    {
        delete[] _naslov;
        _naslov = nullptr;
        delete[] _sadrzaj;
        _sadrzaj = nullptr;
    }
    friend ostream &operator<<(ostream &COUT, Poglavlje &obj)
    {
        if (obj._naslov == nullptr || obj._sadrzaj == nullptr)
            return COUT;
        COUT << endl
             << obj._naslov << endl
             << obj._sadrzaj << endl;
        if (obj._prihvaceno)
            COUT << "Ocjena: " << obj._ocjena << endl;
        ;
        return COUT;
    }

    // getters
    char *GetNaslov() const { return _naslov; }
    char *GetSadrzaj() const { return _sadrzaj; }
    bool GetPrihvaceno() const { return _prihvaceno; }
    int GetOcjena() const { return _ocjena; }

    // setters
    void SetNaslov(const char *naslov)
    {
        _naslov = AlocirajNizKaraktera(naslov);
    }
    void SetSadrzaj(const char *sadrzaj)
    {
        _sadrzaj = AlocirajNizKaraktera(string(string(_sadrzaj) + string("  ") + string(sadrzaj)).c_str());
        // razlika (SetNaslov radi direktno dok SetSadrzaj radi kao append metoda)
    }
    void SetPrihvaceno(bool prihvaceno)
    {
        _prihvaceno = prihvaceno;
    }
    void SetOcjena(int ocjena)
    {
        _ocjena = ocjena;
    }

    friend ostream &operator<<(ostream &COUT, const Poglavlje &obj)
    {
        if (obj._naslov == nullptr || obj._sadrzaj == nullptr)
            return COUT;
        COUT << endl
             << obj._naslov << endl
             << obj._sadrzaj << endl;
        if (obj._prihvaceno)
            COUT << "Ocjena: " << obj._ocjena << endl;
        ;
        return COUT;
    }
};

bool operator==(const Poglavlje &obj1, const Poglavlje &obj2)
{
    bool tmp = true;

    if (strcmp(obj1.GetNaslov(), obj2.GetNaslov()) != 0)
    {
        tmp = false;
    }
    if (strcmp(obj1.GetSadrzaj(), obj2.GetSadrzaj()) != 0)
    {
        tmp = false;
    }
    if (obj1.GetPrihvaceno() != obj2.GetPrihvaceno())
    {
        tmp = false;
    }
    if (obj1.GetOcjena() != obj2.GetOcjena())
    {
        tmp = false;
    }

    return tmp;
}

bool operator!=(const Poglavlje &obj1, const Poglavlje &obj2)
{
    return !(obj1 == obj2);
}

class ZavrsniRad
{
    char *_tema;
    vector<Poglavlje> _poglavljaRada;
    string _datumOdbrane;
    float _konacnaOcjena; // prosjek ocjena svih poglavlja u zavrsnom radu koja se izracunava u momentu zakazivanja odbrane
public:
    ZavrsniRad(const char *nazivTeme = nullptr) : _konacnaOcjena(0.0f), _datumOdbrane(not_set)
    {
        _tema = AlocirajNizKaraktera(nazivTeme);
    }

    ZavrsniRad(const ZavrsniRad &org) : _poglavljaRada(org._poglavljaRada), _datumOdbrane(org._datumOdbrane)
    {
        _tema = AlocirajNizKaraktera(org.GetNazivTeme());
        _konacnaOcjena = org.GetOcjena();
    }
    ZavrsniRad(ZavrsniRad &&obj) noexcept : _poglavljaRada(move(obj._poglavljaRada)), _datumOdbrane(move(obj._datumOdbrane))
    {
        _tema = obj._tema;
        _konacnaOcjena = obj._konacnaOcjena;

        obj._tema = nullptr;
        obj._konacnaOcjena = 0.0f;
    }
    ZavrsniRad &operator=(const ZavrsniRad &obj)
    {
        if (this != &obj)
        {
            delete[] _tema;
            _poglavljaRada.clear();

            _tema = AlocirajNizKaraktera(obj.GetNazivTeme());
            _poglavljaRada = obj._poglavljaRada;
            _datumOdbrane = obj.GetDatumOdbrane();
            _konacnaOcjena = obj.GetOcjena();
        }

        return *this;
    }
    ~ZavrsniRad()
    {
        delete[] _tema;
        _tema = nullptr;
    }

    // getters
    char *GetNazivTeme() const { return _tema; }
    vector<Poglavlje> &GetPoglavlja() { return _poglavljaRada; };
    string GetDatumOdbrane() const { return _datumOdbrane; }
    float GetOcjena() const { return _konacnaOcjena; }

    // setters
    void SetDatumOdbrane(string datumOdbrane)
    {
        _datumOdbrane = datumOdbrane;
    }
    void SetKonacnaOcjena(float konacnaOcjena)
    {
        _konacnaOcjena = konacnaOcjena;
    }

    void DodajPoglavlje(const char *naziv, const char *sadrzaj)
    {
        if (_poglavljaRada.size() == 0)
        {
            _poglavljaRada.push_back(Poglavlje(naziv, sadrzaj));
            return;
        }

        for (int i = 0; i < _poglavljaRada.size(); i++)
        {
            if (strcmp(_poglavljaRada[i].GetNaslov(), naziv) == 0)
            {
                _poglavljaRada[i].SetSadrzaj(sadrzaj);
            }
            else
            {
                _poglavljaRada.push_back(Poglavlje(naziv, sadrzaj));
                return;
            }
        }
    }

    void OcijeniPoglavlje(const char *naziv, int ocjena)
    {
        bool flagged = false;

        for (int i = 0; i < _poglavljaRada.size(); i++)
        {
            if (strcmp(_poglavljaRada[i].GetNaslov(), naziv) == 0)
            {
                _poglavljaRada[i].SetOcjena(ocjena);

                if (ocjena > 6 && ocjena < 10)
                {
                    _poglavljaRada[i].SetPrihvaceno(true);
                    return;
                }
                else
                {
                    throw std::runtime_error("Ocjena nije pozitivna!");
                }
            }
            else
            {
                flagged = true;
            }
        }

        if (flagged)
        {
            throw std::runtime_error("Poglavlje nepostojece!");
        }
    }

    friend ostream &operator<<(ostream &COUT, ZavrsniRad &obj)
    {
        COUT << "Tema rada: " << obj._tema << endl;
        COUT << "Sadrzaj: " << endl;
        for (size_t i = 0; i < obj._poglavljaRada.size(); i++)
            COUT << obj._poglavljaRada[i] << endl;
        COUT << "Datum odbrane rada: " << obj._datumOdbrane << endl
             << " Ocjena: " << obj._konacnaOcjena << endl;
        return COUT;
    }
};

bool operator==(const ZavrsniRad &obj1, const ZavrsniRad &obj2)
{
    bool temp = true;

    if (strcmp(obj1.GetNazivTeme(), obj2.GetNazivTeme()) != 0)
    {
        temp = false;
    }
    if (obj1.GetDatumOdbrane() != obj2.GetDatumOdbrane())
    {
        temp = false;
    }
    if (obj1.GetOcjena() != obj2.GetOcjena())
    {
        temp = false;
    }
    if (const_cast<ZavrsniRad &>(obj1).GetPoglavlja().size() != const_cast<ZavrsniRad &>(obj2).GetPoglavlja().size())
    {
        temp = false;
    }

    for (int i = 0; i < const_cast<ZavrsniRad &>(obj1).GetPoglavlja().size(); i++)
    {
        if (const_cast<ZavrsniRad &>(obj1).GetPoglavlja()[i] != const_cast<ZavrsniRad &>(obj2).GetPoglavlja()[i])
        {
            temp = false;
        }
    }

    return temp;
}

bool operator!=(const ZavrsniRad &obj1, const ZavrsniRad &obj2)
{
    return !(obj1 == obj2);
}

class Osoba
{
protected:
    string _imePrezime;

public:
    Osoba()
    {
        _imePrezime = not_set;
    }
    Osoba(string imePrezime) : _imePrezime(imePrezime) {}
    Osoba(const Osoba &obj)
    {
        _imePrezime = obj.GetImePrezime();
    }
    Osoba(Osoba &&obj) noexcept : _imePrezime(move(obj._imePrezime)) {}
    Osoba &operator=(const Osoba &obj)
    {
        if (this != &obj)
        {
            _imePrezime = obj.GetImePrezime();
        }

        return *this;
    }
    virtual ~Osoba() {}
    string GetImePrezime() const { return _imePrezime; }
    virtual void Info() = 0;
};

bool operator==(const Osoba &obj1, const Osoba &obj2)
{
    return obj1.GetImePrezime() == obj2.GetImePrezime();
}

bool operator!=(const Osoba &obj1, const Osoba &obj2)
{
    return !(obj1 == obj2);
}

mutex mutko;

class Nastavnik : public Osoba
{
    // Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
    Kolekcija<string, ZavrsniRad> _teme;

public:
    virtual void Info() override
    {
        cout << this->GetImePrezime();
        for (int i = 0; i < _teme.getTrenutno(); i++)
        {
            cout << "IB: " << _teme.getElement1(i) << " " << _teme.getElement2(i) << endl;
        }
        cout << endl;
    }
    Nastavnik() : Osoba() {}
    Nastavnik(const char *imePrezime) : Osoba(imePrezime) {}
    Nastavnik(const Nastavnik &obj) : Osoba(obj), _teme(obj._teme) {}
    Nastavnik(Nastavnik &&obj) noexcept : Osoba(move(obj)), _teme(move(obj._teme)) {}
    Nastavnik &operator=(const Nastavnik &obj)
    {
        if (this != &obj)
        {
            static_cast<Osoba &>(*this) = obj;
            _teme = obj._teme;
        }

        return *this;
    }
    virtual ~Nastavnik() override {}
    Kolekcija<string, ZavrsniRad> &GetTeme() { return _teme; };

    bool DodajZavrsniRad(string indeks, ZavrsniRad rad)
    {
        bool temp = false;
        if (_teme.getTrenutno() == 0)
        {
            _teme.DodajElemente(indeks, rad);
            temp = true;
        }

        auto provjeriIsteRadove = [this, &rad, &indeks]()
        {
            bool flag = true;
            for (int i = 0; i < _teme.getTrenutno(); i++)
            {
                if (strcmp(_teme.getElement2(i).GetNazivTeme(), rad.GetNazivTeme()))
                {
                    flag = false;
                }
                if (_teme.getElement1(i) == indeks)
                {
                    flag = false;
                }
            }
            return flag;
        };

        if (provjeriIsteRadove())
        {
            _teme.DodajElemente(indeks, rad);
            temp = true;
        }

        return temp;
    }

    ZavrsniRad *ZakaziOdbranuRada(string indeks, string datumOdbrane)
    {
        float prosjek = 0.0f;
        ZavrsniRad *novi = nullptr;

        for (int i = 0; i < _teme.getTrenutno(); i++)
        {
            if (_teme.getElement1(i) == indeks)
            {
                if (_teme.getElement2(i).GetPoglavlja().size() > min_polgavlja)
                {
                    for (int j = 0; j < _teme.getElement2(i).GetPoglavlja().size(); j++)
                    {
                        if (strlen(_teme.getElement2(i).GetPoglavlja()[j].GetSadrzaj()) > min_karaktera_po_poglavlju)
                        {
                            if (_teme.getElement2(i).GetPoglavlja()[j].GetPrihvaceno() == true)
                            {
                                prosjek += _teme.getElement2(i).GetPoglavlja()[j].GetOcjena();
                                _teme.getElement2(i).SetDatumOdbrane(datumOdbrane);
                                novi = &_teme.getElement2(i);
                            }
                        }
                    }
                    prosjek /= _teme.getElement2(i).GetPoglavlja().size();
                    _teme.getElement2(i).SetKonacnaOcjena(prosjek);
                }
            }
        }
        return novi;
    }

    string posaljiMail(string indeks, float ocjena)
    {
        lock_guard<mutex> lock(mutko); // automatski mutex management lock/unlock
        string mail;
        mail = "Postovani " + indeks + " , uzimajuci u obzir cinjenicu da ste kod mentora" + this->GetImePrezime() + " uspjesno odbranili rad sa ocjenom " + to_string(ocjena) + "  cast nam je pozvati vas na dodjelu nagrada za najbolje studente koja ce se odrzati  na	FIT - u 03.07.2019.godine.";
        return mail;
    }

    friend ostream &operator<<(ostream &COUT, const Nastavnik &obj)
    {
        COUT << obj.GetImePrezime() << endl;
        COUT << "Teme: " << endl;
        COUT << obj._teme << endl;
        return COUT;
    }
};

string PosaljiPozivZaDodjeluNagrada(Nastavnik **nastavnici, int max, float granica)
{
    string poruka = not_set;

    for (int i = 0; i < max; i++)
    {
        for (int j = 0; j < nastavnici[i]->GetTeme().getTrenutno(); j++)
        {
            if (nastavnici[i]->GetTeme().getElement2(j).GetOcjena() > granica)
            {
                stringstream upis;
                auto poziv = [&i, &j, &nastavnici, &upis]()
                {
                    upis << nastavnici[i]->posaljiMail(nastavnici[i]->GetTeme().getElement1(j), nastavnici[i]->GetTeme().getElement2(j).GetOcjena());
                    return upis.str();
                };
                thread t(poziv);
                poruka = poziv();
            }
        }
    }
    return poruka;
}

int main()
{
    cout << crt << "UPLOAD RADA OBAVEZNO IZVRSITI U ODGOVARAJUCI FOLDER NA FTP SERVERU" << endl;
    cout << "U slucaju da je Upload folder prazan pritisnite tipku F5" << crt;

    const int max = 4;
    Nastavnik *nastavnici[max];

    nastavnici[0] = new Nastavnik("Denis Music");
    nastavnici[1] = new Nastavnik("Zanin Vejzovic");
    nastavnici[2] = new Nastavnik("Jasmin Azemovic");
    nastavnici[3] = new Nastavnik("Emina Junuz");
    // parametri: naziv zavrsnog rada
    ZavrsniRad multimedijalni("Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
    ZavrsniRad podrsa_operaterima("Sistem za podršku rada kablovskog operatera");
    ZavrsniRad analiza_sigurnosti("Prakticna analiza sigurnosti bežičnih računarskih mreža");
    ZavrsniRad kriptografija("Primjena teorije informacija u procesu generisanja kriptografskih ključeva");

    /*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu, funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci (izmedju postojeceg i novog sadrzaja se dodaje prazan prostor). u slucaju da poglavlje ne postoji, ono se dodaje zajedno sa sadrzaje*/
    // parametri: nazivPoglavlja, sadrzajPoglavlja
    multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
    multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
    multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
    multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
    multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");
    multimedijalni.DodajPoglavlje("Zakljucak", "U ovom radu su predstavljeni osnovni koncepti i prakticna primjena...");

    try
    {
        /*funkcija OcijeniPoglavlje, na osnovu naziva poglavlja, dodjeljuje ocjenu poglavlju te ukoliko je ocjena pozitivna (6 - 10) onda poglavlje oznacava prihvacenim. U slucaju da ocjena nije validna ili poglavlje ne postoji, funkcija baca izuzetak sa odgovarajucom porukom*/
        // parametri:nazivPoglavlja, ocjena

        multimedijalni.OcijeniPoglavlje("Uvod", 8);
        multimedijalni.OcijeniPoglavlje("Vrste multimedijalnih sistema", 8);
        multimedijalni.OcijeniPoglavlje("Teorija multimedije", 9);
        multimedijalni.OcijeniPoglavlje("Zakljucak", 7);
        multimedijalni.OcijeniPoglavlje("Naziv poglavlja ne postoji", 8);
    }
    catch (exception &err)
    {
        cout << "Greska -> " << err.what() << endl;
    }

    /*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. zavrsni rad se dodaje studentu sa brojem indeksa proslijedjenim kao prvi parametar.
    sprijeciti dodavanje zavrsnih radova sa istom temom*/
    // parametri: brojIndeksa, zavrsniRad
    if (nastavnici[0]->DodajZavrsniRad("IB130011", multimedijalni))
        cout << "Zavrsni rad uspjesno dodat!" << endl;
    if (nastavnici[0]->DodajZavrsniRad("IB120051", podrsa_operaterima))
        cout << "Zavrsni rad uspjesno dodat!" << endl;
    if (!nastavnici[0]->DodajZavrsniRad("IB120056", podrsa_operaterima)) // dupliranje rada
        cout << "Zavrsni rad nije dodat!" << endl;
    if (!nastavnici[0]->DodajZavrsniRad("IB120051", kriptografija)) // studentu vec dodijeljen rad
        cout << "Zavrsni rad nije dodat!" << endl;
    if (nastavnici[1]->DodajZavrsniRad("IB140102", analiza_sigurnosti))
        cout << "Zavrsni rad uspjesno dodat!" << endl;
    if (nastavnici[2]->DodajZavrsniRad("IB140002", kriptografija))
        cout << "Zavrsni rad uspjesno dodat!" << endl;

    /*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva definisanje
    datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio i pri tom su zadovoljeni sljedeci uslovi:
        1. zavrsni rad ima broj poglavlja veci od minimalnog
        2. svako poglavlje ima broj karaktera veci od minimalnog
        3. svako poglavlje je prihvaceno/odobreno
    ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazan odbrana.
    u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
*/

    // parametri: brojIndeksa, datumOdbrane
    ZavrsniRad *zr1 = nastavnici[0]->ZakaziOdbranuRada("IB130011", "25.09.2018");
    if (zr1 != nullptr)
        cout << *zr1 << endl;

    zr1 = nastavnici[0]->ZakaziOdbranuRada("IB130111", "25.09.2018"); // student sa brojem indeksa IB130111 jos uvijek nije prijavio rad
    if (zr1 != nullptr)
        cout << *zr1 << endl;

    // ispisuje sve podatke o nastavniku i njegovim mentorstvima
    nastavnici[0]->Info();

    /*Funkcija PosaljiPozivZaDodjeluNagrada ima zadatak da svim studentima koji su uspjesno okoncali svoj zavrsni rad kod odredjenog nastavnika/mentora
    i tom prilikom ostvarili ocjenu vecu od proslijedjene, u zasebnom thread.u, posalje email poruku (mail adresa: brojIndeksa@edu.fit.ba) sa sadrzajem:
    "Postovani {brojIndeksa}, uzimajuci u obzir cinjenicu da ste kod mentora {imePrezimeMentora} uspjesno odbranili rad sa ocjenom {ocjena} cast nam je pozvati vas na dodjelu nagrada za najbolje studente koja ce se odrzatu u na FIT-u 03.07.2019. godine."	.
    funkcija treba da vrati sadrzaj svih poslatih email poruka, a ukoliko niti jedan od nastavnika ne posjeduje evidentirano mentorstvo na zavrsnom radu, funkcija vraca not_set*/
    cout << "Studenti za dodjelu nagrada: " << PosaljiPozivZaDodjeluNagrada(nastavnici, max, 9.5) << endl;

    for (int i = 0; i < max; i++)
    {
        delete nastavnici[i];
        nastavnici[i] = nullptr;
    }
    system("pause>0");
    return 0;
}
