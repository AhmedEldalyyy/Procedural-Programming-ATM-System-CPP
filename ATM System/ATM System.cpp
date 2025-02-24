#include<fstream>
#include <iomanip>
#include <iostream>
#include<limits>
#include <string>
#include<vector>

using namespace std;


const string ClientsFileName = "ClientsRecord.txt";

enum enMainMenuOptions {


    QuickWithdraw = 1,
    NormalWithdraw = 2,
    Deposit = 3,
    CheckBalance = 4,
    Logout = 5,
    Exit = 6
};

enum enQuickWithdrawOption {

    Twenty = 1,
    Fifty = 2,
    OneHundred = 3,
    TwoHundreds = 4,
    FourHundreds = 5,
    SixHundreds = 6,
    EightHundreds = 7,
    OneThousand = 8,
    MainMenu = 9
};

struct stClients {
    string AccountNumber = "";
    string PinCode = "";
    string FullName = "";
    string PhoneNumber = "";
    double Balance = 0.0f;
};

stClients CurrentClient;

vector<string> vMainMenuOptions = { "Quick Withdraw", "Normal Withdraw","Deposit","Check Balance","Logout","Exit"};

vector<string> vQuickWithdrawOptions = { "20", "50","100","200","400","600","800","1000","Main Menu"};

short GetUserChoice(short From, short To)
{

    short choice = 0;
    cout << " \n Choose what do you want to do [ " << From << " to " << To << " ] : ";
    cin >> choice;

    while (choice > To || choice < From || cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n Invalid input !\a\n";
        cout << " \n Choose what do you want to do [ " << From << " to " << To << " ] : ";
        cin >> choice;

    }

    return choice;
}

void PrintBalance()
{
    cout << "\n [Current Balance] : " << fixed << setprecision(2) << CurrentClient.Balance << endl << endl;
}

void SuccesfullTransactionMessage()
{
    cout << "\n Transaction was done successfully!\n";
    PrintBalance();
}

void ClearScreen() {
    system("cls");
}

void ShowMenu(string Title, vector<string> Options)
{
    ClearScreen();
    
    cout << " \n ___________________________________________\n\n";
    cout << "              " << Title << endl;;
    cout << " ___________________________________________\n\n";
    for (short i = 0; i < Options.size(); i++)
    {
        cout << "        [" << i + 1 << "] - " << Options.at(i) << endl;
    }
    cout << " ___________________________________________\n";
    
}

void GoBackToMenu(string Message = "Press any key to go back to main menu.....")
{
    cout << "\n " << Message;
    system("pause>0");
}

void ProgramScreensTemplate(string Screen)
{
    cout << " ____________________________\n\n";
    cout << "     " << Screen << "  \n";
    cout << " ____________________________\n\n";
}

vector<string> SplitWords(string Record, string delim = "#//#")
{
    vector<string> vWords;

    string Word = "";

    short pos = 0;

    while (( pos = Record.find(delim) ) != string::npos)
    {
        Word = Record.substr(0, pos);

        if (Word != "")
        {
            vWords.push_back(Word);
        }
        
        Record.erase(0, pos + delim.length());
    }
    if (Record != "")
    {
        vWords.push_back(Record);
    }
    return vWords;
}

stClients ClientRecordToStruct(string Record)
{
    vector<string> vRecords = SplitWords(Record);
    
    stClients sClient;

    sClient.AccountNumber = vRecords.at(0);
    sClient.PinCode = vRecords.at(1);
    sClient.FullName = vRecords.at(2);
    sClient.PhoneNumber = vRecords.at(3);
    sClient.Balance = stod(vRecords.at(4));


    
    return sClient;
    
}

string ClientStructToString(stClients sClient, string delim = "#//#")
{
    string Record = "";

    Record += sClient.AccountNumber + delim;
    Record += sClient.PinCode + delim;
    Record += sClient.FullName + delim;
    Record += sClient.PhoneNumber + delim;
    Record += to_string(sClient.Balance);

    return Record;
}

void WriteToFile(string FileName, string Record)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << Record << endl;
    }
    MyFile.close();
}

vector<stClients> ReadClientsFromFile(string FileName)
{
    fstream MyFile;
    stClients sClient;
    vector<stClients> vClients;
    string Record = "";
 
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        while (getline(MyFile, Record))
        {
            sClient = ClientRecordToStruct(Record);
            vClients.push_back(sClient);
        }
    }

    return vClients;
}

bool ValidateLogin(string AccountNumber , string PinCode , vector<stClients> vClients )
{
    for (stClients& C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            CurrentClient = C;
            return true;
        }
    }

    return false;
}

void ClientsVectorToFile(string FileName , vector<stClients> vClients)

{
    fstream ClientsFile;
    string Record = "";
    ClientsFile.open(FileName, ios::out);
    if(ClientsFile.is_open())
    {
        for (stClients& Client : vClients)
        {
            Record = ClientStructToString(Client);
            ClientsFile << Record << endl;
        }
        ClientsFile.close();
    }

}

void Login(vector<stClients> vClients  , bool &LoggedIn)
{
    system("cls");

    string AccountNumber = "";
    string PinCode = "";

    ProgramScreensTemplate("    Login Screen");

    cout << " Enter Account Number : ";
    getline(cin >> ws, AccountNumber);
    cout << " Enter Pin Code : ";
    getline(cin , PinCode);

    while (!ValidateLogin(AccountNumber, PinCode, vClients))
    {
        cout << "\n Invalid Account number or Pin Code!\a\n";
        cout << "\n Enter Account Number : ";
        getline(cin >> ws, AccountNumber);
        cout << " Enter Pin Code : ";
        getline(cin, PinCode);

    }

    LoggedIn = true;
}

