#include "Akcja.h"
#include "GieldaEngine.h"
#include "GieldaException.h"
#include "Kryptowaluta.h"
#include "Obligacja.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>

void utworzDomyslnegoGracza(Gielda& engine) {
  auto gracz = std::make_shared<Inwestor>("GRACZ", 100000.00);
  gracz->pobierzPortfel().dodajAktywo("AAPL", 10.0, 150.0);
  gracz->pobierzPortfel().dodajAktywo("BTC", 0.5, 60000.0);
  engine.dodajInwestora(gracz);
  engine.zapiszPortfele();
}

void wyczyscStrumien() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void pokazNaglowek() {
  std::cout << "\n+==================================================================================================+\n";
  std::cout << "|                             SYMULATOR GIELDY PAPIEROW WARTOSCIOWYCH                              |\n";
  std::cout << "+==================================================================================================+\n";
}

void wyswietlStanRynku(const Gielda &engine) {
  std::cout << "\n+==================================================================================================+\n";
  std::cout << "|                                       AKTUALNY STAN RYNKU                                        |\n";
  std::cout << "+==================================================================================================+\n";
  std::cout << "+-----------------+----------+---------------------------+--------------+--------------+-----------+\n";
  std::cout << "| Typ             | Ticker   | Nazwa                     | Cena         | Otwarcie     | Wolumen   |\n";
  std::cout << "+-----------------+----------+---------------------------+--------------+--------------+-----------+\n";
  for (const auto &inst : engine.pobierzInstrumenty()) {
    std::cout << "| " << std::left << std::setw(15) << inst->pobierzTyp() << " | " << std::left << std::setw(8) << inst->pobierzSymbol() << " | " << std::left << std::setw(25) << inst->pobierzNazwe() << " | " << std::right << std::setw(12) << std::fixed << std::setprecision(2) << inst->pobierzCeneAktualna() << " | " << std::setw(12) << inst->pobierzCeneOtwarciaSesji() << " | " << std::setw(9) << inst->pobierzWolumenObrotu() << " |\n";
  }
  std::cout << "+-----------------+----------+---------------------------+--------------+--------------+-----------+\n";
}

void wyswietlPortfel(std::shared_ptr<Inwestor> gracz, const Gielda &engine) {
  if (!gracz)
    return;
  const auto &p = gracz->pobierzPortfel();
  std::cout << "\n+==================================================================================================+\n";
  std::cout << "|                                      STAN TWOJEGO PORTFELA                                       |\n";
  std::cout << "+==================================================================================================+\n";
  std::cout << "  Inwestor: " << gracz->pobierzId() << "\n";
  std::cout << "  Gotowka: " << std::fixed << std::setprecision(2) << p.pobierzWolnaGotowka() << " zl\n";
  std::cout << "  Wartosc NAV (aktywa + gotowka): " << p.pobierzWartoscPortfela() << " zl\n";
  std::cout << "\n  Posiadane instrumenty:\n";

  const auto &aktywa = p.pobierzAktywa();
  if (aktywa.empty()) {
    std::cout << "  (brak posiadanych instrumentow w portfelu)\n";
  } else {
    std::cout << "+----------+--------------+--------------+--------------+--------------+-----------+\n";
    std::cout << "| Ticker   | Ilosc        | Srednia cena | Cena rynkowa | Wartosc      | Wynik     |\n";
    std::cout << "+----------+--------------+--------------+--------------+--------------+-----------+\n";
    for (const auto &[tick, qty] : aktywa) {
      double avgPrice = p.pobierzSredniaCeneZakupu(tick);

      double currentPrice = 0.0;
      try {
        auto inst = engine.pobierzInstrument(tick);
        currentPrice = inst->pobierzCeneAktualna();
      } catch (...) {
        currentPrice = p.pobierzWyceneAktywa(tick);
      }

      double val = qty * currentPrice;
      double diff = (avgPrice > 0) ? (currentPrice - avgPrice) / avgPrice * 100.0 : 0.0;

      std::cout << "| " << std::left << std::setw(8) << tick << " | " << std::right << std::setw(12) << std::fixed << std::setprecision(4) << qty << " | " << std::setw(12) << std::fixed << std::setprecision(2) << avgPrice << " | " << std::setw(12) << currentPrice << " | " << std::setw(12) << val << " | " << std::showpos << std::setw(8) << std::fixed << std::setprecision(2) << diff << "% |\n" << std::noshowpos;
    }
    std::cout << "+----------+--------------+--------------+--------------+--------------+-----------+\n";
  }
}

