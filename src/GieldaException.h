#ifndef GIELDA_EXCEPTION_H
#define GIELDA_EXCEPTION_H

#include <exception>
#include <string>
#include <fstream>

// Bazowa klasa wyjątków dla błędów giełdowych.
// Zapisuje wszystkie rzucone wyjątki do pliku logowania błędów "bledy.txt".
class BladGieldy : public std::exception {
protected:
    std::string kodBledu;   // Kod błędu (np. "ERR-001")
    std::string komunikat;  // Komunikat błędu dla użytkownika
public:
    BladGieldy(std::string kod, std::string msg) : kodBledu(std::move(kod)), komunikat(std::move(msg)) {
        std::ofstream plik("bledy.txt", std::ios::app);
        if (plik.is_open()) {
            plik << "Kod: " << kodBledu << " | Komunikat: " << komunikat << "\n";
        }
    }
    const char* what() const noexcept override { return komunikat.c_str(); }
    std::string pobierzKod() const { return kodBledu; }
};

// Błąd rzucany, gdy inwestor nie posiada wystarczających środków finansowych na zakup.
class BladBrakSrodkow : public BladGieldy {
public:
    explicit BladBrakSrodkow(const std::string& msg) 
        : BladGieldy("ERR-001", "Brak srodkow: " + msg) {}
};

// Błąd rzucany, gdy format pliku "konta.txt" jest uszkodzony lub nieprawidłowy.
class BladFormatowaniaBazy : public BladGieldy {
public:
    explicit BladFormatowaniaBazy(const std::string& msg) 
        : BladGieldy("ERR-002", "Blad deserializacji (zly format): " + msg) {}
};

// Błąd rzucany przy próbie operacji na instrumencie o nieznanym tickerze.
class BladNieznanyInstrument : public BladGieldy {
public:
    explicit BladNieznanyInstrument(const std::string& msg) 
        : BladGieldy("ERR-003", "Nieznany instrument: " + msg) {}
};

// Błąd rzucany, gdy plik portfeli (konta.txt) nie istnieje.
class BladBrakPlikuKont : public BladGieldy {
public:
    explicit BladBrakPlikuKont(const std::string& msg) 
        : BladGieldy("ERR-004", "Brak pliku konfiguracyjnego: " + msg) {}
};

// Błąd rzucany przy próbie sprzedaży instrumentu, którego inwestor nie posiada w odpowiedniej ilości.
class BladBrakAktywa : public BladGieldy {
public:
    explicit BladBrakAktywa(const std::string& msg) 
        : BladGieldy("ERR-005", "Brak wystarczajacej ilosci aktywa w portfelu: " + msg) {}
};

#endif
