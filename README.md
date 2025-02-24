# ATM Simulator [NOT OOP]

A Procedural C++ console application emulating core ATM functionalities. Handles client authentication, transactions, and data persistence via file I/O.

## Features
- **Login/Logout** with PIN validation
- **Quick/Normal Withdrawals** (amount validation + balance checks)
- **Deposit** (min. $20)
- **Balance Inquiry**
- File-based client database (`ClientsRecord.txt`)

## Usage
1. **Compile**: `g++ main.cpp -o atm`
2. **Run**: `./atm`
3. **Sample Account**:  
   Account: `A201` | PIN: `2312` (modify `ClientsRecord.txt` for more)

## Input Rules
- Withdrawals: Multiples of 5 only
- Deposits: Minimum $20
- Transaction confirmations required

## Files
- `main.cpp`: Core application logic  
- `ClientsRecord.txt`: Client data storage (auto-created)



> **Note**: Requires C++11+ compiler. Contributions welcome!
