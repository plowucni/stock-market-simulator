#include "Portfel.h"
#include "GieldaException.h"

Portfel::Portfel(double gotowka)
    : wolnaGotowka(gotowka), wartoscPortfela(gotowka) {}

void Portfel::przeliczWartoscPortfela() {
    double suma = wolnaGotowka;
    for (const auto& [sym, qty] : aktywa) {
        double cena = 0.0;
        auto it = cenyAktywow.find(sym);
        if (it != cenyAktywow.end()) {
            cena = it->second;
        }
        suma += qty * cena;
    }
    wartoscPortfela = suma;
}

void Portfel::aktualizujStan(const std::string& symbol, double nowaCena) {
    cenyAktywow[symbol] = nowaCena;
    przeliczWartoscPortfela();
}

double Portfel::pobierzWolnaGotowka() const {
    return wolnaGotowka;
}

double Portfel::pobierzWartoscPortfela() const {
    return wartoscPortfela;
}

double Portfel::pobierzIloscAktywa(const std::string& symbol) const {
    auto it = aktywa.find(symbol);
    return (it != aktywa.end()) ? it->second : 0.0;
}

double Portfel::pobierzSredniaCeneZakupu(const std::string& symbol) const {
    auto it = srednieCenyZakupu.find(symbol);
    return (it != srednieCenyZakupu.end()) ? it->second : 0.0;
}

const std::map<std::string, double>& Portfel::pobierzAktywa() const {
    return aktywa;
}

void Portfel::dodajGotowke(double kwota) {
    wolnaGotowka += kwota;
    przeliczWartoscPortfela();
}

void Portfel::odejmijGotowke(double kwota) {
    if (wolnaGotowka < kwota) {
        throw BladBrakSrodkow("Nie udalo sie potracic srodkow.");
    }
    wolnaGotowka -= kwota;
    przeliczWartoscPortfela();
}

void Portfel::dodajAktywo(const std::string& symbol, double ilosc, double cenaZakupu) {
    double obecnaIlosc = pobierzIloscAktywa(symbol);
    double obecnaSrednia = pobierzSredniaCeneZakupu(symbol);
    double nowaIlosc = obecnaIlosc + ilosc;

    if (nowaIlosc > 0) {
        srednieCenyZakupu[symbol] = (obecnaIlosc * obecnaSrednia + ilosc * cenaZakupu) / nowaIlosc;
    } else {
        srednieCenyZakupu[symbol] = 0.0;
    }
    aktywa[symbol] = nowaIlosc;
    przeliczWartoscPortfela();
}

void Portfel::odejmijAktywo(const std::string& symbol, double ilosc) {
    double obecnaIlosc = pobierzIloscAktywa(symbol);
    if (obecnaIlosc < ilosc) {
        throw BladBrakAktywa(symbol);
    }
    double nowaIlosc = obecnaIlosc - ilosc;
    if (nowaIlosc < 1e-9) {
        aktywa.erase(symbol);
        srednieCenyZakupu.erase(symbol);
        cenyAktywow.erase(symbol);
    } else {
        aktywa[symbol] = nowaIlosc;
    }
    przeliczWartoscPortfela();
}

void Portfel::usunAktywo(const std::string& symbol) {
    aktywa.erase(symbol);
    srednieCenyZakupu.erase(symbol);
    cenyAktywow.erase(symbol);
    przeliczWartoscPortfela();
}

void Portfel::recznieUstawWyceneCeny(const std::string& symbol, double cena) {
    cenyAktywow[symbol] = cena;
    przeliczWartoscPortfela();
}

double Portfel::pobierzWyceneAktywa(const std::string& symbol) const {
    auto it = cenyAktywow.find(symbol);
    return (it != cenyAktywow.end()) ? it->second : 0.0;
}
