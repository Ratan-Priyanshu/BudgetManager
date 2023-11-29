#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
#define RESET   7
#define RED     12
#define GREEN   10
#else
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#endif

using namespace std;

class MonthlyBudget
{
private:
    vector<string> categories;
    vector<double> budgets;

public:
    void setBudget();
    void displayTotalBudget() const;
    const vector<string>& getCategories() const { return categories; }
    const vector<double>& getBudgets() const { return budgets; }
};

class DailyExpenses
{
private:
    vector<double> expenses;
    vector<string> categories;

public:
    void addDailyExpense(const MonthlyBudget& mb);
    void displayExpensesVsBudget(const MonthlyBudget& mb) const;
};

// Function prototypes
void describeProgram();

#ifdef _WIN32
void setConsoleColor(int colorCode)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}
#endif


int main()
{
    MonthlyBudget mb;
    DailyExpenses de;

    describeProgram();

    // Set budget for each category
    mb.setBudget();

    // Display the total budget
    mb.displayTotalBudget();

    // Enter daily expenses
    char addAnother;
    do
    {
        de.addDailyExpense(mb);

        cout << "Do you want to add another expense? (y/n): ";
        cin >> addAnother;

    } while (addAnother == 'y' || addAnother == 'Y');

    // Display expenses vs budget
    de.displayExpensesVsBudget(mb);

#ifdef _WIN32
    system("pause");
#endif

    return 0;
}
void describeProgram()
{
#ifdef _WIN32
    // Windows-specific color codes
    setConsoleColor(GREEN);
    cout << "Welcome to the Monthly Budget Tracker!" << endl;
    setConsoleColor(RESET);
#endif
    setConsoleColor(RED);
    cout << "This program allows you to set budgets for different categories\n "
        << "and enter daily expenses for each category. \n";
    setConsoleColor(RESET);
    setConsoleColor(GREEN);
    cout << "It then compares daily expenses and budget.\n\n" << endl;
    setConsoleColor(RESET);
}


void MonthlyBudget::setBudget()
{
    int numCategories;
    setConsoleColor(GREEN);
   
    cout << "How many categories would you like to budget for? ";
    
    setConsoleColor(RESET);
    cin >> numCategories;

    categories.resize(numCategories);
    budgets.resize(numCategories);

    cout << "\nSet Budgets:\n";
    for (int i = 0; i < numCategories; ++i)
    {
        setConsoleColor(i % 2 == 0 ? RED : GREEN);
        cout << "Enter the name of category " << i + 1 << ": ";
        setConsoleColor(RESET);
        cin >> categories[i];

        setConsoleColor(i % 2 == 0 ? RED : GREEN);
        cout << "Enter budget for category " << categories[i] << ": Rs";
        setConsoleColor(RESET);
        cin >> budgets[i];

        while (cin.fail() || budgets[i] < 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            setConsoleColor(i % 2 == 0 ? RED : GREEN);
            cout << "Enter a valid budget (a non-negative number) for category " << categories[i] << ": Rs";
            setConsoleColor(RESET);
            cin >> budgets[i];
        }
    }
}


void MonthlyBudget::displayTotalBudget() const
{
    double totalBudget = 0.0;

    cout << "\nTotal Monthly Budget:\n";
    cout << fixed << showpoint << setprecision(2);
    for (size_t i = 0; i < categories.size(); ++i)
    {
#ifdef _WIN32
        setConsoleColor(GREEN);
#endif
        cout << categories[i] << ": Rs" << budgets[i] << endl;
#ifdef _WIN32
        setConsoleColor(RESET);
#endif
        totalBudget += budgets[i];
    }
    setConsoleColor(RED);
    cout << "-------------------------\n";
    setConsoleColor(RESET);
#ifdef _WIN32
    setConsoleColor(GREEN);
#endif
    cout << "Total: Rs" << totalBudget << endl;
#ifdef _WIN32
    setConsoleColor(RESET);
#endif
}


