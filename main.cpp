#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;
class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int gold;
    vector<string> inventory;

public:
    Character(string n, int h)
        : name(n), health(h), maxHealth(h), gold(30) {}

    string getName() const { return name; }
    int getHealth() const { return health; }
    int getGold() const { return gold; }

    void addGold(int amount) {
        gold += amount;
        if (gold < 0) gold = 0;
    }

    bool hasItem(const string& item) const {
        return find(inventory.begin(), inventory.end(), item) != inventory.end();
    }

    void addItem(const string& item) {
        inventory.push_back(item);
    }

    bool usePotion() {
        auto it = find(inventory.begin(), inventory.end(), "Potion");
        if (it != inventory.end()) {
            health = min(maxHealth, health + 40);
            inventory.erase(it);
            return true;
        }
        return false;
    }

    void takeDamage(int dmg) {
        health = max(0, health - dmg);
    }

    bool isAlive() const { return health > 0; }

    virtual string getType() const = 0;

    void showStats() const {
        cout << name << " (" << getType() << ")\n";
        cout << "Health: ";
        printHealthBar(health, maxHealth);
        cout << "\n";

        cout << "Gold: " << gold << "    Items: ";
        if (inventory.empty()) cout << "(empty)";
        else {
            for (size_t i = 0; i < inventory.size(); ++i) {
                cout << inventory[i];
                if (i < inventory.size() - 1) cout << ", ";
            }
        }
        cout << "\n\n";
    }

    void printHealthBar(int current, int max) const {
        const int BAR_WIDTH = 15;
        int filled = (current * BAR_WIDTH) / max;

        cout << "[";
        for (int i = 0; i < filled; i++) cout << "#";
        for (int i = filled; i < BAR_WIDTH; i++) cout << " ";
        cout << "] " << current << "/" << max;
    }
};

class Warrior : public Character {
public:
    Warrior(string n) : Character(n, 120) { gold = 20; }
    string getType() const override { return "Warrior"; }
};

class Rogue : public Character {
public:
    Rogue(string n) : Character(n, 90) { gold = 50; }
    string getType() const override { return "Rogue"; }
};

class Mage : public Character {
public:
    Mage(string n) : Character(n, 100) {}
    string getType() const override { return "Mage"; }
};

class Goblin {
public:
    string name = "Goblin";
    int health = 35;
    int attack = 8;

    bool isAlive() const { return health > 0; }

    void takeDamage(int dmg) {
        health = max(0, health - dmg);
    }
};

class Game {
private:
    Character* player;
    bool hasKey;

    void simpleCombat() {
        cout << "\n!!! SUDDEN AMBUSH !!!\n";
        cout << "A wild Goblin appears!\n\n";

        Goblin goblin;

        while (player->isAlive() && goblin.isAlive()) {
            player->showStats();
            cout << "1 - Attack   2 - Use Potion   3 - Run\n";
            cout << "> ";

            string action;
            getline(cin, action);

            if (action == "1") {
                int damage = 12 + (rand() % 9);
                cout << "You hit the goblin for " << damage << " damage!\n";
                goblin.takeDamage(damage);
            }
            else if (action == "2") {
                if (player->usePotion())
                    cout << "Potion used.\n";
                else
                    cout << "No potion available.\n";
            }
            else if (action == "3") {
                if (rand() % 100 < 60) {
                    cout << "You escaped successfully.\n";
                    return;
                } else {
                    cout << "Escape failed!\n";
                }
            }
            else {
                cout << "Invalid choice.\n";
            }

            if (!goblin.isAlive()) break;

            cout << "Goblin attacks for " << goblin.attack << " damage.\n";
            player->takeDamage(goblin.attack);

            if (!player->isAlive()) {
                cout << "\nYou were defeated. Game Over.\n";
                exit(0);
            }

            cout << "----------------------------------------\n";
        }

        cout << "\nYou defeated the Goblin!\n";
        cout << "You found 15 gold and a Potion.\n";
        player->addGold(15);
        player->addItem("Potion");
    }

public:
    Game() : player(nullptr), hasKey(false) {
        srand(time(0));
    }

    ~Game() { delete player; }

    void start() {
        cout << "\n=== Tiny Console Adventure ===\n\n";

        cout << "Your name: ";
        string name;
        getline(cin, name);

        cout << "\nChoose your class:\n";
        cout << "1 - Warrior\n2 - Rogue\n3 - Mage\n> ";

        int cls;
        cin >> cls;
        cin.ignore();

        if (cls == 1) player = new Warrior(name);
        else if (cls == 2) player = new Rogue(name);
        else player = new Mage(name);

        cout << "\nYou wake up in a mysterious forest...\n";
        gameLoop();
    }

private:
    void gameLoop() {
        while (true) {
            cout << "----------------------------------------\n";
            player->showStats();

            cout << "1 - Search forest\n";
            cout << "2 - Visit merchant\n";
            cout << "3 - Enter ancient temple\n";
            cout << "4 - Quit\n> ";

            string input;
            getline(cin, input);

            if (input == "1") {
                cout << "You search the area...\n";
                if (!hasKey) {
                    cout << "You found an old key!\n";
                    player->addItem("Key");
                    hasKey = true;
                } else {
                    cout << "Nothing interesting.\n";
                }

                if (rand() % 100 < 40)
                    simpleCombat();
            }
            else if (input == "2") {
                cout << "Potion costs 20 gold. Buy? (p / no)\n> ";
                string buy;
                getline(cin, buy);

                if ((buy == "p" || buy == "P") && player->getGold() >= 20) {
                    player->addItem("Potion");
                    player->addGold(-20);
                    cout << "Potion purchased.\n";
                } else {
                    cout << "No purchase made.\n";
                }
            }
            else if (input == "3") {
                if (hasKey) {
                    cout << "\nThe key works! You found treasure.\n";
                    cout << "*** YOU WIN – ACT 1 COMPLETE ***\n";
                    return;
                } else {
                    cout << "The door is locked.\n";
                }
            }
            else if (input == "4") {
                cout << "See ya, adventurer.\n";
                return;
            }
            else {
                cout << "Invalid option.\n";
            }
        }
    }
};

int main() {
    Game game;
    game.start();
    return 0;
}