bool pobierzDaneTransakcji(std::string& ticker, double& qty) {
  std::cout << "Podaj ticker: ";
  std::cin >> ticker;
  std::transform(ticker.begin(), ticker.end(), ticker.begin(), [](unsigned char c) { return std::toupper(c); });
  std::cout << "Podaj ilosc: ";
  if (!(std::cin >> qty)) {
    wyczyscStrumien();
    std::cout << "Bledna wartosc ilosci.\n";
    return false;
  }
  return true;
}

int main() {
  Gielda engine;

  pokazNaglowek();

  engine.dodajInstrument(std::make_shared<Akcja>("AAPL", "Apple Inc.", 150.00));
  engine.dodajInstrument(std::make_shared<Akcja>("TSLA", "Tesla Inc.", 200.00));
  engine.dodajInstrument(std::make_shared<Akcja>("MSFT", "Microsoft Corp.", 300.00));
  engine.dodajInstrument(std::make_shared<Obligacja>("US10Y", "US Treasury 10Y", 100.00, 0.045, 15));
  engine.dodajInstrument(std::make_shared<Obligacja>("PL10Y", "Poland Treasury 10Y", 100.00, 0.06, 10));
  engine.dodajInstrument(std::make_shared<Kryptowaluta>("BTC", "Bitcoin", 60000.00));
  engine.dodajInstrument(std::make_shared<Kryptowaluta>("ETH", "Ethereum", 3000.00));

  try {
    std::cout << "Proba wczytania stanu portfela z konta.txt...\n";
    engine.wczytajPortfele();
    std::cout << "Stan portfela wczytany pomyslnie.\n";
  } catch (const BladBrakPlikuKont &e) {
    std::cout << e.what() << " nie istnieje. Tworzenie portfela domyslnego...\n";
    utworzDomyslnegoGracza(engine);
  } catch (const BladFormatowaniaBazy &e) {
    std::cout << "[" << e.pobierzKod() << "] Uszkodzony plik portfeli! " << e.what() << "\n";
    std::cout << "Przywracanie stanu poczatkowego...\n";
    utworzDomyslnegoGracza(engine);
  }

  auto gracz = engine.pobierzInwestora("GRACZ");

  bool dziala = true;
  while (dziala) {
    pokazNaglowek();
    std::cout << "Tura symulacji: " << engine.pobierzTureZegara() << "\n";
    std::cout << "1. Wyswietl aktualny stan rynku\n";
    std::cout << "2. Wyswietl stan mojego portfela\n";
    std::cout << "3. Kup instrument\n";
    std::cout << "4. Sprzedaj instrument\n";
    std::cout << "5. Wykonaj krok symulacji (zmiana cen)\n";
    std::cout << "6. Wyjdz z symulatora\n";
    std::cout << "Wybierz opcje (1-6): ";

    int opcja = 0;
    if (!(std::cin >> opcja)) {
      wyczyscStrumien();
      std::cout << "Niepoprawny znak! Sprobuj ponownie.\n";
      continue;
    }

    switch (opcja) {
    case 1:
      wyswietlStanRynku(engine);
      break;
    case 2:
      wyswietlPortfel(gracz, engine);
      break;
    case 3: {
      std::string ticker;
      double qty;
      if (pobierzDaneTransakcji(ticker, qty)) {
        try {
          engine.kupNatychmiast("GRACZ", ticker, qty);
        } catch (const BladGieldy &e) {
          std::cout << "[" << e.pobierzKod() << "] Blad: " << e.what() << "\n";
        }
      }
      break;
    }
    case 4: {
      std::string ticker;
      double qty;
      if (pobierzDaneTransakcji(ticker, qty)) {
        try {
          engine.sprzedajNatychmiast("GRACZ", ticker, qty);
        } catch (const BladGieldy &e) {
          std::cout << "[" << e.pobierzKod() << "] Blad: " << e.what() << "\n";
        }
      }
      break;
    }
    case 5:
      engine.wykonajKrokSymulacji();
      break;
    case 6:
      engine.zapiszPortfele();
      std::cout << "Zapisano stan portfela. Do zobaczenia!\n";
      dziala = false;
      break;
    default:
      std::cout << "Niepoprawna opcja. Wybierz liczbe od 1 do 6.\n";
      break;
    }
  }

  return 0;
}