void DailyExpenses::addDailyExpense(const MonthlyBudget& mb)
{
    double expense;
    int categoryChoice;
    setConsoleColor(RED);
    cout << "\nEnter the expense amount: Rs";
    setConsoleColor(RESET);
    cin >> expense;

    while (cin.fail() || expense < 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter a valid expense (a non-negative number): Rs";
        cin >> expense;
    }
    setConsoleColor(RED);
    cout << "Select a category for the expense:\n";
    setConsoleColor(RESET);
    for (size_t i = 0; i < mb.getCategories().size(); ++i)
    {
#ifdef _WIN32
        setConsoleColor(GREEN);
#endif
        cout << i + 1 << ". " << mb.getCategories()[i] << endl;
#ifdef _WIN32
        setConsoleColor(RESET);
#endif
    }
    setConsoleColor(RED);
    cout << "Enter the category number: ";
    setConsoleColor(RESET);
    cin >> categoryChoice;

    while (cin.fail() || categoryChoice < 1 || categoryChoice > static_cast<int>(mb.getCategories().size()))
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid choice. Enter a number between 1 and " << mb.getCategories().size() << ": ";
        cin >> categoryChoice;
    }

    // Save the expense and category
    expenses.push_back(expense);
    categories.push_back(mb.getCategories()[categoryChoice - 1]);
}
// ...
void DailyExpenses::displayExpensesVsBudget(const MonthlyBudget& mb) const {
    cout << "\nExpenses vs Budget:\n";
    cout << fixed << showpoint << setprecision(2);
    cout << "--------------------------------------------------------------------\n";
#ifdef _WIN32
    setConsoleColor(GREEN);
#endif
    cout << left << setw(20) << "Category";
#ifdef _WIN32
    setConsoleColor(RESET);
#endif
#ifdef _WIN32
    setConsoleColor(RED);
#endif
    cout << right << setw(15) << "Budget";
#ifdef _WIN32
    setConsoleColor(RESET);
#endif
#ifdef _WIN32
    setConsoleColor(GREEN);
#endif
    cout << right << setw(15) << "Expenses";
#ifdef _WIN32
    setConsoleColor(RESET);
#endif
#ifdef _WIN32
    setConsoleColor(RED);
#endif
    cout << right << setw(20) << "Over(-)/Under\n";
#ifdef _WIN32
    setConsoleColor(RESET);
#endif
    cout << "--------------------------------------------------------------------\n";

    double totalOverUnder = 0.0;

    for (size_t i = 0; i < mb.getCategories().size(); ++i)
    {
        cout << left << setw(20) << mb.getCategories()[i];
#ifdef _WIN32
        setConsoleColor(GREEN);
#endif
        cout << right << setw(15) << mb.getBudgets()[i];
#ifdef _WIN32
        setConsoleColor(RESET);
#endif

        double totalExpensesForCategory = 0.0;

        // Sum expenses for the current category
        for (size_t j = 0; j < categories.size(); ++j)
        {
            if (categories[j] == mb.getCategories()[i])
            {
                totalExpensesForCategory += expenses[j];
            }
        }

        cout << right << setw(15) << totalExpensesForCategory;
        double overUnder = mb.getBudgets()[i] - totalExpensesForCategory;
        totalOverUnder += overUnder;

#ifdef _WIN32
        setConsoleColor(overUnder < 0 ? RED : GREEN);
#endif
        cout << right << setw(20) << overUnder << "\n";
#ifdef _WIN32
        setConsoleColor(RESET);
#endif  
    }
    cout << "--------------------------------------------------------------------\n";
#ifdef _WIN32
    setConsoleColor(totalOverUnder < 0 ? RED : GREEN);
#endif
    cout << "Total Over(-)/Under: Rs" << totalOverUnder << "\n";
#ifdef _WIN32
    setConsoleColor(RESET);
#endif
}
