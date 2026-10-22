# Stock Market Simulator (CLI)

A multi-asset financial market simulation engine developed in **C++**. The project models a turn-based trading environment supporting stocks, government bonds, and cryptocurrencies with real-time portfolio revaluation and custom persistence.

## Key Features

- **Object-Oriented Architecture (OOP):** Abstract base class `InstrumentFinansowy` defining uniform interfaces for all assets via runtime polymorphism (`std::shared_ptr`).
- **Observer Design Pattern:** Decoupled event notification mechanism where the market engine notifies the `Portfel` (wallet) of price fluctuations to recalculate Net Asset Value (NAV) automatically.
- **Asset Logic:** 
  - *Equities & Crypto:* Volatility simulation with boundary clamps preventing sub-zero valuations.
  - *Bonds:* Coupon interest schedules and automated principal redemption at maturity.
- **Data Persistence & Resilience:** File-based account persistence (`konta.txt`, `historia.txt`) with custom exception hierarchies handling corrupted records gracefully.

## Build & Run

### Requirements
- GCC / G++ (supporting C++17 or higher)
- Make / Terminal (Windows/Linux/macOS)

### Compilation
```bash
g++ -std=c++17 src/*.cpp -o Gielda.exe
./Gielda.exe
