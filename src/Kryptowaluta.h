#ifndef KRYPTOWALUTA_H
#define KRYPTOWALUTA_H

#include "InstrumentFinansowy.h"

// Klasa reprezentująca kryptowalutę.
// Cechuje się znacznie większą zmiennością cenową (wysoki poziom ryzyka).
class Kryptowaluta : public InstrumentFinansowy {
public:
    Kryptowaluta(std::string sym, std::string name, double startPrice)
        : InstrumentFinansowy(std::move(sym), std::move(name), startPrice) {}

    // Aktualizuje cenę kryptowaluty na podstawie dużego szumu losowego (-8.0% do +8.0%)
    void aktualizujCeneDynamicznie() override;
      // Zwraca typ instrumentu "Kryptowaluta"
    std::string pobierzTyp() const override;
};

#endif
