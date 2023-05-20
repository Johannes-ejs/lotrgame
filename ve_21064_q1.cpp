#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

class incorrect_amount_error : public exception{
    string message;
    public:
        incorrect_amount_error(const char* message): exception(), message(message) {}
        const char* what(){ return message.c_str(); }
};


class Soldado{
    string nome;
    int poder_ataque;
    int saude;
    
    protected:
        void set_nome(string nome) { nome = nome; }
        void set_saude(int saude) { saude = saude; }
        void set_poder_ataque(int pa) { poder_ataque = pa; }

    public: 

        Soldado(string nome, int hp, int pa): 
            nome(nome), poder_ataque(pa), saude(hp) {}
        Soldado(const Soldado& other) = delete;
            
        virtual string get_nome() { return nome; }
        virtual int get_poder_ataque() { return poder_ataque; }
        virtual int get_saude() { return saude; }
        virtual void defesa(int pa){ saude -= pa;}
        virtual void ataque(Soldado* other){ other->defesa(this->get_poder_ataque());}
        virtual bool is_alive() { return saude >0; }
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

    static size_t cid;
    static size_t vergil;
    static size_t talion;
    static size_t minuano;
    
    int stamina; // will be used for special attacks
    
    public:
        Humano(string nome, int hp, int pa) : Soldado(nome, hp, pa) {
            // if(cid == 2) throw incorrect_amount_error("There can only be one Eminence in Shadows");
            // if(vergil == 2) throw incorrect_amount_error("There can only be one ");
            // if(talion == 2) throw incorrect_amount_error("There can only be one undying one");
            // if(minuano == 2) throw incorrect_amount_error("There can only be one brazilian");
        }
        

        void I_AM_ATOMIC(){
            cout << "\x1B[3m\x1B[1mPlaytime is over\x1B[0m" << endl;
            this_thread::sleep_for(chrono::seconds(1));
            cout << "The area is filled with a bluish magic" << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
            cout << "Allies and foes alike all stopped battling, and silently, fearfully stared at the source of the great magic" << endl;
            this_thread::sleep_for(chrono::milliseconds(2500));
            cout << "Let the true meaning of almighty be carved in your soul..." << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
            cout << "I..." << endl;
            this_thread::sleep_for(chrono::milliseconds(1500));
            cout << "\x1B[1mAM...\x1B[0m" << endl;
            this_thread::sleep_for(chrono::seconds(2));
            cout << "\x1B[3matomic\x1B[0m" << endl;
            // this_thread::sleep_for(chrono::seconds(2));
            // cout << "A great explosion takes place" << endl;
        }
        Humano(const Humano& other)= delete;
        
        void ataque(Soldado* other) override {
            if(get_saude()<=0) return;
            other->defesa(this->get_poder_ataque()); 
            if(!(rand()%10)) other->defesa(this->get_poder_ataque());
        }

        // Special treatment to humans called Samuel Rodrigues, Vergil, Talion and Cid Kagenou
        // Special attacks: JCE (I NEED MORE POWER, THIS IS POWER), I AM ATOMIC
};

class Gondoriano: public Humano {
    size_t is_berserk;

    Gondoriano(string nome, int hp, int pa) : Humano(nome, hp, pa), is_berserk() {}

    void defesa(int pa) override {
        set_saude(get_saude()-pa/2);
    } 

    void berserk(){
        cout << get_nome() << "'s blood is boiling..." << endl;
        set_poder_ataque(2*get_poder_ataque());
    }

    void letargy(){

    }

    void normal(){

    }

    void ataque(Soldado* other) override{
        if(!is_berserk && rand()%100 == 0){
            is_berserk = 3;
            berserk();
        }

        Humano::ataque(other);

        
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
        cout << "The air is getting warmer around " << get_nome() << endl;
    }

    void ataque(Soldado* other) override{

    }

    void long_range_attack(){

    }

};

class Sauron: public Soldado{

    static size_t counter;

    public:
        Sauron(int hp, int pa) : Soldado("Sauron", 10 * hp, pa){
            if(counter) throw runtime_error("There can only be one Dark Lord");
            counter++;
        }
        Sauron(const Sauron &other) = delete;
        
        ~Sauron(){counter--;}
        
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

        

        void has_revived(){
            if(revived) cout << "The mage has revived" << endl;
            else cout << "The mage has not revived" << endl;
        }
};

class Vector: public vector<Soldado>{};

class Menu{

    bool game_over;

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

};


int main(){ 
    srand(time(NULL));
    Menu game;
    Humano* cid = new Humano("Cid Kagenou", 10, 10);

    
    
    // proof of concept: printing and erasing characters
    // cout.sync_with_stdio(false);
    // for(int i = 0; i < 10; i++){
    //     cout << i << flush;
    //     this_thread::sleep_for(chrono::milliseconds(50));
    //     cout << "\b" << flush;
    // }
    // cout.sync_with_stdio(true);
    
    
    
    
    // cid->I_AM_ATOMIC();
    // cout << "\x1B[3m\x1B[1mPlaytime is over\x1B[0m" << endl;
    // do{
    // game.instantiate();
    // game.run();
    // } while(!game.over());
    
}