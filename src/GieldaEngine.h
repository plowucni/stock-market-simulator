#ifndef GIELDA_ENGINE_H
#define GIELDA_ENGINE_H

#include "Obserwator.h"
#include "InstrumentFinansowy.h"
#include "Inwestor.h"

#include <map>
#include <vector>
#include <memory>

// Główna klasa silnika giełdowego (Engine) koordynująca całą symulację.
// Dziedziczy po PodmiotObserwowany (Subject), by móc wysyłać powiadomienia o cenach.
class Gielda : public PodmiotObserwowany {
private:
    // Lista i mapa instrumentów dostępnych na giełdzie
    std::vector<std::shared_ptr<InstrumentFinansowy>> instrumenty;
    std::map<std::string, std::shared_ptr<InstrumentFinansowy>> mapaInstrumentow;
      // Lista i mapa inwestorów (kont) zarejestrowanych w systemie
    std::vector<std::shared_ptr<Inwestor>> inwestorzy;
    std::map<std::string, std::shared_ptr<Inwestor>> mapaInwestorow;

    uint64_t turaZegara; // Licznik aktualnej tury (kroku) symulacji

public:
    Gielda();

    void dodajInstrument(std::shared_ptr<InstrumentFinansowy> inst);
    void dodajInwestora(std::shared_ptr<Inwestor> inv);

    void kupNatychmiast(const std::string& idInwestora, const std::string& symbol, double ilosc);
    void sprzedajNatychmiast(const std::string& idInwestora, const std::string& symbol, double ilosc);

    void wykonajKrokSymulacji();

    uint64_t pobierzTureZegara() const;
    const std::vector<std::shared_ptr<InstrumentFinansowy>>& pobierzInstrumenty() const;
    std::shared_ptr<InstrumentFinansowy> pobierzInstrument(const std::string& symbol) const;
    std::shared_ptr<Inwestor> pobierzInwestora(const std::string& idInwestora) const;

    void zapiszPortfele(const std::string& sciezka = "konta.txt") const;
    void wczytajPortfele(const std::string& sciezka = "konta.txt");
    void logujTransakcje(const std::string& kupiec, const std::string& sprzedawca, const std::string& symbol, double ilosc, double cena) const;
};

#endif
