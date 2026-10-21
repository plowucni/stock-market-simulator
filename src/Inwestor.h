#ifndef INWESTOR_H
#define INWESTOR_H

#include "Portfel.h"
#include <string>

// Klasa reprezentująca inwestora uczestniczącego w obrocie giełdowym.
class Inwestor {
private:
    std::string idInwestora; // Unikalny identyfikator inwestora (np. "GRACZ")
    Portfel portfel;         // Posiadany portfel aktywów finansowych

public:
    Inwestor(std::string id, double gotowkaStartowa)
        : idInwestora(std::move(id)), portfel(gotowkaStartowa) {}

    const std::string& pobierzId() const;
    Portfel& pobierzPortfel();
    const Portfel& pobierzPortfel() const;
};

#endif
