#include "Obligacja.h"
#include <random>

void Obligacja::aktualizujCeneDynamicznie() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-0.001, 0.001);
    double szumLosowy = dis(gen);

    double nowaCena = cenaAktualna * (1.0 + szumLosowy);
    ustawCeneAktualna(nowaCena);
}

std::string Obligacja::pobierzTyp() const {
    return "Obligacja";
}

double Obligacja::pobierzOprocentowanieStale() const {
    return oprocentowanieStale;
}

double Obligacja::pobierzCeneNominalna() const {
    return cenaNominalna;
}

uint32_t Obligacja::pobierzPoczatkowyTerminWykupu() const {
    return poczatkowyTerminWykupu;
}

void Obligacja::zmniejszTerminWykupu() {
    if (terminWykupu > 0) {
        terminWykupu--;
    }
}

bool Obligacja::czyWykupiona() const {
    return terminWykupu == 0;
}
