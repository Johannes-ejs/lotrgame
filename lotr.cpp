#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;


class Soldado{
    string nome;
    double poder_ataque;
    double saude;

    protected:
        virtual void set_nome(string nome) { nome = nome; }
        virtual void set_saude(double saude) { saude = saude; }
        virtual void set_poder_ataque(double pa) { poder_ataque = pa; }

    public:
        Soldado(string nome, double hp, double pa) : nome(nome), poder_ataque(pa), saude(hp) {}
        Soldado(const Soldado &other) = delete;

        virtual string get_nome() { return nome; }
        virtual double get_poder_ataque() { return poder_ataque; }
        virtual double get_saude() { return saude; }
        virtual void defesa(double pa) { saude -= pa; }
        virtual void true_damage(double pa) final { saude -= pa; }
        virtual void ataque(Soldado *other) { other->defesa(this->get_poder_ataque()); }
        virtual bool is_alive() { return saude > 0; }
        virtual void resurrectio() {}
};


class Elfo: public Soldado{
    public:
 
        Elfo(string nome, double hp, double pa): Soldado(nome, hp, pa+1) {}        
        Elfo(const Elfo& other)= delete;
        void ataque(Soldado* other) override { Soldado::ataque(other); }        

};


class Anao: public Soldado{
    public:
        
        Anao(string nome, double hp, double pa) : Soldado(nome, hp, pa + 20) {}
        
        Anao(const Anao& other)= delete;
        
        void ataque(Soldado* other) override {
            if (!(rand() % 5 <= 1))
                Soldado::ataque(other);
        }

};


class Humano: public Soldado{
    
    
    public:
        Humano(string nome, double hp, double pa) : Soldado(nome, hp, pa) {
        }
        
        Humano(const Humano& other)= delete;
        
        void ataque(Soldado* other) override {
            Soldado::ataque(other);
            if(!(rand()%10))
                Soldado::ataque(other);
        }

};


class Eminence: public Humano {
    size_t WAR_MODE; // turns Shadow immune to damage and with double damage and attack speed for 3 rounds

    public:

    Eminence(double hp, double pa) : Humano("Shadow", hp, pa), WAR_MODE(false) {}

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

    void defesa(double pa) override {
        if(WAR_MODE) return;
        Soldado::defesa(pa/2);
    } 

    void overdrive(){
        cout << "Now... " << flush; 
        this_thread::sleep_for(chrono::milliseconds(700));
        cout << "\x1B[1mI am a little motivated\x1B[0m" << endl;
        WAR_MODE = 3;
    }

    void normal(){
        // too lazy to implement
    }

    void ataque(Soldado* other) override{


        if(!WAR_MODE && rand() % 100==0)
            overdrive();

        if(WAR_MODE){
            WAR_MODE--;
            other->true_damage(get_poder_ataque());
            return;
        }

        if(rand()%1000)
            Humano::ataque(other);
        else{
            I_AM_ATOMIC(other);
        }
    
    }
};


class Balrog: public Soldado{
    
    bool IS_BERSERK;
    double original_hp;

    public:
    
        Balrog(string nome, double hp, double pa): Soldado(nome, hp, pa), IS_BERSERK(false), original_hp(hp) {}


        void defesa(double pa) override{
            if(!IS_BERSERK) Soldado::defesa(pa);
            else Soldado::defesa(pa/5);
            if(!IS_BERSERK && get_saude() < original_hp/5) berserk();
        }

        void berserk(){
            cout << "\x1B[3mThe air is getting warmer around " << get_nome() << "..." <<"\x1B[0m" << endl;
            this_thread::sleep_for(chrono::milliseconds(700));
            IS_BERSERK = true;
            set_poder_ataque(1.25*get_poder_ataque());
        }

        void ataque(Soldado* other) override{
            Soldado::ataque(other);
            Soldado::ataque(other);
            if (rand()%20 == 0 || IS_BERSERK)
                Soldado::ataque(other);
        }

};


class Sauron: public Soldado{

    public:

        Sauron(double hp, double pa) : Soldado("Sauron", 10 * hp, pa){}
        Sauron(const Sauron &other) = delete;
        
        ~Sauron(){}
        
