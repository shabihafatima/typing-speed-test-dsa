#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <conio.h>    
using namespace std;


 //  UTILITY CLASS
 
class Utils
{
public:

    static void clearScreen()
    {
        system("cls");
    }

    static void pauseSystem()
    {
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    static void centerBetweenLines(string text)
    {
        int width = 33;
        int pad = (width - text.length()) / 2;

        cout << "=================================\n";
        cout << string(pad, ' ') << text << "\n";
        cout << "=================================\n";
    }
};


 //  USER MANAGEMENT (Linked List)
  
class UserManager
{
    struct UserNode
    {
        string username;
        string password;
        UserNode* next;
    };

    UserNode* head;

public:

    UserManager()
    {
        head = NULL;
    }

    void loadUsers()
    {
        ifstream file("users.txt");
        string u, p;

        while (file >> u >> p)
        {
            head = new UserNode{u, p, head};
        }

        file.close();
    }

    bool userExists(string uname)
    {
        for (UserNode* t = head;
		 t;
		  t = t->next)
        {
            if (t->username == uname)
                return true;
        }
        return false;
    }

    bool verifyLogin(string uname, string pass)
    {
        for (UserNode* t = head; t; t = t->next)
        {
            if (t->username == uname && t->password == pass)
                return true;
        }
        return false;
    }

    void registerUser()
    {
        Utils::clearScreen();
        Utils::centerBetweenLines("REGISTER");

        string uname, pass;

        cout << "Enter Username: ";
        cin >> uname;

        if (userExists(uname))
        {
            cout << "Username already exists!\n";
            Utils::pauseSystem();
            return;
        }

        cout << "Enter Password: ";
        cin >> pass;

        ofstream file("users.txt", ios::app);
        file << uname << " " << pass << endl;
        file.close();

        head = new UserNode{uname, pass, head};

        cout << "Registration Successful!\n";
        Utils::pauseSystem();
    }

    void forgotPassword()
    {
        Utils::clearScreen();
        Utils::centerBetweenLines("FORGOT PASSWORD");

        string uname;
        cout << "Enter Username: ";
        cin >> uname;

        for (UserNode* t = head; t; t = t->next)
        {
            if (t->username == uname)
            {
                cout << "Your Password is: " << t->password << endl;
                Utils::pauseSystem();
                return;
            }
        }

        cout << "User not found!\n";
        Utils::pauseSystem();
    }
};

  // QUEUE CLASS (Words)
  
class WordQueue
{
    struct Node
    {
        string word;
        Node* next;
    };

    Node* front;
    Node* rear;

public:

    WordQueue()
    {
        front = rear = NULL;
    }

    void enqueue(string w)
    {
        Node* n = new Node{w, NULL};

        if (!rear)
            front = rear = n;
        else
        {
            rear->next = n;
            rear = n;
        }
    }

    string dequeue()
    {
        string w = front->word;
        Node* t = front;

        front = front->next;
        delete t;

        if (!front)
            rear = NULL;

        return w;
    }

    bool empty()
    {
        return front == NULL;
    }

    string peek()
    {
        return front->word;
    }
};


//   STACK CLASS (Mistakes)
 
class MistakeStack
{
    struct Node
    {
        string word;
        Node* next;
    };

    Node* top;

public:

    MistakeStack()
    {
        top = NULL;
    }

    void push(string w)
    {
        top = new Node{w, top};
    }

    bool isEmpty()
    {
        return top == NULL;
    }

    void display()
    {
        Node* t = top;
        while (t)
        {
            cout << "- " << t->word << endl;
            t = t->next;
        }
    }
};


//   GAME CLASS
  
class TypingGame
{
public:

    void saveScore(string user, double wpm)
    {
        ofstream file("leaderboard.txt", ios::app);
        file << user << " " << wpm << endl;
        file.close();
    }

    void showHistory(string user)
    {
        Utils::clearScreen();
        Utils::centerBetweenLines("MY HISTORY");

        ifstream file("leaderboard.txt");
        string u;
        double w;

        int count = 0;
        double total = 0;
        double maxWPM = 0;
        bool found = false;

        while (file >> u >> w)
        {
            if (u == user)
            {
                cout << "WPM: " << w << endl;
                total += w;
                if (w > maxWPM) maxWPM = w;
                count++;
                found = true;
            }
        }

        if (!found)
            cout << "No previous record.\n";
        else
        {
            cout << "\nTotal Games: " << count << endl;
            cout << "Average WPM: " << total / count << endl;
            cout << "Highest WPM: " << maxWPM << endl;
        }

        file.close();
        Utils::pauseSystem();
    }

