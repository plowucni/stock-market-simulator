# Stock Market Simulator (CLI)

A multi-asset financial market simulation engine developed in **C++17**. The project models a turn-based trading environment supporting stocks, government bonds, and cryptocurrencies with real-time portfolio revaluation and custom persistence[cite: 36, 37].

## Key Features

- **Object-Oriented Architecture (OOP):** Abstract base class `InstrumentFinansowy` defining uniform interfaces for all assets via runtime polymorphism (`std::shared_ptr`)[cite: 36, 40].
- **Observer Design Pattern:** Decoupled event notification mechanism where the market engine notifies the `Portfel` (wallet) of price fluctuations to recalculate Net Asset Value (NAV) automatically.
- **Asset Logic:** 
  - *Equities & Crypto:* Volatility simulation with boundary clamps preventing sub-zero valuations[cite: 38, 39, 41].
  - *Bonds:* Coupon interest schedules and automated principal redemption at maturity[cite: 36, 38, 41].
- **Data Persistence & Resilience:** File-based account persistence (`konta.txt`, `historia.txt`) with custom exception hierarchies handling corrupted records gracefully[cite: 37, 38, 41].

## Build & Run

### Requirements
- GCC / G++ (supporting C++17 or higher)
- Make / Terminal (Windows/Linux/macOS)[cite: 35]

### Compilation
```bash
g++ -std=c++17 src/*.cpp -o Gielda.exe
./Gielda.exe
