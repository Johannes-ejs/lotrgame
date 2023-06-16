#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random> // used for normal_distribution. NOTE: WORKS ONLY ON C++11 AND ABOVE
#include <queue>
#include <iomanip>


using namespace std;



class Soldado{
    string nome;
    double poder_ataque;
    double saude;
    bool IS_MAGE;
    
    default_random_engine engine;

    protected:
        virtual void set_nome(string nome) { nome = nome; }
        virtual void set_saude(double hp) { saude = hp; }
        virtual void set_poder_ataque(double pa) { poder_ataque = pa; }
        virtual void set_is_mage(bool flag) { IS_MAGE = flag; }

    public:
        Soldado(string nome, double hp, double pa) : nome(nome), poder_ataque(pa), saude(hp), IS_MAGE(false){
            engine.seed(chrono::system_clock::now().time_since_epoch().count());
        }
        
        Soldado(const Soldado &other) = delete;

        virtual string get_nome() { return nome; }
        
        virtual double get_poder_ataque() { return poder_ataque; }
        
        virtual double get_saude(){
            return saude >= 0.01? saude : 0;
        }
        
        virtual void contra_ataque(Soldado* other){
            cout << get_nome() << " throws a counter-attack!" << endl;
            ataque(other);            
        }

        virtual bool will_dodge(){
            return !(rand()%20);
        }

        virtual bool will_block(){
            return !(rand()%10);
        }

        virtual double block(){
            return 0.2;
        }

        virtual void defesa(Soldado* other, double pa){
            normal_distribution<double> dist(pa, pa/5);
            double true_dmg = round(dist(engine)*100.0)/100.0;
            if(true_dmg <= pa-1.5*pa/5){
                contra_ataque(other);
                return;
            }
            if(true_dmg >= 1.2*pa) cout << "It's super effective!" << endl;
            if(will_block()){
                cout<<get_nome()<<" blocks the attack!"<<endl;
                saude-=true_dmg*block();
            }
            else if(will_dodge()){
                cout<<get_nome()<<" dodge the attack!"<<endl;
            }

            else saude -= true_dmg;
            return;
        }


        
        virtual void true_damage(Soldado* other, double pa) final {
            if(other->get_saude() <=0) return;
            cout << other->get_nome() << " throws an unavoidable attack!" << endl;
            saude -= pa;
        }
        
        virtual void ataque(Soldado *other) {
            if(other-> get_saude() <=0) return;
            cout << get_nome() << " throws an attack!" << endl;
            other->defesa(this, this->get_poder_ataque());
        }
        
        virtual bool is_alive() { return saude > 0; }
        
        virtual void resurrectio() {}
        
        virtual bool is_resurrected() {
            return false;
        } 

        virtual bool is_evil() {return false; throw runtime_error("this method was not supposed to be called");}

        virtual bool is_mage() {return IS_MAGE;}

};


class Elfo: public Soldado{
    public:
 
        Elfo(string nome, double hp, double pa): Soldado(nome, hp, pa+1) {}        
        Elfo(const Elfo& other)= delete;
        void ataque(Soldado* other) override { Soldado::ataque(other); }     
        bool will_block(){
            return !(rand()%3);
        }
        bool will_dodge(){
            return !(rand()%3);
        }
        double block(){
            return 0.4;
        }
};


class Anao: public Soldado{
    public:
        
        Anao(string nome, double hp, double pa) : Soldado(nome, hp, pa + 20) {}
        
        Anao(const Anao& other)= delete;
        
        void ataque(Soldado* other) override {
            if (!(rand() % 3 <= 1))
                Soldado::ataque(other);
            else
                cout << get_nome() << " misses the attack!" << endl;
        }

        void defesa(Soldado* other, double pa) override {
            cout << get_nome() << "'s sturdy skin allows him to resist some damage!" << endl;
            Soldado::defesa(other, pa/3);
        }
        bool will_dodge(){
            return !(rand()%8);
        }
        double block(){
            return 0.3;
        }
};


