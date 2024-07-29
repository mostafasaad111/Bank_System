#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector<string> SplitString(const string& S1, const string& Delim) {
    vector<string> vString;
    size_t pos = 0, start = 0;
    string sWord;

    while ((pos = S1.find(Delim, start)) != string::npos) {
        sWord = S1.substr(start, pos - start);
        if (!sWord.empty()) {
            vString.push_back(sWord);
        }
        start = pos + Delim.length();
    }
    sWord = S1.substr(start);
    if (!sWord.empty()) {
        vString.push_back(sWord);
    }
    return vString;
}

sClient ReadClient() {
    sClient Client;

    cout << "Please enter your name" << endl;
    getline(cin >> ws, Client.Name);

    cout << "Please enter Account Number " << endl;
    cin >> Client.AccountNumber;

    cout << "Please enter Account Balance " << endl;
    cin >> Client.AccountBalance;

    cout << "Please enter Phone " << endl;
    cin >> Client.Phone;

    cout << "Please enter Pin Code " << endl;
    cin >> Client.PinCode;

    return Client;
}

sClient ConvertLineToRecord(const string& Line, const string& Seperator = "#||#") {
    sClient Client;
    vector<string> vClientData = SplitString(Line, Seperator);

    if (vClientData.size() == 5) {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]);
    }
    return Client;
}

string ConvertRecordToLine(const sClient& Client, const string& sper = "#||#") {
    string s = "";
    s += Client.AccountNumber + sper;
    s += Client.PinCode + sper;
    s += Client.Name + sper;
    s += Client.Phone + sper;
    s += to_string(Client.AccountBalance);
    return s;
}

vector<sClient> LoadClientsDataFromFile(const string& FileName) {
    vector<sClient> vClients;
    ifstream MyFile(FileName);
    if (MyFile.is_open()) {
        string Line;
        while (getline(MyFile, Line)) {
            sClient Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void SaveClientsDataToFile(const string& FileName, const vector<sClient>& vClients) {
    ofstream MyFile(FileName, ios::out);
    if (MyFile.is_open()) {
        for (const sClient& Client : vClients) {
            if (!Client.MarkForDelete) {
                MyFile << ConvertRecordToLine(Client) << endl;
            }
        }
        MyFile.close();
    }
}

void PrintClientCard(const sClient& Client) {
    cout << "\nThe following are the client details:\n";
    cout << "Account Number: " << Client.AccountNumber << "\n";
    cout << "Pin Code      : " << Client.PinCode << "\n";
    cout << "Name          : " << Client.Name << "\n";
    cout << "Phone         : " << Client.Phone << "\n";
    cout << "Account Balance: " << Client.AccountBalance << "\n";
}

bool FindClientByAccountNumber(const string& AccountNumber, const vector<sClient>& vClients, sClient& Client) {
    for (const sClient& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            Client = C;
            return true;
        }
    }
    return false;
}

string ReadClientAccountNumber() {
    string AccountNumber;
    cout << "\nPlease enter Account Number: ";
    cin >> AccountNumber;
    return AccountNumber;
}

void UpdateClient(const string& AccountNumber, vector<sClient>& vClients) {
    for (sClient& c : vClients) {
        if (c.AccountNumber == AccountNumber) {
            cout << "Enter pin code?";
            getline(cin >> ws, c.PinCode);
            cout << "Enter Name?";
            getline(cin, c.Name);
            cout << "Enter Phone?";
            getline(cin, c.Phone);
            cout << "Enter Account Balance?";
            cin >> c.AccountBalance;

            cout << "\nClient Updated Successfully.";
        }
    }
}

bool UpdateAccount(const string& AccountNumber, vector<sClient>& vClients) {
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintClientCard(Client);

        cout << "\n\nDo you want to update your account? (y/n): ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y') {
            UpdateClient(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);

            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\n\nClient updated successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is not found!";
    }
    return false;
}

short MainMenu() {
    short Number;
    cout << "__________________________________________________________________________\n";
    cout << "\t\t\t\tMain Menu Screen\n";
    cout << "__________________________________________________________________________\n";
    cout << "\t\t[1] Show Client List\n";
    cout << "\t\t[2] Add New Client\n";
    cout << "\t\t[3] Delete Client\n";
    cout << "\t\t[4] Update Client Info\n";
    cout << "\t\t[5] Find Client\n";
    cout << "\t\t[6] Exit\n";
    cout << "__________________________________________________________________________\n";
    cout << "Choose what do you want to do? [1 to 6]? ";
    cin >> Number;
    return Number;
}

void PrintClientRecord(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<sClient> vClients) {
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << "_______________________________________________________";
    cout << "_________________________________________\n";
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance\n";
    cout << "_______________________________________________________";
    cout << "_________________________________________\n";
    for (const sClient& Client : vClients) {
        PrintClientRecord(Client);
        cout << endl;
    }
    cout << "_______________________________________________________";
    cout << "_________________________________________\n";
}

void AddNewClient(const string& FileName, vector<sClient>& vClients) {
    sClient Client = ReadClient();
    vClients.push_back(Client);
    SaveClientsDataToFile(FileName, vClients);
}

bool MarkClientForDeleteByAccountNumber(const string& AccountNumber, vector<sClient>& vClients) {
    for (sClient& c : vClients) {
        if (c.AccountNumber == AccountNumber) {
            c.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteClientByAccountNumber(const string& AccountNumber, vector<sClient>& vClients) {
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintClientCard(Client);

        cout << "\n\nDo you want to delete your account? (y/n): ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y') {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);

            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\n\nClient deleted successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is not found!";
    }
    return false;
}

void FindAndPrintClient(const string& AccountNumber, const vector<sClient>& vClients) {
    sClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        PrintClientCard(Client);
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is not found!\n";
    }
}

int main() {
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    short Number;

    do {
        Number = MainMenu();

        switch (Number) {
        case 1:
            PrintAllClientsData(vClients);
            break;
        case 2:
            AddNewClient(ClientsFileName, vClients);
            break;
        case 3: {
            string AccountNumber = ReadClientAccountNumber();
            DeleteClientByAccountNumber(AccountNumber, vClients);
            break;
        }
        case 4: {
            string AccountNumber = ReadClientAccountNumber();
            UpdateAccount(AccountNumber, vClients);
            break;
        }
        case 5: {
            string AccountNumber = ReadClientAccountNumber();
            FindAndPrintClient(AccountNumber, vClients);
            break;
        }
        case 6:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice. Please select a valid option." << endl;
        }
    } while (Number != 6);

    return 0;
}


