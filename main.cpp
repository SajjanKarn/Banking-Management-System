#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

struct Account
{
    string name;
    int age;
    string password;
    int accountNumber;
    double balance;
    vector<string> transactions;
};

// Utility function to get the current date/time in string format
string getCurrentTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

// Function to generate a random 5-digit account number
int generateAccountNumber()
{
    return rand() % 90000 + 10000;
}

// Function to save account data to a file
void saveAccountToFile(Account &acc)
{
    ofstream file("accounts.txt", ios::app);
    if (file.is_open())
    {
        file << acc.name << " " << acc.age << " " << acc.password << " " << acc.accountNumber << " " << acc.balance << "\n";
        for (const string &transaction : acc.transactions)
        {
            file << transaction << "\n";
        }
        file << "---\n"; // separator for each account
        file.close();
    }
}

// Function to save all accounts to a file (overwrite the file)
void saveAllAccountsToFile(const vector<Account> &accounts)
{
    ofstream file("accounts.txt", ios::trunc); // Open in truncate mode to overwrite
    if (file.is_open())
    {
        for (const Account &acc : accounts)
        {
            file << acc.name << " " << acc.age << " " << acc.password << " " << acc.accountNumber << " " << acc.balance << "\n";
            for (const string &transaction : acc.transactions)
            {
                file << transaction << "\n";
            }
            file << "---\n"; // separator for each account
        }
        file.close();
    }
    else
    {
        cout << "Error saving accounts to file.\n";
    }
}

// Function to load account data from a file
bool loadAccountFromFile(vector<Account> &accounts)
{
    ifstream file("accounts.txt");
    if (!file.is_open())
        return false;

    Account acc;
    string line;
    while (file >> acc.name >> acc.age >> acc.password >> acc.accountNumber >> acc.balance)
    {
        acc.transactions.clear();
        while (getline(file, line) && line != "---")
        {
            if (!line.empty())
            {
                acc.transactions.push_back(line);
            }
        }
        accounts.push_back(acc);
    }
    file.close();
    return true;
}

// Function to find an account by name and account number
Account *findAccount(vector<Account> &accounts, const string &name, int accountNumber)
{
    for (Account &acc : accounts)
    {
        if (acc.name == name && acc.accountNumber == accountNumber)
        {
            return &acc;
        }
    }
    return nullptr;
}

// Function to create a new account
void createAccount(vector<Account> &accounts)
{
    Account acc;
    cout << "Enter your name: ";
    cin >> acc.name;
    cout << "Enter your age: ";
    cin >> acc.age;
    cout << "Set your password: ";
    cin >> acc.password;
    cout << "Enter initial balance: ";
    cin >> acc.balance;

    acc.accountNumber = generateAccountNumber();
    cout << "Your account number is: " << acc.accountNumber << "\n";

    acc.transactions.push_back("Account created on: " + getCurrentTime() + " with initial balance: " + to_string(acc.balance));

    accounts.push_back(acc);
    saveAccountToFile(acc);

    cout << "Account created successfully!\n";
}

// Function to deposit money
void deposit(vector<Account> &accounts)
{
    string name;
    int accountNumber;
    int amount;

    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your account number: ";
    cin >> accountNumber;

    Account *acc = findAccount(accounts, name, accountNumber);
    if (acc)
    {
        cout << "Enter deposit amount: ";
        cin >> amount;
        acc->balance += amount;
        acc->transactions.push_back("Deposited: " + to_string(amount) + " on: " + getCurrentTime());
        cout << "Deposit successful. New balance: " << acc->balance << "\n";

        // Save all accounts to file after deposit
        saveAllAccountsToFile(accounts);
    }
    else
    {
        cout << "Account not found!\n";
    }
}

// Function to withdraw money
void withdraw(vector<Account> &accounts)
{
    string name;
    int accountNumber;
    int amount;

    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your account number: ";
    cin >> accountNumber;

    Account *acc = findAccount(accounts, name, accountNumber);
    if (acc)
    {
        cout << "Enter amount to withdraw (must be multiple of 500): ";
        cin >> amount;
        if (amount % 500 != 0)
        {
            cout << "Invalid amount. Must be a multiple of 500.\n";
        }
        else if (acc->balance - amount < 0)
        {
            cout << "Insufficient funds. Balance cannot go below zero.\n";
        }
        else
        {
            acc->balance -= amount;
            acc->transactions.push_back("Withdrew: " + to_string(amount) + " on: " + getCurrentTime());
            cout << "Withdrawal successful. New balance: " << acc->balance << "\n";

            // Save all accounts to file after withdrawal
            saveAllAccountsToFile(accounts);
        }
    }
    else
    {
        cout << "Account not found!\n";
    }
}

// Function to view transaction history
void viewTransactionHistory(const vector<Account> &accounts)
{
    string name;
    int accountNumber;
    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your account number: ";
    cin >> accountNumber;

    const Account *acc = findAccount(const_cast<vector<Account> &>(accounts), name, accountNumber);
    if (acc)
    {
        cout << "Transaction history for account: " << acc->accountNumber << "\n";
        for (const string &transaction : acc->transactions)
        {
            cout << transaction << "\n";
        }
    }
    else
    {
        cout << "Account not found!\n";
    }
}

// Main menu
void menu()
{
    vector<Account> accounts;
    loadAccountFromFile(accounts);

    int choice;
    bool exitProgram = false;

    while (!exitProgram)
    {
        cout << "\n----- Bank Account Management System -----\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. View Transaction History\n";
        cout << "5. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            createAccount(accounts);
            break;
        case 2:
            deposit(accounts);
            break;
        case 3:
            withdraw(accounts);
            break;
        case 4:
            viewTransactionHistory(accounts);
            break;
        case 5:
            exitProgram = true;
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }

        if (choice != 5)
        {
            char exitChoice;
            cout << "Do you want to exit? (y/n): ";
            cin >> exitChoice;
            if (exitChoice == 'y' || exitChoice == 'Y')
            {
                exitProgram = true;
            }
        }
    }

    cout << "Thank you for using the Bank Account Management System!\n";
}

int main()
{
    srand(time(0)); // Initialize random seed for account number generation
    menu();
    return 0;
}