class Humano: public Soldado{
    
    
    public:
        Humano(string nome, double hp, double pa) : Soldado(nome, hp, pa) {
        }
        
        Humano(const Humano& other)= delete;
        
        void ataque(Soldado* other) override {
            Soldado::ataque(other);
            if(!(rand()%10)){
                Soldado::ataque(other);
                Soldado::ataque(other);
                if(rand()%2)
                    Soldado::ataque(other);
            }
        }
        bool will_block(){
            return !(rand()%4);
        }
        bool will_dodge(){
            return !(rand()%8);
        }
};


class Eminence: public Humano {
    size_t WAR_MODE; 

    public:

    Eminence(double hp, double pa) : Humano("Shadow", hp, pa), WAR_MODE(false) {}

    void I_AM_ATOMIC(Soldado* other){
        cout << "\x1B[3m\x1B[1m\x1B[38;5;21mPlaytime is over\x1B[0m" << endl;
        cout << "The area is filled with a bluish magic" << endl;
        cout << "Allies and foes alike all stopped battling, and silently, fearfully stared at the source of the great magic" << endl;
        cout << "Let the true meaning of almighty be carved in your soul..." << endl;
        cout << "I..." << endl;
        cout << "\x1B[1mAM...\x1B[0m" << endl;
        cout << "\x1B[3matomic\x1B[0m " << endl;
        other->true_damage(this, 999999);
        cout << "A great explosion takes place, with " << other->get_nome()<< " at its center" << endl;
        cout << "The dust wears off, revealing " << other->get_nome()<< "'s corpse lying in the ground" << endl;
    }

    void defesa(Soldado* other, double pa) override {
        cout << "Shadow's increased metabolism improved his cells's regeneration, recovering his health " << endl;
        this_thread::sleep_for(chrono::seconds(1));
        set_saude(get_saude()+pa/10.);
        if(WAR_MODE){
            set_saude(get_saude() + 9*pa/10.0);
            return;
        } 
        Soldado::defesa(other, pa/3);
    } 

    void overdrive(){
        cout << "Now... " << flush; 
        cout << "\x1B[1mI am a little motivated\x1B[0m" << endl;
        WAR_MODE = 3;
    }


    void ataque(Soldado* other) override{


        if(!WAR_MODE && rand() % 15==0)
            overdrive();

        if(WAR_MODE){
            WAR_MODE--;
            other->true_damage(this, 2*get_poder_ataque());
            return;
        }

        if(rand()%20)
            Humano::ataque(other);
        else{
            I_AM_ATOMIC(other);
        }
    
    }
    bool will_block(){
        return false;
    }
    bool will_dodge(){
        return false;
    }

};


class Balrog: public Soldado{
    
    bool IS_BERSERK;
    double original_hp;

    public:
    
        Balrog(string nome, double hp, double pa): Soldado(nome, hp, pa), IS_BERSERK(false), original_hp(hp) {}


        void defesa(Soldado* other, double pa) override{
            if(!IS_BERSERK) Soldado::defesa(other, pa);
            else Soldado::defesa(other, pa/5);
            if(!IS_BERSERK && get_saude() < original_hp/5 && get_saude()>0) berserk();
        }


        void berserk(){
            cout << "\x1B[3mThe air is getting warmer around " << get_nome() << "..." <<"\x1B[0m" << endl;
            IS_BERSERK = true;
            set_poder_ataque(1.25*get_poder_ataque());
        }

        void ataque(Soldado* other) override{
            Soldado::ataque(other);
            Soldado::ataque(other);
            if (rand()%20 == 0 || IS_BERSERK)
                Soldado::ataque(other);
        }
        bool will_dodge(){
            return false;
        }

};


class Sauron: public Soldado{

    public:

        Sauron(double hp, double pa) : Soldado("Sauron", 10 * hp, pa){}
    
        Sauron(const Sauron &other) = delete;
            
        void ataque(Soldado *other) {
            if(rand() % 10 <=2){
                set_poder_ataque(2*get_poder_ataque());
                Soldado::ataque(other);
                Soldado::ataque(other);
                set_poder_ataque(get_poder_ataque()/2);
            } else Soldado::ataque(other);
        }

