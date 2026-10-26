# Simple-console-based-Accounting-System-made-using-Cpp
A console based basic Accounting System which takes single line Journal entries, can provide Ledger balances and provide with Trial balance to check the arithmetic correction of the accounts.

## Features 
1. It can accept single Journal entries (one debit and one credit entry), along with date and description.
2. It can provide with the Ledger balances of the account one searches for.
3. It can provide with the Trial balance to check the arithmetic accuracy of the accounts.
4. All the data gets stored in .bin files.
5. A proper traditional format (used in the Journal, Ledger and Trial Balance) has been maintained. 

## Limitations
1. The debit and credit side entry along with the description can only store in them a maximum of 30 characters.
2. Ledgers can only be displayed one accounts at a time.
3. Only single Journal entries can be stored (Few Journal entries have been provided in the Journal Entries.bin file).

## How does it work
Since it is a simple Accounting system it is fairly easy to use. The functions include:
1. "Create new Journal" : This creates a fresh Journal Entry file, all the previous records will be deleted.
2. "Enter new entry" : This appends a new entry into the file (date format (dd-mm-yyyy), dr side entry, cr side entry, amount and description).
3. "Display Journal" : Displays the whole Journal (it is requested that the entries are to be stored in a chronological order).
4. "Search Ledger by name" : This defines the Ledger and displays the Ledger A/c which is searched.
5. "Display Trial Balance" : This displays the Trial balance on the console, if the either sides matches it shows a perfect arithmetic accuracy. 