    void start(string user)
    {
    RESTART:

        Utils::clearScreen();
        Utils::centerBetweenLines("SELECT LEVEL");

        cout << "1. Beginner\n2. Advanced\n3. Expert\nChoice: ";

        int level;
        cin >> level;

        WordQueue q;

        if (level == 1)
        {
            q.enqueue("cat");
            q.enqueue("dog");
            q.enqueue("pen");
            q.enqueue("book");
            q.enqueue("mouse");
            q.enqueue("ball");
        }
        else if (level == 2)
        {
            q.enqueue("algorithm");
            q.enqueue("structure");
            q.enqueue("recursion");
            q.enqueue("complexity");
            q.enqueue("pointer");
            q.enqueue("function");
        }
        else
        {
            q.enqueue("implementation");
            q.enqueue("optimization");
            q.enqueue("computational");
            q.enqueue("objectoriented");
            q.enqueue("abstraction");
            q.enqueue("polymorphism");
            q.enqueue("encapsulation");
            q.enqueue("inheritance");
            q.enqueue("modularity");
        }

        MistakeStack mistakes;

        int total = 0;
        int correct = 0;
        int streak = 0;
        int maxStreak = 0;

        clock_t start = clock();
        const int TIME_LIMIT = 60;

        Utils::clearScreen();
        Utils::centerBetweenLines("TYPING TEST START");

        cout << "You have 60 seconds. Type the words as fast as you can!\n";

        bool timeOver = false;

        while (!q.empty() && !timeOver)
        {
            string word = q.peek();
            cout << "Type: " << word << " -> ";

            string input = "";
            char ch;

            while (true)
            {
                if (_kbhit())
                {
                    ch = _getch();

                    if (ch == 13)
                    {
                        cout << endl;
                        break;
                    }
                    else if (ch == 8)
                    {
                        if (!input.empty())
                        {
                            input.pop_back();
                            cout << "\b \b";
                        }
                    }
                    else
                    {
                        input.push_back(ch);
                        cout << ch;
                    }
                }

                double elapsed = double(clock() - start) / CLOCKS_PER_SEC;
                if (elapsed >= TIME_LIMIT)
                {
                    timeOver = true;
                    cout << "\n\n--- TIME OVER ---\n";
                    break;
                }
            }

            if (timeOver)
                break;

            q.dequeue();
            total++;

            if (input == word)
            {
                correct++;
                streak++;
                if (streak > maxStreak)
                    maxStreak = streak;
            }
            else
            {
                mistakes.push(word);
                streak = 0;
            }
        }

        double elapsedTime = double(clock() - start) / CLOCKS_PER_SEC;
        double minutes = min(elapsedTime / 60.0, TIME_LIMIT / 60.0);

        double wpm = (minutes > 0) ? correct / minutes : 0;
        double accuracy = (total > 0) ? double(correct) / total * 100 : 0;

        saveScore(user, wpm);

        string msgs[] =
        {
            "Excellent! Keep Practicing!",
            "Great Progress! Accuracy is key!",
            "Well Done! Type even faster next time!",
            "Nice effort! Stay consistent!"
        };
        Utils::clearScreen();
        Utils::centerBetweenLines("RESULT");

        cout << "Total Words Attempted: " << total << endl;
        cout << "Correct Words: " << correct << endl;
        cout << "Longest Correct Streak: " << maxStreak << endl;
        cout << "Time Elapsed: " << elapsedTime << " sec" << endl;
        cout << "WPM: " << wpm << endl;
        cout << "Accuracy: " << accuracy << "%" << endl;

        if (timeOver)
            cout << "--- GAME OVER (Time Limit Reached) ---\n";
        cout << msgs[rand() % 4] << endl;

        if (!mistakes.isEmpty())
        {
            cout << "\nMistyped Words:\n";
            mistakes.display();
        }
        char c;
        cout << "\nPlay Again? (y/n): ";
        cin >> c;

        if (c == 'y' || c == 'Y')
            goto RESTART;
    }
};


//   MAIN FUNCTION
  
int main()
{
    UserManager users;
    TypingGame game;

    users.loadUsers();

    while (true)
    {
        Utils::clearScreen();
        Utils::centerBetweenLines("WELCOME");

        cout << "1. Login\n2. Register\n3. Forgot Password\n4. Exit\nChoice: ";

        int c;
        cin >> c;

        if (c == 4)
            break;

        else if (c == 2)
            users.registerUser();

        else if (c == 3)
            users.forgotPassword();

        else if (c == 1)
        {
            string u, p;

            Utils::clearScreen();
            Utils::centerBetweenLines("LOGIN");

            cout << "Username: ";
            cin >> u;

            cout << "Password: ";
            cin >> p;

            if (!users.verifyLogin(u, p))
            {
                cout << "Invalid Login!\n";
                Utils::pauseSystem();
            }
            else
            {
                int op;

                do
                {
                    Utils::clearScreen();
                    Utils::centerBetweenLines("DASHBOARD");

                    cout << "1. Start Game\n2. My History\n3. Logout\nChoice: ";
                    cin >> op;

                    if (op == 1)
                        game.start(u);
                    else if (op == 2)
                        game.showHistory(u);

                } while (op != 3);
            }
        }
    }

    Utils::clearScreen();
    Utils::centerBetweenLines("THANK YOU");
    cout << "Practice Makes Perfect!\n";

    return 0;
}