        bool will_block(){
            return !(rand()%20);
        }
        bool will_dodge(){
            return false;
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
        bool will_block(){
            return !(rand()%5);
        }
        bool will_dodge(){
            return !(rand()%20);
        }
        double block(){
            return 0.05;
        }
};


class Mago: public Soldado{
    protected:
        bool revived;
        double original_hp;

    public:
        Mago(string nome, double hp, double pa) : Soldado(nome, hp, pa), revived(false), original_hp(hp) {
            set_is_mage(true);
        }

        Mago(const Mago& other)= delete;

        void ataque(Soldado *other) {
            if(!revived && get_saude() <=0 && rand()%20){
                set_saude(original_hp/2);
                revived = true;
            }
            if(get_saude() <=0) return;
            
            int random_var = rand();
            if (random_var % 25 == 0){
                cout << get_nome() << "'s spell goes haywire, hurting the caster!" << endl;
                true_damage(this, this->get_poder_ataque());
            }

            else if (random_var % 10 == 0){
                set_poder_ataque(1.5*get_poder_ataque());
                Soldado::ataque(other);
                set_poder_ataque(get_poder_ataque()/1.5);
            } else
                Soldado::ataque(other);
        }

        void defesa(Soldado* other, double pa) override {
            if(revived)
                set_saude(get_saude()-10);
                Soldado::defesa(other, pa);
        }

        void resurrectio() override{
                cout << "\x1B[38;5;21mBlack lightning sparkles around " << get_nome() << "...\x1B[0m" << endl;
                cout << get_nome() << " raises from the ground, as if they've only been asleep." << endl;
                cout << "Their skin, now pale white, confirms our suspicions..." << endl;
                cout << get_nome() << " has revived!" << endl;
                set_poder_ataque(1.5 * get_poder_ataque());
                set_saude(original_hp);
                revived = true;
        } 

        bool is_resurrected() override{
            return revived;
        }

        bool is_evil() override{
            return false;
        }

        bool will_block(){
            return !(rand()%5);
        }
        bool will_dodge(){
            return !(rand()%10);
        }
        double block(){
            return 0.4;
        }

};


class ReiBruxo: public Mago{
    public:
        ReiBruxo(string name, double hp, double pa): Mago(name,hp,pa) {}

        void resurrectio() override{
            Mago::resurrectio();
            set_poder_ataque(get_poder_ataque()/1.5*2);
        }
        
        void defesa(Soldado* other, double pa) override{
            Soldado::defesa(other, pa);
        }

        bool is_evil() override{
            return true;
        }

        bool will_block(){
            return !(rand()%5);
        }
        bool will_dodge(){
            return !(rand()%10);
        }
        double block(){
            return 0.3;
        }

};

class Menu{

        enum class winner{
            none,sauron,elfo
        };
        winner GAME_OVER;
        queue<Soldado *> sauron_army;
        queue<Soldado *> elf_army;
        queue<Soldado *> can_resurrect;
        bool leave;
        int sauron_size;
        int elf_size;
        int hp;
        int pa;
        int rodada_index;

    public:
        Menu(int hp, int pa) :GAME_OVER(winner::none),leave(false), hp(hp), pa(pa),rodada_index(1){
            srand(time(NULL));
            instantiate();
        }


        Menu(const Menu &other) = delete;

