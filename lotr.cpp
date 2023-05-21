#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

class incorrect_amount_error : public exception{
    const char *message;
    public:
        incorrect_amount_error(const char* message): message(message) {}
        const char* what() const noexcept override{ return  message;}
};


class Soldado{
    string nome;
    int poder_ataque;
    int saude;
    int stamina; // will be used for special attacks. Success rate and damage will rely on this statistic

protected:
    void set_nome(string nome) { nome = nome; }
    void set_saude(int saude) { saude = saude; }
    void set_poder_ataque(int pa) { poder_ataque = pa; }

public:
    Soldado(string nome, int hp, int pa) : nome(nome), poder_ataque(pa), saude(hp) {}
    Soldado(const Soldado &other) = delete;

    virtual string get_nome() { return nome; }
    virtual int get_poder_ataque() { return poder_ataque; }
    virtual int get_saude() { return saude; }
    virtual void defesa(int pa) { saude -= pa; }
    virtual void ataque(Soldado *other) { other->defesa(this->get_poder_ataque()); }
    virtual bool is_alive() { return saude > 0; }
    virtual void has_revived() {}


};

class Elfo: public Soldado{
    public:
        
        Elfo(string nome, int hp, int pa): Soldado(nome, hp, pa+1) {}
        
        Elfo(const Elfo& other)= delete;
        
        void ataque(Soldado* other) override { if(get_saude() > 0)other->defesa(this->get_poder_ataque()); }
        
        void long_range(){}
        
        void special(){}

};

class Anao: public Soldado{
    public:
        
        Anao(string nome, int hp, int pa) : Soldado(nome, hp, pa + 20) {}
        
        Anao(const Anao& other)= delete;
        
        void ataque(Soldado* other) override {
            if (get_saude() > 0) return; 
            if (!(rand() % 5 <= 1))
            other->defesa(this->get_poder_ataque());
        }

        void rage_beneath_the_mountains(){}
};

class Humano: public Soldado{
    
    
    public:
        Humano(string nome, int hp, int pa) : Soldado(nome, hp, pa) {
        }
        
        Humano(const Humano& other)= delete;
        
        void ataque(Soldado* other) override {
            if(get_saude()<=0) return;
            other->defesa(this->get_poder_ataque()); 
            if(!(rand()%10)) other->defesa(this->get_poder_ataque());
        }

        // Special treatment to human called Cid Kagenou
        // Special attacks: Battle Mode (Playtime is over), I AM ATOMIC, 
};

class Eminence: public Humano {
    static size_t num_obj;
    size_t war_mode; // turns Cid immune to damage and with double damage and attack speed for 3 rounds
    size_t HEART_BREAK;

    public:

    Eminence(int hp, int pa) : Humano("Shadow", hp, pa), war_mode(), HEART_BREAK() {
        if(num_obj) throw incorrect_amount_error("There can only be one Eminence in Shadow");
        num_obj++;
    }

    ~Eminence() { num_obj--;}

    enum class ATOMIC_MODE{
        NORMAL,   // lot of dmg to all enemies
        RECOVERY, // heal allies
        SWORD,    // probably instakill in 1 enemy
        UTSUSEMI  // massive dmg for 9 enemies
    };

    void I_AM_ATOMIC(ATOMIC_MODE mode = ATOMIC_MODE::NORMAL){
            cout << "\x1B[3m\x1B[1mPlaytime is over\x1B[0m" << endl;
            this_thread::sleep_for(chrono::seconds(1));
            cout << "The area is filled with a bluish magic" << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
            cout << "Allies and foes alike all stopped battling, and silently, fearfully stared at the source of the great magic" << endl;
            this_thread::sleep_for(chrono::milliseconds(2500));
            cout << "Let the true meaning of almighty be carved in your soul..." << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
            cout << "I..." << endl;
            this_thread::sleep_for(chrono::milliseconds(1000));
            cout << "\x1B[1mAM...\x1B[0m" << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));

            if (mode == ATOMIC_MODE::RECOVERY){
            cout << "\x1B[1mTHE RECOVERY...\x1B[0m" << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
            }

            if (mode == ATOMIC_MODE::UTSUSEMI){
            cout << "\x1B[1mTHE UTSUSEMI...\x1B[0m" << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
            }

            if (mode == ATOMIC_MODE::SWORD)
                cout << "\x1B[3mthe\x1B[0m ";

            cout << "\x1B[3matomic\x1B[0m ";

            if (mode == ATOMIC_MODE::SWORD || mode == ATOMIC_MODE::UTSUSEMI){
            cout << "\x1B[3msword\x1B[0m";
            this_thread::sleep_for(chrono::milliseconds(1500));
            }
            // this_thread::sleep_for(chrono::seconds(2));
            // cout << "A great explosion takes place" << endl;
    }

    void defesa(int pa) override {
        if(war_mode || HEART_BREAK) return;
        set_saude(get_saude()-pa/2);
        if(rand() % 100 == 0) heart_break();
    } 

    void overdrive(){
        cout << get_nome() << "'s blood is boiling..." << endl;
        set_poder_ataque(2*get_poder_ataque());
    }

    void heart_break(){

    }

    void normal(){

    }

    void ataque(Soldado* other) override{
        if(HEART_BREAK){
            HEART_BREAK--;
            return;
        }

        if(!war_mode && rand()%100 == 0){
            war_mode = 3;
            overdrive();
        }

        if(rand()%200)
            Humano::ataque(other);
        else{
            int mode = rand()%4;
            ATOMIC_MODE am = static_cast<ATOMIC_MODE>(mode); 
            I_AM_ATOMIC(am);
        }
    
    }
};


