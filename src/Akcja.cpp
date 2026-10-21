#include "Akcja.h"
#include <random>

void Akcja::aktualizujCeneDynamicznie() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-0.015, 0.015);
    double szumLosowy = dis(gen);

    double nowaCena = cenaAktualna * (1.0 + szumLosowy);
    ustawCeneAktualna(nowaCena);
}

std::string Akcja::pobierzTyp() const {
    return "Akcja";
}