        int prelude_sauron(){
            int i;
            cout << "\x1B[32mWelcome to Lord of The Rings: ... \x1B[0m" << endl;
            cout << "How many characters would you like to have in sauron army?" << endl;
            cin >> i;
            while (!i > 0){
                cout << "The number must be positive! Let's try again" << flush;
                for (int i = 0; i < 3; i++){
                    cout << "." << flush;
                }
            }
            cout << "Sauron is in the army, so you need to choose the race of his soldiers" << flush;
            Soldado *sauron = new Sauron(hp, pa);
            sauron_army.push(sauron);
            for (int i = 0; i < 3; i++){
                cout << "." << flush;
            }
            int num_Rei_Bruxo{}, num_Balrog{}, num_Orc{};
            for (int j = 0; j < i - 1; j++){
                int key = 0;
                Soldado *sauron_soldier;
                while (key == 0){
                    key = 1;
                    cout << endl
                         << "Choose the race of the " << j + 2 << "th member of Sauron Army: " << endl;
                    cout << "1 - Orc\n2 - Balrog\n3 - Rei Bruxo" << endl;
                    int option;
                    cin >> option;
                    string nome;
                    switch (option){
                    case 1:
                        num_Orc++;
                        nome = "Orc" + to_string(num_Orc);
                        sauron_soldier = new Orc(nome, hp, pa);
                        break;
                    case 2:
                        if (num_Balrog == 2){
                            cout << endl
                                 << "We already have too many Balrog in the game, choose another race" << flush;
                            key = 0;
                            cout << '\n';
                            break;
                        }
                        num_Balrog++;
                        nome = "Balrog" + to_string(num_Balrog);
                        sauron_soldier = new Balrog(nome,hp,pa);
                        break;
                    case 3:
                        if (num_Rei_Bruxo == 2){
                            cout << "We already have too many Rei Bruxo in the game, choose another race" << flush;
                            for (int i = 0; i < 3; i++){
                                cout << "." << flush;
                            }
                            cout << '\n';
                            key = 0;
                            break;
                        }
                        num_Rei_Bruxo++;
                        nome = "Rei Bruxo" + to_string(num_Rei_Bruxo);
                        sauron_soldier = new ReiBruxo(nome, hp, pa);
                        break;
                    default:
                        key = 0;
                        cout << endl
                             << "invalid option" << endl;
                        break;
                    }
                }
                sauron_army.push(sauron_soldier);
            }
            return i;
        }

        int prelude_elf()
        {
            int i;
            cout << endl
                 << "Now, Decide How many characters would you like to have in elf army?" << endl;
            cin >> i;
            while (!i > 0)
            {
                cout << "The number must be positive! Let's try again" << flush;
                for (int i = 0; i < 3; i++)
                {
                    cout << "." << flush;
                }
            }
            int num_elf{}, num_anao{}, num_hum{}, num_emin{}, num_mago{};
            for (int j = 0; j < i; j++){
                int key = 0;
                Soldado *elf_soldier;
                while (key == 0){
                    key = 1;
                    cout << endl
                         << "Choose the race of the " << j + 1 << "th member of Elf Army: " << endl;
                    cout << "1 - Elfo\n2 - Anao\n3 - Humano\n4 - Eminence\n5 - Mago" << endl;
                    int option;
                    cin >> option;
                    string nome;
                    switch (option){
                    case 1:
                        num_elf++;
                        nome = "Elfo" + to_string(num_elf);
                        elf_soldier = new Elfo(nome, hp, pa);
                        break;
                    case 2:
                        num_anao++;
                        nome = "Anao" + to_string(num_anao);
                        elf_soldier = new Anao(nome, hp, pa);
                        break;
                    case 4:
                        if (num_emin == 1){
                            cout << "We already have too many Eminence in Shadows in the game, choose another race" << flush;
                            for (int i = 0; i < 3; i++)
                            {
                                cout << "." << flush;
                            }
                            cout << '\n';
                            key = 0;
                            break;
                        }
                        num_emin++;
                        nome = "Eminence";
                        elf_soldier = new Eminence(hp, pa);
                        break;
                    case 3:
                        num_hum++;
                        nome = "Humano" + to_string(num_hum);
                        elf_soldier = new Humano(nome, hp, pa);
                        break;
                    case 5:
                        num_mago++;
                        nome = "Mago" + to_string(num_mago);
                        elf_soldier = new Mago(nome, hp, pa);
                        break;
                    default:
                        key = 0;
                        cout << endl
                             << "invalid option" << endl;
                        break;
                    }
                }
                elf_army.push(elf_soldier);
            }
            return i;
        }

