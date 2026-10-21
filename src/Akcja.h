#ifndef AKCJA_H
#define AKCJA_H

#include "InstrumentFinansowy.h"

// Klasa reprezentująca akcję spółki giełdowej.
// Dziedziczy po InstrumentFinansowy i implementuje jej metody wirtualne.
class Akcja : public InstrumentFinansowy {
public:
  Akcja(std::string sym, std::string name, double startPrice)
      : InstrumentFinansowy(std::move(sym), std::move(name), startPrice) {}

  // Aktualizuje cenę akcji na podstawie umiarkowanego szumu losowego (-1.5% do +1.5%)
  void aktualizujCeneDynamicznie() override;
  // Zwraca typ instrumentu "Akcja"
  std::string pobierzTyp() const override;
};

#endif
