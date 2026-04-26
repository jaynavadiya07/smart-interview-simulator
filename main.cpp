#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

// COLORS
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

// UI DESIGN
void line() {
    cout << "--------------------------------------------------\n";
}

void title(string text) {
    line();
    cout << "   " << text << endl;
    line();
}

// Loading Animation
void loadingAnimation(string text) {
    cout << "\n   " << text;
    for (int i = 0; i < 5; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(400));
    }
    cout << "\n\n";
}

// Progress Bar
void showProgress(int current, int total) {
    int barWidth = 30;
    float progress = (float)current / total;

    cout << "\n   Progress: [";

    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "█";
        else cout << "-";
    }

    cout << "] " << int(progress * 100.0) << "%\n";
}

// Question Structure
struct Question {
    string question;
    string options[3];
    int correct;
};

// Question Bank
vector<Question> questions = {
    {"What is C++?", {"Programming Language", "OS", "Game"}, 1},
    {"What is OOP?", {"Output Process", "Object Oriented Programming", "None"}, 2},
    {"What is Function?", {"Variable", "Loop", "Block of code"}, 3},
    {"What is Array?", {"Loop", "Collection of elements", "Condition"}, 2},
    {"What is Loop?", {"Variable", "Function", "Repetition"}, 3},
    {"Comment symbol?", {"//", "**", "##"}, 1},
    {"Valid data type?", {"integer", "int", "number"}, 2},
    {"Which is loop?", {"for", "int", "char"}, 1},
    {"Which is keyword?", {"class", "apple", "car"}, 1},
    {"OOP pillar?", {"Encapsulation", "Cooking", "Driving"}, 1}
};

// Safe Input
int getValidInput() {
    int x;
    cin >> x;

    while (cin.fail() || x < 1 || x > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "   Invalid! Enter 1-3: " << RESET;
        cin >> x;
    }
    return x;
}

// Rank System
string getRank(int percentage) {
    if (percentage >= 80) return "Expert";
    else if (percentage >= 60) return "Intermediate";
    else if (percentage >= 40) return "Beginner";
    else return "Needs Improvement";
}

// Save Result
void saveResult(string name, int score, int total, int percentage) {
    ofstream file("interview.txt", ios::app);
    time_t now = time(0);

    file << "\n----------------------------------------\n";
    file << "Name  : " << name << endl;
    file << "Score : " << score << "/" << total << endl;
    file << "Result: " << percentage << "%" << endl;
    file << "Rank  : " << getRank(percentage) << endl;
    file << "Date  : " << ctime(&now);
    file << "----------------------------------------\n";

    file.close();
}

// View History
void viewHistory() {
    ifstream file("interview.txt");
    string line;

    title("INTERVIEW HISTORY");

    if (!file) {
        cout << RED << "   No records found!\n" << RESET;
        return;
    }

    while (getline(file, line)) {
        cout << "   " << line << endl;
    }
}

// Clear History
void clearHistory() {
    ofstream file("interview.txt", ios::trunc);
    file.close();
    cout << YELLOW << "\n   History Cleared Successfully!\n\n" << RESET;
}

// Interview
void startInterview() {
    string name;

    title("START INTERVIEW");

    cout << "   Enter your name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    loadingAnimation("Preparing your interview");

    srand(time(0));
    random_shuffle(questions.begin(), questions.end());

    int score = 0;
    int total = 5;

    for (int i = 0; i < total; i++) {

        cout << "\n";
        line();

        cout << CYAN << "   Q" << i + 1 << ": " << questions[i].question << RESET << endl;

        for (int j = 0; j < 3; j++) {
            cout << "     " << j + 1 << ". " << questions[i].options[j] << endl;
        }

        cout << YELLOW << "\n   Answer (10 sec): " << RESET;

        auto start = chrono::steady_clock::now();
        int ans = getValidInput();
        auto end = chrono::steady_clock::now();

        auto time_taken = chrono::duration_cast<chrono::seconds>(end - start);

        if (time_taken.count() > 10) {
            cout << RED << "   ⏱ Time Up!\n" << RESET;
        }
        else if (ans == questions[i].correct) {
            cout << GREEN << "   ✔ Correct!\n" << RESET;
            score++;
        }
        else {
            cout << RED << "   ✘ Wrong!\n" << RESET;
        }

        showProgress(i + 1, total);
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    int percentage = (score * 100) / total;

    cout << "\n";
    title("FINAL RESULT");

    cout << "   Name     : " << name << endl;
    cout << "   Correct  : " << score << endl;
    cout << "   Wrong    : " << total - score << endl;
    cout << "   Score    : " << percentage << "%" << endl;
    cout << "   Rank     : " << getRank(percentage) << endl;

    cout << "\n";
    line();

    saveResult(name, score, total, percentage);
}

// MAIN
int main() {
    int choice;

    do {
        cout << "\n";
        title("SMART INTERVIEW SIMULATOR");

        cout << "   1. Start Interview\n";
        cout << "   2. View History\n";
        cout << "   3. Clear History\n";
        cout << "   4. Exit\n\n";

        cout << "   Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "\n   Invalid input!\n" << RESET;
            continue;
        }

        switch (choice) {
            case 1: startInterview(); break;
            case 2: viewHistory(); break;
            case 3: clearHistory(); break;
            case 4: cout << YELLOW << "\n   Exiting...\n" << RESET; break;
            default: cout << RED << "\n   Invalid choice!\n" << RESET;
        }

    } while (choice != 4);

    cout << GREEN << "\n   Thank You!\n\n" << RESET;
    return 0;
}