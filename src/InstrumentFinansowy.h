#ifndef INSTRUMENT_FINANSOWY_H
#define INSTRUMENT_FINANSOWY_H

#include <string>
#include <cstdint>

// Klasa abstrakcyjna reprezentująca instrument finansowy.
// Stanowi bazę dla polimorfizmu (dziedziczenie: Akcja, Obligacja, Kryptowaluta).
class InstrumentFinansowy {
protected:
    std::string symbol;         // Ticker instrumentu (np. AAPL, BTC)
    std::string nazwa;          // Pełna nazwa instrumentu
    double cenaAktualna;        // Aktualna cena rynkowa
    double cenaOtwarciaSesji;   // Cena na początku danej tury (sesji)
    uint64_t wolumenObrotu;     // Wolumen obrotu w bieżącej turze

public:
    InstrumentFinansowy(std::string sym, std::string name, double startPrice)
        : symbol(std::move(sym)), nazwa(std::move(name)), cenaAktualna(startPrice),
          cenaOtwarciaSesji(startPrice), wolumenObrotu(0) {}

    virtual ~InstrumentFinansowy() = default;

    // Metoda czysto wirtualna do dynamicznej aktualizacji cen (polimorfizm)
    virtual void aktualizujCeneDynamicznie() = 0;
      // Zwraca typ instrumentu jako string (do celów identyfikacji i wyświetlania)
    virtual std::string pobierzTyp() const = 0;

    const std::string& pobierzSymbol() const;
    const std::string& pobierzNazwe() const;
    double pobierzCeneAktualna() const;
    double pobierzCeneOtwarciaSesji() const;
    uint64_t pobierzWolumenObrotu() const;

    void ustawCeneAktualna(double cena);
    void ustawCeneOtwarciaSesji(double cena);
    void dodajWolumen(uint64_t wolumen);
    void resetWolumenSesji();
};

#endif