        void ataque(Soldado *other) {
            if(rand() % 10 <=2){
                set_poder_ataque(2*get_poder_ataque());
                Soldado::ataque(other);
                Soldado::ataque(other);
                set_poder_ataque(get_poder_ataque()/2);
            } else Soldado::ataque(other);
        }

};


class Orc: public Soldado{
    public:
        Orc(string nome, double hp, double pa): Soldado(nome, hp, pa) {}
        Orc(const Orc& other)= delete;
        void ataque(Soldado *other) {
            if(rand() % 10 <= 1){
                set_poder_ataque(1.1*get_poder_ataque());
                Soldado::ataque(other);
                Soldado::ataque(other);
                set_poder_ataque(get_poder_ataque()/1.1);
            } else Soldado::ataque(other);
        }
};


class Mago: public Soldado{
    
    bool revived;
    double original_hp;

    public:
        Mago(string nome, double hp, double pa) : Soldado(nome, hp, pa), revived(false), original_hp(hp) {}
        Mago(const Mago& other)= delete;

        void ataque(Soldado *other) {
            if(!revived && get_saude() <=0 && rand()%20){
                set_saude(original_hp/2);
                revived = true;
            }
            if(get_saude() <=0) return;
            
            int random_var = rand();
            if (random_var % 25 == 0){
                Soldado::defesa(this->get_poder_ataque());
            }
            else if (random_var % 10 == 0){
                set_poder_ataque(1.5*get_poder_ataque());
                Soldado::ataque(other);
                set_poder_ataque(get_poder_ataque()/1.5);
            } else
                Soldado::ataque(other);
        }

        void defesa(double pa) override {
            if(revived)
                set_saude(get_saude()-10);
                Soldado::defesa(pa);
        }

        void resurrectio() override{
            // Ressucita o mago. Mago levará 10 de dano a cada rodada e terá 1.5 vezes o P.A.
            set_poder_ataque(1.5*get_poder_ataque());
        } // Declare in Soldado

};


class ReiBruxo: public Mago{
    public:
        ReiBruxo(string name, double hp, double pa): Mago(name,hp,pa) {}

        void resurrectio() override{
            // Ressucita o mago. Mago levará 10 de dano a cada rodada e terá 1.5 vezes o P.A.
            set_poder_ataque(2*get_poder_ataque());
        } // Declare in Soldado
        
        void defesa(double pa) override{
            Soldado::defesa(pa);
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



int main(){ 
    srand(time(NULL));

    Soldado* a1 = new Elfo("ksdjhbgjwbdg", 100, 9);
    Soldado* a2 = new Anao("ksdjhbgjwbdg", 100, 9);
    Soldado* a3 = new Humano("ksdjhbgjwbdg", 100, 9);
    Soldado* a4 = new Eminence(100, 9);
    Soldado* a5 = new Balrog("ksdjhbgjwbdg", 100, 9);
    Soldado* a6 = new Mago("ksdjhbgjwbdg", 100, 9);
    Soldado* a7 = new ReiBruxo("ksdjhbgjwbdg", 100, 9);
    Soldado* a8 = new Sauron(100, 9);
    Soldado* a9 = new Orc("ksdjhbgjwbdg", 100, 9);

    for(int i=0; i<10; i++){
        a8->ataque(a6);
        a6->ataque(a8);
    }


    // cout << a1->get_saude() << endl;
    // cout << a2->get_saude() << endl;
    // cout << a3->get_saude() << endl;
    // cout << a4->get_saude() << endl;
    // cout << a5->get_saude() << endl;
    cout << a6->get_saude() << endl;
    // cout << a7->get_saude() << endl;
    cout << a8->get_saude() << endl;
    // cout << a9->get_saude() << endl;



    // cout << a1->get_saude() << endl;
    // cout << a2->get_saude() << endl;
    // cout << a3->get_saude() << endl;
    // cout << a4->get_saude() << endl;
    // cout << a5->get_saude() << endl;
    // cout << a6->get_saude() << endl;
    // cout << a7->get_saude() << endl;
    // cout << a8->get_saude() << endl;
    // cout << a9->get_saude() << endl;




    delete a1;
    delete a2;
    delete a3;
    delete a4;
    delete a5;
    delete a6;
    delete a7;
    delete a8;
    delete a9;

    // Menu game;
    // do{
    //     game.run();
    // } while(!game.over())    
}