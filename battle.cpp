#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

class Player
{
public:
    string username;
    int hp, maxhp, attack, defense, level, exp, gold;

    Player(string nama)
    {
        username = nama;
        maxhp = 100;
        hp = maxhp;
        attack = 15;
        defense = 5;
        level = 1;
        exp = 0;
        gold = 50;
    }

    bool isAlive() { return hp > 0; }

    void terimaDamage(int damage)
    {
        hp -= damage;

        if (hp < 0)
            hp = 0;
    }
    void heal(int jumlah)
    {
        hp += jumlah;
        if (hp > maxhp)
            hp = maxhp;
    }
    void tambahGold(int jumlah)
    {
        gold += jumlah;
    }
    bool bayar(int jumlah)
    {
        if (gold < jumlah)
            return false;
        gold -= jumlah;
        return true;
    }
    int expbutuh() { return level * 50; }

    void naillevel()
    {
        level++;
        maxhp += 20;
        attack += 4;
        defense += 2;
        hp = maxhp;
        cout << "\n*** LEVEL UP! Sekarang Level " << level << " ***\n";
        cout << "Max HP " << maxhp << " | Attack " << attack << " | Defense " << defense << "\n";
    }

    void tambahexp(int jumlah)
    {
        exp += jumlah;
        while (exp >= expbutuh())
        {
            exp -= expbutuh();
            naillevel();
        }
    }

    void tampilStatus()
    {
        cout << "\n=== STATUS PLAYER ===\n"
             << username << "===\n";
        cout << "Level :  " << level << "\n";
        cout << "HP :  " << hp << "/" << maxhp << "\n";
        cout << "Attack : " << attack << "\n";
        cout << "Defense : " << defense << "\n";
        cout << "EXP : " << exp << "\n"
             << expbutuh() << "\n";
        cout << "GOLD : " << gold << "\n";

        cout << "=====================\n";
    }
};

class Enemy
{
public:
    string nama;
    int hp, maxhp, attack, defense, expreward, goldreward;

    Enemy(string hedo, int h, int atk, int def, int exp, int gold)
    {
        nama = hedo;
        hp = maxhp = h;
        attack = atk;
        defense = def;
        expreward = exp;
        goldreward = gold;
    }

    bool isAlive() { return hp > 0; }

    void terimaDamage(int damage)
    {
        hp -= damage;
        if (hp < 0)
            hp = 0;
    }
};

const int harga_potion = 10;
const int harga_armor = 40;

enum hasilbattle
{
    MENANG,
    KALAH,
    KABUR
};

int inputangka()
{
    int x;
    while (!(cin >> x))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input harus berupa angka! Coba lagi : ";
    }
    return x;
}

int hitungdamage(int attack, int defense)
{
    int dmg = attack - defense + (rand() % 5 - 2);
    return dmg < 1 ? 1 : dmg;
}

void giliranenemy(Player &p, Enemy &e)
{
    int dmg = hitungdamage(e.attack, p.defense);
    p.terimaDamage(dmg);
    cout << e.nama << " menyerang! Kamu menerima " << dmg << " damage,\n";
}

bool giliranplayer(Player &p, Enemy &e, bool &kabur)
{
    cout << "\n1. Attack\n2. Heal (" << harga_potion << " GOLD)\n3. Kabur\nPilihanmu : ";
    int pilihan = inputangka();
    switch (pilihan)
    {
    case 1:
    {
        int dmg = hitungdamage(p.attack, e.defense);
        e.terimaDamage(dmg);
        cout << "Kamu menyerang " << e.nama << " sebesar " << dmg << " DAMAGE!\n";
        return true;
    }
    case 2:
    {
        if (p.hp == p.maxhp)
        {
            cout << "HP sudah penuh!\n";
            return false;
        }
        if (!p.bayar(harga_potion))
        {
            cout << "Gold tidak cukup untuk membeli potion heal!\n";
            return false;
        }
        p.heal(harga_potion);
        cout << "Kamu memakai potion. HP sekarang " << p.hp << "/" << p.maxhp << "\n";
        return true;
    }

    case 3:
    {
        if (rand() % 100 < 50)
        {
            cout << "Kamu berhasil kabur!\n";
            kabur = true;
        }
        else
        {
            cout << "Gagal kabur@\n";
        }
        return true;
    }

    default:
        cout << "Pilihan tidak valid!\n";
        return false;
    }
}

hasilbattle mulaibattle(Player &p, Enemy &e)
{
    cout << "\n=== BATTLE DIMULAI ===\n"
         << p.username << " VS " << e.nama << "\n";

    while (p.isAlive() && e.isAlive())
    {
        cout << "\n===============\n";
        cout << p.username << " | HP : " << p.hp << "/" << p.maxhp << "\n";
        cout << e.nama << " | HP : " << e.hp << "/" << e.maxhp << "\n";

        bool kabur = false;
        if (!giliranplayer(p, e, kabur))
            continue;
        if (kabur)
            return KABUR;
        if (!e.isAlive())
            break;

        giliranenemy(p, e);
    }

    if (!p.isAlive())
    {
        cout << "\nKamu dikalahkan oleh : " << e.nama << "\n";
        return KALAH;
    }

    cout << "\n Kamu mengalahkan : " << e.nama << "!\n";
    cout << "HADIAH: +" << e.expreward << " EXP / +" << e.goldreward << " GOLD\n";
    p.tambahGold(e.goldreward);
    p.tambahexp(e.expreward);
    return MENANG;
}

int main()
{
    srand(time(0));
    cout << "===START===" << endl;
    cout << "ENTER USERNAME : ";
    string username;
    cin >> username;
    Player player(username);
    player.tampilStatus();

    Enemy musuh("SLIME", 40, 10, 2, 30, 15);
    mulaibattle(player, musuh);
    player.tampilStatus();
    return 0;
}