enMainMenuOptions PerformMainMenuOptions()
{
    ShowMenu("ATM Main Menu Screen", vMainMenuOptions);
    enMainMenuOptions enUserChouice = enMainMenuOptions(GetUserChoice(1,6));
    return enUserChouice;
}

bool ValidateTransaction(int amount, double balance , bool isWithdraw)
{
    if (isWithdraw)
    {
        return (balance - amount >= 0) && (amount > 0) && (amount % 5 == 0);
    }
     
    return amount >= 20;
}

bool ConfirmationMessage()
{
    char choice;
    cout << "\n Are you sure you want to peform this transaction ? [Y] - Yes / [Other] - No : ";
    cin >> choice;

    return toupper(choice) == 'Y';
}

void OperationCancelMessage()
{
    
    cout << "\n Operation Cancelled...\n";
}

void UpdateClientBalance(int amount,vector<stClients>& vClients, bool isWithdraw)
{

    if (!ValidateTransaction(amount, CurrentClient.Balance, isWithdraw) )
    {
        cout << "\n The amout exceeds your balance or invalid input!\n\a";
    }
    else
    {
        for (stClients& C : vClients)
        {
            if (C.AccountNumber == CurrentClient.AccountNumber)
            {
                if (isWithdraw)
                {
                    C.Balance -= amount;
                }
                else
                {
                    C.Balance += amount;
                }
                CurrentClient = C;
                ClientsVectorToFile(ClientsFileName, vClients);
                SuccesfullTransactionMessage();
                break;
            }
        }
    }
    
}

void CheckBalanceScreen()
{
    system("cls");
    ProgramScreensTemplate("Check Balance Screen");
    PrintBalance();
    GoBackToMenu();
}

void QuickWithdrawScreen(vector<stClients>& vClients)
{
    ShowMenu("Quick Withdraw", vQuickWithdrawOptions);
    PrintBalance();
    enQuickWithdrawOption QuickwithdrawChoice;
    
    QuickwithdrawChoice = enQuickWithdrawOption(GetUserChoice(1, 9));
    
    if (ConfirmationMessage())
    {
        switch (QuickwithdrawChoice)
        {
           
        case Twenty:

            UpdateClientBalance(20, vClients,true);
            break;

        case Fifty:
            UpdateClientBalance(50, vClients, true);
            break;

        case OneHundred:
            UpdateClientBalance(100, vClients, true);
            break;

        case TwoHundreds:
            UpdateClientBalance(200, vClients, true);
            break;

        case FourHundreds:
            UpdateClientBalance(400, vClients, true);
            break;

        case SixHundreds:
            UpdateClientBalance(600, vClients, true);
            break;

        case EightHundreds:
            UpdateClientBalance(800, vClients, true);
            break;

        case OneThousand:
            UpdateClientBalance(1000, vClients, true);
            break;

        case MainMenu:
            break;

        default:
            OperationCancelMessage();
            break;
        }

    }

    else
    {
        OperationCancelMessage();
    }

    GoBackToMenu();
}

void ReadTransactionAmount(int &amount,string Message ="Enter your amount : ")
{
    
    cout << "\n " << Message ;
    cin >> amount;

}

void NormalWithdrawScreen(vector<stClients>& vClients)
{
    ClearScreen();

    ProgramScreensTemplate("Normal Withdraw Screen");
    
    int amount = 0;
    ReadTransactionAmount(amount,"Enter amount multiple of 5's : ");
    while (!ValidateTransaction(amount, CurrentClient.Balance,true))
    {
        cout << "\n Amount exceeds your balance or invalid input!\n\a";
        ReadTransactionAmount(amount, "Enter amount multiple of 5's : ");

    }
    
    if (ConfirmationMessage())
    {
        UpdateClientBalance(amount,vClients,true);
        
    }
    else
    {
        OperationCancelMessage();
    }
    
    GoBackToMenu();

}

void DepositScreen(vector<stClients>& vClients)
{
    system("cls");

    ProgramScreensTemplate("Deposit Screen");
    int amount = 0;
    ReadTransactionAmount(amount);
    if (!ValidateTransaction(amount, CurrentClient.Balance,false))
    {
        cout << "\n Wrong input!\n";
        OperationCancelMessage();
    }
    else
    {
        if (ConfirmationMessage())
        {
            UpdateClientBalance(amount, vClients, false);
        }
        
        else
        {
            
            OperationCancelMessage();
        }
       
    }
    GoBackToMenu();
}

void Run()
{
    vector<stClients> ClientsVector = ReadClientsFromFile(ClientsFileName);
    
    bool Loggedin = false;
    bool Running = true;
    
    enMainMenuOptions enMainMenuChoice;

    while (Running)
    {
        Login(ClientsVector, Loggedin);
        
        while (Loggedin)
        {
            enMainMenuChoice = PerformMainMenuOptions();

            switch (enMainMenuChoice)
            {
            case QuickWithdraw: 
                QuickWithdrawScreen(ClientsVector);
                break;
            case NormalWithdraw:
                NormalWithdrawScreen(ClientsVector);
                break;
            case CheckBalance:
                CheckBalanceScreen();
                break;
            case Deposit:
                DepositScreen(ClientsVector);
                break;
            case Logout:
                Loggedin = false;
                break;
            case Exit:
                Loggedin = false;
                Running = false;
                break;
            default:
                cout << "\n Invalid input!\n\a";
                break;
            }
           
        }
        
    }
    exit(0);
}

int main()
{
    Run();
}
