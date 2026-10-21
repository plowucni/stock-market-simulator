#include "GieldaEngine.h"
#include "GieldaException.h"
#include "Obligacja.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

Gielda::Gielda() : turaZegara(1) {}

void Gielda::dodajInstrument(std::shared_ptr<InstrumentFinansowy> inst) {
    if (inst) {
        instrumenty.push_back(inst);
        mapaInstrumentow[inst->pobierzSymbol()] = inst;
    }
}

void Gielda::dodajInwestora(std::shared_ptr<Inwestor> inv) {
    if (inv) {
        inwestorzy.push_back(inv);
        mapaInwestorow[inv->pobierzId()] = inv;
              auto portfelObs = std::shared_ptr<Obserwator>(inv, &(inv->pobierzPortfel()));
        zarejestrujObserwatora(portfelObs);
              for (const auto& inst : instrumenty) {
            inv->pobierzPortfel().recznieUstawWyceneCeny(inst->pobierzSymbol(), inst->pobierzCeneAktualna());
        }
    }
}

void Gielda::kupNatychmiast(const std::string& idInwestora, const std::string& symbol, double ilosc) {
    auto instIt = mapaInstrumentow.find(symbol);
    if (instIt == mapaInstrumentow.end()) {
        throw BladNieznanyInstrument(symbol);
    }
    auto inst = instIt->second;

    auto invIt = mapaInwestorow.find(idInwestora);
    if (invIt == mapaInwestorow.end()) {
        throw BladGieldy("ERR-007", "Nieznany inwestor: " + idInwestora);
    }
    auto inv = invIt->second;

    if (ilosc <= 0) {
        throw BladGieldy("ERR-008", "Ilosc musi byc dodatnia.");
    }

    double cena = inst->pobierzCeneAktualna();
    double koszt = ilosc * cena;

    if (inv->pobierzPortfel().pobierzWolnaGotowka() < koszt) {
        throw BladBrakSrodkow(idInwestora + " | koszt: " + std::to_string(koszt) + ", wolne: " + std::to_string(inv->pobierzPortfel().pobierzWolnaGotowka()));
    }

    inv->pobierzPortfel().odejmijGotowke(koszt);
    inv->pobierzPortfel().dodajAktywo(symbol, ilosc, cena);
    inst->dodajWolumen(static_cast<uint64_t>(ilosc));

    logujTransakcje(idInwestora, "RYNEK", symbol, ilosc, cena);
    std::cout << ">>> [KUPNO] " << symbol << ": " << ilosc << " szt. po " << std::fixed << std::setprecision(2) << cena << " zl (razem: " << koszt << " zl)\n";
}

void Gielda::sprzedajNatychmiast(const std::string& idInwestora, const std::string& symbol, double ilosc) {
    auto instIt = mapaInstrumentow.find(symbol);
    if (instIt == mapaInstrumentow.end()) {
        throw BladNieznanyInstrument(symbol);
    }
    auto inst = instIt->second;

    auto invIt = mapaInwestorow.find(idInwestora);
    if (invIt == mapaInwestorow.end()) {
        throw BladGieldy("ERR-007", "Nieznany inwestor: " + idInwestora);
    }
    auto inv = invIt->second;

    if (ilosc <= 0) {
        throw BladGieldy("ERR-008", "Ilosc musi byc dodatnia.");
    }

    if (inv->pobierzPortfel().pobierzIloscAktywa(symbol) < ilosc) {
        throw BladBrakAktywa(idInwestora + " | wymagane: " + std::to_string(ilosc) + ", posiadane: " + std::to_string(inv->pobierzPortfel().pobierzIloscAktywa(symbol)));
    }

    double cena = inst->pobierzCeneAktualna();
    double przychod = ilosc * cena;

    inv->pobierzPortfel().odejmijAktywo(symbol, ilosc);
    inv->pobierzPortfel().dodajGotowke(przychod);
    inst->dodajWolumen(static_cast<uint64_t>(ilosc));

    logujTransakcje("RYNEK", idInwestora, symbol, ilosc, cena);
    std::cout << ">>> [SPRZEDAZ] " << symbol << ": " << ilosc << " szt. po " << std::fixed << std::setprecision(2) << cena << " zl (przychod: " << przychod << " zl)\n";
}

