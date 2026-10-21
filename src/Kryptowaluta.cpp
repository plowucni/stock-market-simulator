#include "Kryptowaluta.h"
#include <random>

void Kryptowaluta::aktualizujCeneDynamicznie() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-0.08, 0.08);
    double szumLosowy = dis(gen);

    double nowaCena = cenaAktualna * (1.0 + szumLosowy);
    ustawCeneAktualna(nowaCena);
}

std::string Kryptowaluta::pobierzTyp() const {
    return "Kryptowaluta";
}
