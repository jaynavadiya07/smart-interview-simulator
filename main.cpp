#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std;

// COLORS
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

// GLOBALS (thread input)
atomic<bool> answered(false);
int userAnswer;

// UI
void line() {
    cout << "--------------------------------------------------\n";
}

void title(string text) {
    line();
    cout << "   " << text << endl;
    line();
}

// Loading
void loadingAnimation(string text) {
    cout << "\n   " << text;
    for (int i = 0; i < 5; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(300));
    }
    cout << "\n\n";
}

// Progress
void showProgress(int current, int total) {
    int width = 30;
    float progress = (float)current / total;

    cout << "\n   Progress: [";
    int pos = width * progress;

    for (int i = 0; i < width; i++) {
        if (i < pos) cout << "█";
        else cout << "-";
    }

    cout << "] " << int(progress * 100) << "%\n";
}

// Question
struct Question {
    string question;
    string options[3];
    int correct;
    string difficulty;
};

// Questions
vector<Question> questions = {
    {"What is C++?", {"Programming Language", "OS", "Game"}, 1, "easy"},
    {"What is OOP?", {"Output Process", "Object Oriented Programming", "None"}, 2, "easy"},
    {"What is Function?", {"Variable", "Loop", "Block of code"}, 3, "easy"},

    {"What is Array?", {"Loop", "Collection of elements", "Condition"}, 2, "medium"},
    {"What is Loop?", {"Variable", "Function", "Repetition"}, 3, "medium"},
    {"Comment symbol?", {"//", "**", "##"}, 1, "medium"},

    {"Valid data type?", {"integer", "int", "number"}, 2, "hard"},
    {"Which is loop?", {"for", "int", "char"}, 1, "hard"},
    {"Which is keyword?", {"class", "apple", "car"}, 1, "hard"},
    {"OOP pillar?", {"Encapsulation", "Cooking", "Driving"}, 1, "hard"}
};

// Rank
string getRank(int p) {
    if (p >= 80) return "Expert";
    else if (p >= 60) return "Intermediate";
    else if (p >= 40) return "Beginner";
    else return "Needs Improvement";
}

// Input thread
void inputThread() {
    cin >> userAnswer;
    answered = true;
}

// Save
void saveResult(string name, int score, int total, int per) {
    ofstream file("interview.txt", ios::app);
    time_t now = time(0);

    file << "\n----------------------------------------\n";
    file << "Name: " << name << endl;
    file << "Score: " << score << "/" << total << endl;
    file << "Result: " << per << "%\n";
    file << "Rank: " << getRank(per) << endl;
    file << "Date: " << ctime(&now);
    file << "----------------------------------------\n";
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

    cout << "\n   Press Enter to go back...";
    cin.ignore();
    cin.get();
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

    srand(time(0) + rand());

    vector<Question> easy, med, hard;

    for (auto &q : questions) {
        if (q.difficulty == "easy") easy.push_back(q);
        else if (q.difficulty == "medium") med.push_back(q);
        else hard.push_back(q);
    }

    random_shuffle(easy.begin(), easy.end());
    random_shuffle(med.begin(), med.end());
    random_shuffle(hard.begin(), hard.end());

    vector<Question> finalQ;

    for (int i = 0; i < 2; i++) finalQ.push_back(easy[i]);
    for (int i = 0; i < 2; i++) finalQ.push_back(med[i]);
    finalQ.push_back(hard[0]);

    random_shuffle(finalQ.begin(), finalQ.end());

    int score = 0;

    for (int i = 0; i < 5; i++) {

        cout << "\n";
        line();

        cout << CYAN << "   Q" << i + 1 << ": " << finalQ[i].question << RESET << endl;

        for (int j = 0; j < 3; j++) {
            cout << "     " << j + 1 << ". " << finalQ[i].options[j] << endl;
        }

        cout << YELLOW << "\n   Answer (10 sec): " << RESET;

        answered = false;
        thread t(inputThread);

        int timeLimit = 10;

        for (int sec = 0; sec < timeLimit; sec++) {
            this_thread::sleep_for(chrono::seconds(1));

            if (answered) break;

            cout << "\r   Time left: " << (timeLimit - sec - 1) << " sec " << flush;
        }

        cout << endl;

        if (answered) {
            t.join();

            if (userAnswer == finalQ[i].correct) {
                cout << GREEN << "   ✔ Correct!\n" << RESET;
                score++;
            } else {
                cout << RED << "   ✘ Wrong!\n" << RESET;
            }
        } else {
            cout << RED << "   ⏱ Time Up! Skipped\n" << RESET;
            t.detach();
        }

        showProgress(i + 1, 5);
    }

    int per = (score * 100) / 5;

    title("FINAL RESULT");

    cout << "   Name     : " << name << endl;
    cout << "   Score    : " << per << "%\n";
    cout << "   Rank     : " << getRank(per) << endl;

    saveResult(name, score, 5, per);
}

// MAIN
int main() {
    int ch;

    do {
        title("SMART INTERVIEW SIMULATOR");

        cout << "   1. Start Interview\n";
        cout << "   2. View History\n";
        cout << "   3. Clear History\n";
        cout << "   4. Exit\n\n";

        cout << "   Enter choice: ";
        cin >> ch;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "\n   Invalid input!\n" << RESET;
            continue;
        }

        switch (ch) {
            case 1: startInterview(); break;
            case 2: viewHistory(); break;
            case 3: clearHistory(); break;
            case 4: cout << YELLOW << "\n   Exiting...\n" << RESET; break;
            default: cout << RED << "\n   Invalid choice!\n" << RESET;
        }

    } while (ch != 4);

    cout << GREEN << "\n   Thank You!\n\n" << RESET;
    return 0;
}