        void instantiate(){
            rodada_index=1;
            GAME_OVER = winner::none;
            sauron_size = prelude_sauron();
            elf_size = prelude_elf();
            // Only one Witch King, Sauron and Eminence allowed
        }

        void rodada(){
            cout<<endl<<endl<<"--------------------------------------"<<endl;
            if(rodada_index<10) cout<<"-------------- RODADA "<<rodada_index++<<" --------------"<<endl;
            else cout<<"-------------- RODADA "<<rodada_index++<<" -------------"<<endl;
            cout<<"--------------------------------------"<<endl<<endl;
            for (int i = 0; i < can_resurrect.size(); i++){
                Soldado *soldado_resurrect = can_resurrect.front();
                can_resurrect.pop();
                if(rand()%10==0){
                    soldado_resurrect->resurrectio();
                    if(soldado_resurrect->is_evil())
                        sauron_army.push(soldado_resurrect);
                    else
                        elf_army.push(soldado_resurrect);
                }
                else{
                    can_resurrect.push(soldado_resurrect);
                }
            }
            vector<Soldado *> elf_battle, sauron_battle;
            int minimo=min(elf_army.size(), sauron_army.size());
            for (int i = 0; i < minimo; i++){
                elf_battle.push_back(elf_army.front());
                elf_army.pop();
                sauron_battle.push_back(sauron_army.front());
                sauron_army.pop();
            }
            random_shuffle(elf_battle.begin(),elf_battle.end());
            random_shuffle(sauron_battle.begin(),sauron_battle.end());
            cout<<"The opponents of this round are:"<<endl;
            for(int i=0;i<minimo;i++){
                cout<<elf_battle[i]->get_nome()<<" vs "<<sauron_battle[i]->get_nome()<<endl;
            }
            char c;
                do{
                    cout<<endl<<"press ENTER to continue"<<endl;
                    fflush(stdin);
                    scanf("%c",&c);
                }while(c!='\n');
            float elf_live0,sauron_live0;
            for (int i = 0; i < minimo; i++){
                cout<<endl<<endl<<"-------------------------------------";
                cout<<endl<<endl<<elf_battle[i]->get_nome()<<" vs "<<sauron_battle[i]->get_nome()<<" :"<<endl<<endl<<endl;
                elf_live0=elf_battle[i]->get_saude();
                sauron_live0=sauron_battle[i]->get_saude();
                if(rand()%2){
                    elf_battle[i]->ataque(sauron_battle[i]);
                    if(sauron_battle[i]->is_alive()) sauron_battle[i]->ataque(elf_battle[i]);
                }
                else{
                    sauron_battle[i]->ataque(elf_battle[i]);
                    if(elf_battle[i]->is_alive())elf_battle[i]->ataque(sauron_battle[i]);
                }
                log(elf_live0,elf_battle[i],sauron_live0,sauron_battle[i]);

                if (!sauron_battle[i]->is_alive() && sauron_battle[i]->is_mage() && !sauron_battle[i]->is_resurrected())
                    can_resurrect.push(sauron_battle[i]);    
                else if (sauron_battle[i]->is_alive())
                    sauron_army.push(sauron_battle[i]);
                else{
                    delete sauron_battle[i];
                }

                if (!elf_battle[i]->is_alive() && elf_battle[i]->is_mage() && !elf_battle[i]->is_resurrected())
                    can_resurrect.push(elf_battle[i]);
                else if ( elf_battle[i]->is_alive())
                    elf_army.push(elf_battle[i]);
                else{
                    delete elf_battle[i];
                }
                char c;
                if(i!=minimo-1){
                    do{
                    cout<<endl<<"press ENTER to continue"<<endl;
                        fflush(stdin);
                        scanf("%c",&c);
                    }while(c!='\n');
                }
            }
            if(sauron_army.empty()) GAME_OVER=winner::elfo;
            if(elf_army.empty()) GAME_OVER=winner::sauron;
            cout<<endl<<"\x1B[32mROUND'S OVER\x1B[0m"<<endl;
        }

