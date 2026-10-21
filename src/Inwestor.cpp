#include "Inwestor.h"

const std::string& Inwestor::pobierzId() const {
    return idInwestora;
}

Portfel& Inwestor::pobierzPortfel() {
    return portfel;
}

const Portfel& Inwestor::pobierzPortfel() const {
    return portfel;
}

