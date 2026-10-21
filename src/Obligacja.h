#ifndef OBLIGACJA_H
#define OBLIGACJA_H

#include "InstrumentFinansowy.h"

// Klasa reprezentująca obligację skarbową/korporacyjną o stałym oprocentowaniu i określonym terminie wykupu.
class Obligacja : public InstrumentFinansowy {
private:
    double oprocentowanieStale; // Oprocentowanie obligacji (np. 0.05 oznacza 5% na okres kuponowy)
    uint32_t terminWykupu;      // Pozostała liczba tur do wykupu nominalnego (maturity)
    double cenaNominalna;       // Cena nominalna obligacji
    uint32_t poczatkowyTerminWykupu; // Początkowy termin zapadalności obligacji

public:
    Obligacja(std::string sym, std::string name, double startPrice, double rate, uint32_t maturity)
        : InstrumentFinansowy(std::move(sym), std::move(name), startPrice),
          oprocentowanieStale(rate), terminWykupu(maturity), cenaNominalna(startPrice), poczatkowyTerminWykupu(maturity) {}

    // Aktualizuje cenę obligacji na podstawie minimalnego szumu losowego (-0.1% do +0.1%)
    void aktualizujCeneDynamicznie() override;
      // Zwraca typ instrumentu "Obligacja"
    std::string pobierzTyp() const override;

    double pobierzOprocentowanieStale() const;
    double pobierzCeneNominalna() const;
    uint32_t pobierzPoczatkowyTerminWykupu() const;
      // Zmniejsza pozostały czas do wykupu o 1 turę
    void zmniejszTerminWykupu();
      // Sprawdza, czy termin wykupu obligacji minął (czy terminWykupu == 0)
    bool czyWykupiona() const;
};

#endif
