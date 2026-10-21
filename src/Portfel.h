#ifndef PORTFEL_H
#define PORTFEL_H

#include "Obserwator.h"
#include <map>
#include <string>

// Klasa reprezentująca portfel inwestora.
// Implementuje interfejs Obserwator, reagując na zmiany cen rynkowych i aktualizując wycenę NAV.
class Portfel : public Obserwator {
private:
    double wolnaGotowka;                              // Gotówka wolna do inwestycji (w zł)
    std::map<std::string, double> aktywa;             // Mapa symbol -> aktualna posiadana ilość
    std::map<std::string, double> srednieCenyZakupu;  // Mapa symbol -> średnia cena ważona zakupu
    std::map<std::string, double> cenyAktywow;        // Mapa symbol -> ostatnia znana cena rynkowa (do wyceny)
    double wartoscPortfela;                           // Łączna wartość NAV (aktywa + gotówka)

    // Recalculates total portfolio value (NAV) based on held assets and their current prices
    void przeliczWartoscPortfela();

public:
    explicit Portfel(double gotowka = 0.0);

    void aktualizujStan(const std::string& symbol, double nowaCena) override;

    double pobierzWolnaGotowka() const;
    double pobierzWartoscPortfela() const;
    double pobierzIloscAktywa(const std::string& symbol) const;
    double pobierzSredniaCeneZakupu(const std::string& symbol) const;
    const std::map<std::string, double>& pobierzAktywa() const;

    void dodajGotowke(double kwota);
    void odejmijGotowke(double kwota);

    void dodajAktywo(const std::string& symbol, double ilosc, double cenaZakupu);
    void odejmijAktywo(const std::string& symbol, double ilosc);
    void usunAktywo(const std::string& symbol);

    void recznieUstawWyceneCeny(const std::string& symbol, double cena);
    double pobierzWyceneAktywa(const std::string& symbol) const;
};

#endif
