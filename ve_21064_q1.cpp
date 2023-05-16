#include <iostream>
#include <ctime>

using namespace std;


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
        Humano(string nome, int hp, int pa) : Soldado(nome, hp, pa) {}
        Humano(const Humano& other)= delete;
        void ataque(Soldado* other) override {
            if(get_saude()<=0) return;
            other->defesa(this->get_poder_ataque()); 
            if(!(rand()%10)) other->defesa(this->get_poder_ataque());
        }
};

class Gondoriano: public Humano {
    void defesa(int pa) override {
        set_saude(get_saude()-pa/2);
    } 
};


class Sauron: public Soldado{

    public:
        Sauron(int hp, int pa) : Soldado("Sauron", 10 * hp, pa){}
        Sauron(const Sauron &other) = delete;
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

        void has_revived(){
            if(revived) cout << "The mage has revived" << endl;
            else cout << "The mage has not revived" << endl;
        }
};



int main(){ 
    srand(time(NULL));
    Soldado* sauron = new Sauron(10, 10);
    
    Soldado* galadriel = new Elfo("Galadriel", 10, 10);
    Soldado* celebrimbor = new Elfo("Celebrimbor", 10, 10);
    Soldado* a = new Elfo("A", 10, 10);
    Soldado* b = new Elfo("B", 10, 10);
    Soldado* c = new Elfo("C", 10, 10);
    
    Soldado* talion = new Humano("Talion", 10, 10);
    Soldado* talion2 = new Humano("Talion2", 10, 10);
    Soldado* talion3 = new Humano("Talion3", 10, 10);
    Soldado* talion4 = new Humano("Talion4", 10, 10);
    Soldado* talion5 = new Humano("Talion5", 10, 10);

    Soldado* anao1 = new Anao("Anao1", 10, 10);
    Soldado* anao2 = new Anao("Anao2", 10, 10);
    Soldado* anao3 = new Anao("Anao3", 10, 10);
    Soldado* anao4 = new Anao("Anao4", 10, 10);
    Soldado* anao5 = new Anao("Anao5", 10, 10);

    Soldado* orc1 = new Orc("Orc1", 10, 10);
    Soldado* orc2 = new Orc("Orc2", 10, 10);
    Soldado* orc3 = new Orc("Orc3", 10, 10);
    Soldado* orc4 = new Orc("Orc4", 10, 10);
    Soldado* orc5 = new Orc("Orc5", 10, 10);
    Soldado* orc6 = new Orc("Orc6", 10, 10);
    Soldado* orc7 = new Orc("Orc7", 10, 10);
    Soldado* orc8 = new Orc("Orc8", 10, 10);
    Soldado* orc9 = new Orc("Orc9", 10, 10);
    Soldado* orc10 = new Orc("Orc10", 10, 10);

    Mago* gandalf = new Mago("Gandalf", 10, 10);

    gandalf->ataque(orc1);
    sauron->ataque(galadriel);
    gandalf->ataque(orc2);
    talion->ataque(orc3);
    talion2->ataque(orc4);
    anao2->ataque(orc5);
    anao1->ataque(orc6);
    gandalf->ataque(orc7);
    talion->ataque(orc3);
    gandalf->ataque(orc4);
    celebrimbor->ataque(orc1);
    celebrimbor->ataque(orc7);
    gandalf->ataque(sauron);
    talion4->ataque(orc10);
    talion5->ataque(sauron);
    talion->ataque(orc10);
    talion->ataque(orc10);
    talion->ataque(orc10);
    talion->ataque(orc10);

    cout << orc10->is_alive() << " " << gandalf->is_alive() << " " << sauron->is_alive() << " " << galadriel->is_alive() << " ";
    cout << orc5->is_alive() << " " << celebrimbor->is_alive() << endl;
    cout << orc10->get_saude() << " " << gandalf->get_saude() << " " << sauron->get_saude() << " " << galadriel->get_saude() << " ";
    cout << orc5->get_saude() << " " << celebrimbor->get_saude() << endl;

    gandalf->has_revived();


    

}