#include "Obserwator.h"
#include <algorithm>

void PodmiotObserwowany::zarejestrujObserwatora(std::shared_ptr<Obserwator> o) {
    if (o) {
        auto it = std::find(obserwatorzy.begin(), obserwatorzy.end(), o);
        if (it == obserwatorzy.end()) {
            obserwatorzy.push_back(o);
        }
    }
}

void PodmiotObserwowany::powiadomObserwatorow(const std::string& symbol, double nowaCena) {
    for (const auto& obs : obserwatorzy) {
        if (obs) {
            obs->aktualizujStan(symbol, nowaCena);
        }
    }
}