class Balrog: public Soldado{
    
    static size_t count;

    // Balrog(){
    //     if(count == 7) throw runtime_error("No more than 7 Balrogs survived the War of Wrath");
    //     count++;
    // }

    // ~Balrog(){}

    void defesa(int pa) override{
        if(rand()%10) Soldado::defesa(pa);
    }

    void berserk(){
        cout << "\x1B[3mThe air is getting warmer around " << get_nome() << "\x1B[0m" << endl;
    }

    void ataque(Soldado* other) override{

    }

    void long_range_attack(){

    }

};

class Sauron: public Soldado{


    public:
    static size_t count;
        Sauron(int hp, int pa) : Soldado("Sauron", 10 * hp, pa){
            count++;
            if(count == 2) throw incorrect_amount_error("There can only be one Dark Lord");
        }
        Sauron(const Sauron &other) = delete;
        
        ~Sauron(){count--;}
        
        void ataque(Soldado *other) {
            if(get_saude()<=0) return;
            if(rand() % 10 <=2){
                other->defesa(2 * this->get_poder_ataque());
                other->defesa(2 * this->get_poder_ataque());
            } else other->defesa(this->get_poder_ataque());
        }

};

class Orc: public Soldado{
    public:
        Orc(string nome, int pa, int hp): Soldado(nome, hp, pa) {}
        Orc(const Orc& other)= delete;
        void ataque(Soldado *other) {
            if(rand() % 10 <= 1){
                other->defesa(this->get_poder_ataque()*1.1);
                other->defesa(this->get_poder_ataque()*1.1);
            } else other->defesa(this->get_poder_ataque());
        }
};

class ReiBruxo{

};



class Mago: public Soldado{
    
    bool revived;
    
    int mana; // will be used for special attacks

    public:
        Mago(string nome, int pa, int hp) : Soldado(nome, hp, pa), revived(false) {}
        Mago(const Mago& other)= delete;

        void ataque(Soldado *other) {
            if(!revived && get_saude() <=0 && rand()%20){
                set_saude(20);
                revived = true;
            }
            if(get_saude() <=0) return;
            
            int random_var = rand();
            if (random_var % 25 == 0){
                defesa(this->get_poder_ataque());
            }
            else if (random_var % 10 == 0){
                other->defesa(this->get_poder_ataque() * 1.5);
            } else
                other->defesa(this->get_poder_ataque());
        }

        void avada_kedavra(){}

        void armiger_verum_rex(){}

        // 2 more magic spells

        void has_revived(){
            if(revived) cout << "The mage has revived" << endl;
            else cout << "The mage has not revived" << endl;
        }
};

class Vector{
    Soldado** arr;
    size_t sz;
    size_t cp;

    public:
        Vector():cp(40), sz(), arr(new Soldado*[cp]) {}

        ~Vector() {erase(); delete[] arr;}

        Vector(const Vector& other) = delete;
        void push_back();

        Soldado* pop_back(){return nullptr;}
        void erase(){
            for(size_t i=0; i < sz; i++) pop_back();
            delete[] arr;
            arr = new Soldado*[cp];
        }

        Soldado* operator[](size_t index){
            if(index >= sz) throw logic_error("There is no " + to_string(index) + " element in this Vector");
            return arr[index];
        }

};

class Menu{

    bool game_over;

    enum class gameplay_style{
        GOOD,
        EVIL,
        NPC
    }; // sets the side the player is controlling (if he is playing)

    Vector sauron_army;
    Vector elf_army;

    public:

        Menu(){

        }

        Menu(const Menu& other) = delete;

        void run(){

        }

        void instantiate(){

        }

        bool over(){ return game_over;}

        void log(){

        }

};


size_t Eminence::num_obj = 0;

int main(){ 
    srand(time(NULL));
    Menu game;
    Soldado* cid = new Eminence(10, 10);
    cid->ataque(cid);
    // cid->I_AM_ATOMIC();
    // Sauron s1(10,10);
    // Sauron s2(10,10);
    
    
    // proof of concept: printing and erasing characters
    // cout.sync_with_stdio(false);
    // for(int i = 0; i < 10; i++){
    //     cout << i << flush;
    //     this_thread::sleep_for(chrono::milliseconds(100));
    //     cout << "\b" << flush;
    // }
    // cout.sync_with_stdio(true);
    
    
    
    
    // do{
    // game.instantiate();
    // game.run();
    // } while(!game.over());
    
}