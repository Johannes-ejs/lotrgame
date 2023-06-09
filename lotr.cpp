#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

// class incorrect_amount_error : public exception{
//     const char *message;
//     public:
//         incorrect_amount_error(const char* message): message(message) {}
//         const char* what() const noexcept override{ return  message;}
// };


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
    virtual void true_damage(int pa) { saude -= pa; }
    virtual void ataque(Soldado *other) { other->defesa(this->get_poder_ataque()); }
    virtual bool is_alive() { return saude > 0; }
    virtual void has_revived() = 0;


};

class Elfo: public Soldado{
    public:
        
        Elfo(string nome, int hp, int pa): Soldado(nome, hp, pa+1) {}
        
        Elfo(const Elfo& other)= delete;
        
        void ataque(Soldado* other) override { if(get_saude() > 0)other->defesa(this->get_poder_ataque()); }        

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

};

class Eminence: public Humano {
    size_t WAR_MODE; // turns Shadow immune to damage and with double damage and attack speed for 3 rounds
    size_t HEART_BREAK;

    public:

    Eminence(int hp, int pa) : Humano("Shadow", hp, pa), WAR_MODE(), HEART_BREAK() {}

    ~Eminence() {}

    void I_AM_ATOMIC(Soldado* other){
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
        cout << "\x1B[3matomic\x1B[0m ";
        other->true_damage(999999);
        // this_thread::sleep_for(chrono::seconds(2));
        // cout << "A great explosion takes place" << endl;
    }

    void defesa(int pa) override {
        if(WAR_MODE || HEART_BREAK) return;
        set_saude(get_saude()-pa/2);
        if(rand() % 100 == 0) heart_break();
    } 

    void overdrive(){
        cout << "Now... " << flush; 
        this_thread::sleep_for(chrono::milliseconds(700));
        cout << "\x1B[1mI am a little motivated\x1B[0m" << endl;
        set_poder_ataque(2*get_poder_ataque());
    }

    void heart_break(){
        // too lazy to implement
    }

    void normal(){
        // too lazy to implement
    }

    void ataque(Soldado* other) override{
        if(HEART_BREAK){
            HEART_BREAK--;
            return;
        }

        if(!WAR_MODE && rand()%100 == 0){
            WAR_MODE = 3;
            overdrive();
        }

        if(rand()%200)
            Humano::ataque(other);
        else{
            I_AM_ATOMIC(other);
        }
    
    }
};


class Balrog: public Soldado{
    

    Balrog(string nome, int hp, int pa): Soldado(nome, hp, pa){}


    void defesa(int pa) override{
        if(rand()%10) Soldado::defesa(pa);
    }

    void berserk(){
        // below certain amount of health, Balrog will permanently be berserk, dealing only true damage
        cout << "\x1B[3mThe air is getting warmer around " << get_nome() << "..." <<"\x1B[0m" << endl;
    }

    void ataque(Soldado* other) override{
        // idk what to do
    }

    // void long_range_attack(){
        // idk what to do
    // }

};

class Sauron: public Soldado{


    public:
    // static size_t count;
        Sauron(int hp, int pa) : Soldado("Sauron", 10 * hp, pa){}
        Sauron(const Sauron &other) = delete;
        
        ~Sauron(){}
        
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

        void avada_kedavra() {} // Declare in Soldado

        void armiger_verum_rex() {} // Declare in Soldado

        void resurrectio(){} // Declare in Soldado

        void has_revived(){
            if(revived) cout << "The mage has revived" << endl;
            else cout << "The mage has not revived" << endl;
        }
};


class Menu{

    bool GAME_OVER;

    vector<Soldado*> sauron_army;
    vector<Soldado*> elf_army;

    public:

        Menu(){
            srand(time(NULL));
            instantiate();
        }

        Menu(const Menu& other) = delete;

        int prelude(){
            int i;
            cout << "\x1B[32mWelcome to Lord of The Rings: ... \x1B[0m" << endl;
            do{
                cout <<  "How many characters would you like to have in each army?" << endl;
                cin >> i;
                cout << "The number must be positive! Let's try again" << flush;
                for (int i = 0; i < 3; i++){
                    this_thread::sleep_for(chrono::milliseconds(300));
                    cout << "." << flush;
                }
                this_thread::sleep_for(chrono::milliseconds(500));

            } while (i <= 0);
            return i;
        }

        void instantiate(){
            int size = prelude();
            sauron_army.clear();
            elf_army.clear();
            // Only one Witch King, Sauron and Eminence allowed
        }

        void log(){

        }

        void run(){

        }

        void final_results(){

        }

        void end_game(){
            char c;
            cout << "Would you like to play again? (Y/N) ";
            cin >> c;
            if(c != 'Y'){
                cout << "Alright. Bye!" << endl;
                GAME_OVER = true;
            } else{
                cout << "Alright! Restarting session" << flush;
                for(int i=0; i< 3; i++){
                    this_thread::sleep_for(chrono::milliseconds(300));
                    cout << "." << flush;
                }
                this_thread::sleep_for(chrono::milliseconds(500));
                instantiate();
            }
        }

        bool over(){ return GAME_OVER;}

};


// size_t Eminence::num_obj = 0;

int main(){ 
    Menu game;
    game.end_game();
    // do{
    //     game.run();
    // } while(!game.over())
    
    
    // proof of concept: printing and erasing characters
    // cout.sync_with_stdio(false);
    // for(int i = 0; i < 10; i++){
    //     cout << i << flush;
    //     this_thread::sleep_for(chrono::milliseconds(100));
    //     cout << "\b" << flush;
    // }
    // cout.sync_with_stdio(true);    
}