        void log(float elf_live0,Soldado *elf_soldier,float sauron_live0,Soldado *sauron_soldier){
            if(!elf_soldier->is_alive()) cout<<elf_soldier->get_nome()<<" is dead!"<<endl;
            if(!sauron_soldier->is_alive()) cout<<sauron_soldier->get_nome()<<" is dead!"<<endl;
            string elf_live,sauron_live;
            elf_soldier->is_alive()?elf_live="Yes":elf_live="No";
            sauron_soldier->is_alive()?sauron_live="Yes":sauron_live="No";
            cout<<endl<<endl<<"                RESULT"<<endl;
            cout<<"_____________________________________"<<endl;
            cout << left <<setw(15) << "|ARMY"<<setw(10)<<"|Elf"<<setw(12)<<"|Sauron"<<"|"<<endl;
            cout << left <<setw(15) << "|NAME"<<setw(10)<<"|"+elf_soldier->get_nome()<<setw(12)<<"|"+sauron_soldier->get_nome()<<"|"<<endl;
            cout << left <<setw(15) << "|LIFE"<<"|"<<setw(9)<<round(elf_soldier->get_saude()*100)/100.0<<"|"<<setw(11)<<round(sauron_soldier->get_saude()*100)/100.0<<"|"<<endl;
            cout << left <<setw(15) << "|DAMAGE TAKEN"<<"|"<<setw(9)<<round((elf_live0-elf_soldier->get_saude())*100)/100.0<<"|"<<setw(11)<<round((sauron_live0-sauron_soldier->get_saude())*100)/100.0<<"|"<<endl;
            cout << left <<setw(15) << "|ALIVE"<<setw(10)<<"|"+elf_live<<setw(12)<<"|"+sauron_live<<"|"<<endl; 
            cout<<"|______________|_________|___________|"<<endl;
        }

        void run(){
            while(GAME_OVER==winner::none){
                rodada();
                char c;
                do{
                    cout<<endl<<"press ENTER to continue"<<endl;
                    fflush(stdin);
                    scanf("%c",&c);
                }while(c!='\n');
            }
            final_results();
        }

        void final_results(){
            cout <<"\x1B[32mGAME OVER!\x1B[0m"<<endl<<endl<<"The winner is ";
            if(GAME_OVER==winner::sauron){
                cout<<"Sauron army !"<<endl<<endl;;
                cout << "The survivors are: ..." << endl;
                while(!sauron_army.empty()){
                    Soldado* soldier=(sauron_army.front()); sauron_army.pop();
                    cout<<soldier->get_nome()<<endl;
                    delete soldier;
                }
                cout<<endl;
            }
            else{
                cout<<"Elf army !"<<endl<<endl;
                cout << "The survivors are: ..." << endl;
                while(!elf_army.empty()){
                    Soldado* soldier=(elf_army.front()); elf_army.pop();
                    cout<<soldier->get_nome()<<endl;
                    delete soldier;
                }
                cout<<endl;
            }
            end_game();
        }
        void delete_game(){
            while (!sauron_army.empty()){
                Soldado *soldier=sauron_army.front();
                sauron_army.pop();
                delete soldier;
            }
            while (!elf_army.empty()){
                Soldado *soldier=sauron_army.front();
                elf_army.pop();
                delete soldier;
            }
            while(!can_resurrect.empty()){
                Soldado *soldier=can_resurrect.front();
                can_resurrect.pop();
                delete soldier;
            }
        }

        void end_game(){
            delete_game();
            char c;
            cout << "Would you like to play again? (Y/N) " << endl;
            cin >> c;
            if (tolower(c) != 'y'){
                cout << "Alright. Bye!" << endl;
                leave=true;
            }
            else{
                cout << "Alright! Restarting session" << flush;
                for (int i = 0; i < 3; i++){
                    cout << "." << flush;
                }
                cout << endl;
                leave=false;
                instantiate();
            }
        }

        bool over() { return leave; }
};



int main(){
    //here we decide the default amount of health and attack for our game
    //by passing these parameters, in that order, to the constructor of the menu class
    
    Menu game(500, 100);
    do{
        game.run();
    } while (!game.over());  
}