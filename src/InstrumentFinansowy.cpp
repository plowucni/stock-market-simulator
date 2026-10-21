#include "InstrumentFinansowy.h"


const std::string& InstrumentFinansowy::pobierzSymbol() const {
    return symbol;
}

const std::string& InstrumentFinansowy::pobierzNazwe() const {
    return nazwa;
}

double InstrumentFinansowy::pobierzCeneAktualna() const {
    return cenaAktualna;
}

double InstrumentFinansowy::pobierzCeneOtwarciaSesji() const {
    return cenaOtwarciaSesji;
}

uint64_t InstrumentFinansowy::pobierzWolumenObrotu() const {
    return wolumenObrotu;
}

void InstrumentFinansowy::ustawCeneAktualna(double cena) {
    if (cena < 0.01) {
        cenaAktualna = 0.01;
    } else {
        cenaAktualna = cena;
    }
}

void InstrumentFinansowy::ustawCeneOtwarciaSesji(double cena) {
    cenaOtwarciaSesji = cena;
}

void InstrumentFinansowy::dodajWolumen(uint64_t wolumen) {
    wolumenObrotu += wolumen;
}

void InstrumentFinansowy::resetWolumenSesji() {
    wolumenObrotu = 0;
}
