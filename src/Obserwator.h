#ifndef OBSERWATOR_H
#define OBSERWATOR_H

#include <string>
#include <vector>
#include <memory>

// Interfejs Obserwatora (wzorzec projektowy Obserwator)
// Klasy implementujące ten interfejs reagują na zmiany stanu PodmiotuObserwowanego.
class Obserwator {
public:
    virtual ~Obserwator() = default;
      // Metoda wywoływana, gdy podmiot obserwowany zmienia cenę instrumentu
    virtual void aktualizujStan(const std::string& symbol, double nowaCena) = 0;
};

// Klasa bazowa dla podmiotu obserwowanego (Subject)
// Przechowuje listę obserwatorów i powiadamia ich o zmianach.
class PodmiotObserwowany {
protected:
    std::vector<std::shared_ptr<Obserwator>> obserwatorzy; // Zarejestrowani obserwatorzy
public:
    virtual ~PodmiotObserwowany() = default;
      // Rejestruje nowego obserwatora w systemie
    void zarejestrujObserwatora(std::shared_ptr<Obserwator> o);
      // Powiadamia wszystkich zarejestrowanych obserwatorów o zmianie ceny instrumentu
    void powiadomObserwatorow(const std::string& symbol, double nowaCena);
};

#endif
