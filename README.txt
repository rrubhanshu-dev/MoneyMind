# MoneyMind
### Smart Personal Finance and Expense Tracker Using C

MoneyMind is a terminal-based personal finance tracker developed in C for a B.Tech BCPC solo project. It helps users record income and expenses, review transactions, calculate financial summaries, and check a budget.

## Features

- Add income and expense records
- Display saved transactions and search by transaction ID
- Edit a transaction's amount or delete a transaction
- Calculate total income, expenses, and balance
- Show expense totals by category
- Set and check a budget
- Sort transactions by amount
- Export transactions to CSV for Excel
- Import transaction rows from CSV

## Files

- `moneymind.c` — C source code
- `EXPLAIN.txt` — detailed code explanation and viva guide
- `transactions.txt` — saved transaction records
- `budget.txt` — saved budget value
- `transactions.csv` — exported transaction data
- `mydata.csv` — example CSV file for import

## Build and Run

Compile with GCC:

```bash
gcc -std=c11 -Wall -Wextra moneymind.c -o moneymind.exe