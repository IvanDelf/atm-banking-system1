#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ========== Account Class ==========
class Account {
public:
    int id;
    string name;
    string pin;
    double balance;

    // Read one account from file
    void loadFromFile(ifstream &file) {
        file >> id >> name >> pin >> balance;
    }

    // Check if pin matches
    bool checkPin(string inputPin) {
        return pin == inputPin;
    }

    // Deposit money
    void deposit(double amount) {
        balance += amount;
    }

    // Withdraw money, returns false if not enough balance
    bool withdraw(double amount) {
        if (amount > balance) {
            return false;
        }
        balance -= amount;
        return true;
    }

    // Change pin
    void changePin(string newPin) {
        pin = newPin;
    }

    void showBalance() {
        cout << "Your balance is: PHP " << balance << endl;
    }
};

// ========== ATM Class ==========
class ATM {
public:
    Account accounts[100];
    int totalAccounts = 0;
    Account *currentUser = NULL;

    // Load all accounts from file
    void loadAccounts() {
        ifstream file("accounts.txt");
        if (!file) {
            cout << "No accounts file found!" << endl;
            return;
        }
        while (!file.eof()) {
            accounts[totalAccounts].loadFromFile(file);
            if (file.fail()) break;
            totalAccounts++;
        }
        file.close();
    }

    // Save all accounts back to file
    void saveAccounts() {
        ofstream file("accounts.txt");
        for (int i = 0; i < totalAccounts; i++) {
            file << accounts[i].id << " "
                 << accounts[i].name << " "
                 << accounts[i].pin << " "
                 << accounts[i].balance << endl;
        }
        file.close();
    }

    // Login function
    bool login() {
        string name, pin;
        int tries = 0;

        while (tries < 3) {
            cout << "\n--- LOGIN ---" << endl;
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter PIN: ";
            cin >> pin;

            for (int i = 0; i < totalAccounts; i++) {
                if (accounts[i].name == name && accounts[i].checkPin(pin)) {
                    currentUser = &accounts[i];
                    cout << "Login successful! Welcome, " << name << endl;
                    return true;
                }
            }

            tries++;
            cout << "Wrong name or PIN. " << (3 - tries) << " tries left." << endl;
        }

        cout << "Too many failed attempts. Card blocked." << endl;
        return false;
    }

    // Show the menu
    void showMenu() {
        int choice;

        do {
            cout << "\n--- MAIN MENU ---" << endl;
            cout << "1. Check Balance" << endl;
            cout << "2. Deposit" << endl;
            cout << "3. Withdraw" << endl;
            cout << "4. Change PIN" << endl;
            cout << "5. Logout" << endl;
            cout << "Choose: ";
            cin >> choice;

            if (choice == 1) {
                currentUser->showBalance();

            } else if (choice == 2) {
                double amount;
                cout << "Enter deposit amount: ";
                cin >> amount;
                if (amount <= 0) {
                    cout << "Invalid amount." << endl;
                } else {
                    currentUser->deposit(amount);
                    saveAccounts();
                    cout << "Deposited PHP " << amount << " successfully." << endl;
                    currentUser->showBalance();
                }

            } else if (choice == 3) {
                double amount;
                cout << "Enter withdrawal amount: ";
                cin >> amount;
                if (amount <= 0) {
                    cout << "Invalid amount." << endl;
                } else if (currentUser->withdraw(amount)) {
                    saveAccounts();
                    cout << "Withdrawn PHP " << amount << " successfully." << endl;
                    currentUser->showBalance();
                } else {
                    cout << "Insufficient balance!" << endl;
                }

            } else if (choice == 4) {
                string oldPin, newPin, confirmPin;
                cout << "Enter current PIN: ";
                cin >> oldPin;
                if (!currentUser->checkPin(oldPin)) {
                    cout << "Wrong PIN!" << endl;
                } else {
                    cout << "Enter new PIN: ";
                    cin >> newPin;
                    cout << "Confirm new PIN: ";
                    cin >> confirmPin;
                    if (newPin == confirmPin) {
                        currentUser->changePin(newPin);
                        saveAccounts();
                        cout << "PIN changed successfully!" << endl;
                    } else {
                        cout << "PINs do not match!" << endl;
                    }
                }

            } else if (choice == 5) {
                cout << "Logged out. Thank you!" << endl;

            } else {
                cout << "Invalid choice. Try again." << endl;
            }

        } while (choice != 5);
    }

    // Run the ATM
    void run() {
        loadAccounts();
        if (login()) {
            showMenu();
        }
    }
};

// ========== Main ==========
int main() {
    ATM atm;
    atm.run();
    return 0;
}