void Gielda::wykonajKrokSymulacji() {
    for (auto& inst : instrumenty) {
        inst->ustawCeneOtwarciaSesji(inst->pobierzCeneAktualna());
        inst->resetWolumenSesji();
        inst->aktualizujCeneDynamicznie();
        powiadomObserwatorow(inst->pobierzSymbol(), inst->pobierzCeneAktualna());
    }

    std::vector<std::string> obligacjeDoWykupu;
    for (auto& inst : instrumenty) {
        if (inst->pobierzTyp() == "Obligacja") {
            auto oblig = std::dynamic_pointer_cast<Obligacja>(inst);
            if (oblig) {
                if (turaZegara > 0 && turaZegara % 5 == 0) {
                    for (auto& inv : inwestorzy) {
                        double qty = inv->pobierzPortfel().pobierzIloscAktywa(oblig->pobierzSymbol());
                        if (qty > 0) {
                            double kwotaKuponu = qty * oblig->pobierzCeneNominalna() * oblig->pobierzOprocentowanieStale();
                            inv->pobierzPortfel().dodajGotowke(kwotaKuponu);
                            std::cout << ">>> [WYPLATA KUPONU] Inwestor " << inv->pobierzId() << " otrzymal " << std::fixed << std::setprecision(2) << kwotaKuponu << " zl odsetek z obligacji " << oblig->pobierzSymbol() << "\n";
                        }
                    }
                }

                oblig->zmniejszTerminWykupu();
                if (oblig->czyWykupiona()) {
                    obligacjeDoWykupu.push_back(oblig->pobierzSymbol());
                }
            }
        }
    }

    bool juzWyplaconyKupon = (turaZegara > 0 && turaZegara % 5 == 0);
    for (const auto& tick : obligacjeDoWykupu) {
        auto oblig = std::dynamic_pointer_cast<Obligacja>(mapaInstrumentow[tick]);
        if (!oblig) continue;

        std::cout << "\n>>> [WYKUP NOMINALNY] Obligacja " << tick << " wygasla. Nastepuje rozliczenie.\n";

        for (auto& inv : inwestorzy) {
            double qty = inv->pobierzPortfel().pobierzIloscAktywa(tick);
            if (qty > 0) {
                double wyplata = qty * oblig->pobierzCeneNominalna();
                double kwotaKuponu = 0.0;
                if (oblig->pobierzPoczatkowyTerminWykupu() % 5 == 0 && !juzWyplaconyKupon) {
                    kwotaKuponu = qty * oblig->pobierzCeneNominalna() * oblig->pobierzOprocentowanieStale();
                }
                inv->pobierzPortfel().usunAktywo(tick);
                inv->pobierzPortfel().dodajGotowke(wyplata + kwotaKuponu);
                std::cout << "  * Wykupiono " << qty << " szt. od " << inv->pobierzId() << " na kwote " << std::fixed << std::setprecision(2) << wyplata << " zl";
                if (kwotaKuponu > 0) {
                    std::cout << " + koncowy kupon " << kwotaKuponu << " zl";
                }
                std::cout << "\n";
            }
        }

        instrumenty.erase(
            std::remove(instrumenty.begin(), instrumenty.end(), oblig),
            instrumenty.end()
        );
        mapaInstrumentow.erase(tick);
    }

    zapiszPortfele();
    turaZegara++;
    std::cout << ">>> [SYSTEM] Zakonczono ture " << (turaZegara - 1) << ". Rozpoczyna sie tura " << turaZegara << ".\n";
}

uint64_t Gielda::pobierzTureZegara() const {
    return turaZegara;
}

const std::vector<std::shared_ptr<InstrumentFinansowy>>& Gielda::pobierzInstrumenty() const {
    return instrumenty;
}

std::shared_ptr<InstrumentFinansowy> Gielda::pobierzInstrument(const std::string& symbol) const {
    auto it = mapaInstrumentow.find(symbol);
    if (it != mapaInstrumentow.end()) {
        return it->second;
    }
    throw BladNieznanyInstrument(symbol);
}

std::shared_ptr<Inwestor> Gielda::pobierzInwestora(const std::string& idInwestora) const {
    auto it = mapaInwestorow.find(idInwestora);
    return (it != mapaInwestorow.end()) ? it->second : nullptr;
}

void Gielda::zapiszPortfele(const std::string& sciezka) const {
    std::ofstream plik(sciezka);
    if (!plik.is_open()) {
        throw BladGieldy("ERR-014", "Nie udalo sie otworzyc pliku do zapisu: " + sciezka);
    }

    for (const auto& inv : inwestorzy) {
        const auto& p = inv->pobierzPortfel();
        plik << inv->pobierzId() << ";"
             << std::fixed << std::setprecision(2) << p.pobierzWolnaGotowka() << ";";

        bool first = true;
        for (const auto& [sym, qty] : p.pobierzAktywa()) {
            if (qty > 0) {
                if (!first) plik << ",";
                plik << sym << ":" << std::fixed << std::setprecision(8) << qty;
                first = false;
            }
        }
        plik << "\n";
    }
}

void Gielda::wczytajPortfele(const std::string& sciezka) {
    std::ifstream plik(sciezka);
    if (!plik.is_open()) {
        throw BladBrakPlikuKont(sciezka);
    }

    inwestorzy.clear();
    mapaInwestorow.clear();
    obserwatorzy.clear();

    std::string linia;
    int numerLinii = 0;
    while (std::getline(plik, linia)) {
        numerLinii++;
        if (linia.empty()) continue;

        std::stringstream ss(linia);
        std::string id, wolnaGotStr, aktywaStr;

        if (!std::getline(ss, id, ';') ||
            !std::getline(ss, wolnaGotStr, ';')) {
            throw BladFormatowaniaBazy("Linia " + std::to_string(numerLinii));
        }

        double wolnaGot = 0.0;
        try {
            wolnaGot = std::stod(wolnaGotStr);
        } catch (...) {
            throw BladFormatowaniaBazy("Gotowka w linii " + std::to_string(numerLinii));
        }

        auto inv = std::make_shared<Inwestor>(id, wolnaGot);

        std::getline(ss, aktywaStr);
        if (!aktywaStr.empty()) {
            std::stringstream ass(aktywaStr);
            std::string para;
            while (std::getline(ass, para, ',')) {
                if (para.empty()) continue;
                size_t dwukropek = para.find(':');
                if (dwukropek == std::string::npos) {
                    throw BladFormatowaniaBazy("Zly format aktywa w linii " + std::to_string(numerLinii));
                }
                std::string sym = para.substr(0, dwukropek);
                std::string iloscStr = para.substr(dwukropek + 1);
                double ilosc = 0.0;
                try {
                    ilosc = std::stod(iloscStr);
                } catch (...) {
                    throw BladFormatowaniaBazy("Ilosc aktywa w linii " + std::to_string(numerLinii));
                }

                double cenaBiezaca = 100.0;
                auto it = mapaInstrumentow.find(sym);
                if (it != mapaInstrumentow.end()) {
                    cenaBiezaca = it->second->pobierzCeneAktualna();
                }
                inv->pobierzPortfel().recznieUstawWyceneCeny(sym, cenaBiezaca);
                inv->pobierzPortfel().dodajAktywo(sym, ilosc, cenaBiezaca);
            }
        }

        dodajInwestora(inv);
    }
}

void Gielda::logujTransakcje(const std::string& kupiec, const std::string& sprzedawca, const std::string& symbol, double ilosc, double cena) const {
    std::ofstream plik("historia.txt", std::ios::app);
    if (plik.is_open()) {
        plik << turaZegara << ";"
             << kupiec << ";"
             << sprzedawca << ";"
             << symbol << ";"
             << std::fixed << std::setprecision(8) << ilosc << ";"
             << std::fixed << std::setprecision(2) << cena << ";"
             << std::fixed << std::setprecision(2) << (ilosc * cena) << "\n";
    }
}
