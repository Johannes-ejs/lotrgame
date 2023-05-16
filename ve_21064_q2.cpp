#include <iostream>

using namespace std;

class Character{
    string nome;
    string ocupacao;

    public:
        Character(): nome(""), ocupacao("") {}
        Character(string nome, string ocupacao): nome(nome), ocupacao(ocupacao) {}
        Character(const Character& c): nome(c.nome), ocupacao(c.ocupacao) {}

        void set_nome(string name) { nome = name; }
        void set_ocupacao(string oc) { ocupacao = oc; }
        string get_nome() { return nome; }
        string get_ocupacao() { return ocupacao; }

};

class Relation{
    Character character1;
    Character character2;
    string relationship;
    
    public:
        Relation(): character1(), character2(), relationship("") {}
        Relation(Character character1, Character character2, string rel): character1(character1), character2(character2), relationship(rel) {}
        Relation(const Relation& other): character1(other.character1), character2(other.character2), relationship(other.relationship) {}

        friend class CharacterDatabase;

        string display(){
            return character1.get_nome() + " and " + character2.get_nome() + ": " + relationship;
        }

};

class CharacterDatabase{
    Character* database;
    Relation* relationdatabase;

    size_t data_size;
    size_t rel_size;

    public:
        CharacterDatabase(): database(new Character[100]), data_size(), rel_size(), relationdatabase(new Relation[1000]) {}
        CharacterDatabase(const CharacterDatabase& other) = delete; 

        ~CharacterDatabase() {
            delete[] database;
            delete[] relationdatabase;
        }
        
        size_t searchCharacter(string name){
            for(size_t i = 0; i < data_size; i++) {
                if(database[i].get_nome() == name) return i;
            }
            return 101;
        }

        bool addCharacter(Character& other){
            if(searchCharacter(other.get_nome()) == 101){
                database[data_size++] = other;
                return true;
            } return false;
        }

        bool deleteCharacter(string name) {
            size_t aux = searchCharacter(name); 
            if (aux == 101) return false;
            Character temp(database[aux].get_nome(), database[aux].get_ocupacao());
            database[aux] = database[data_size-1];
            database[data_size-1] = temp;
            data_size--;
            return true;
        }

        void imprimir() {
            cout << "-------------------------------- PERSONAGENS --------------------------------" << endl << endl;
            for(size_t i = 0; i<data_size; i++){
                cout << database[i].get_nome() << " - " << database[i].get_ocupacao() << endl;
            }
            cout << endl;
            cout << "-------------------------------- RELACOES --------------------------------" << endl << endl;
            for(size_t i = 0; i<rel_size; i++){
                cout << relationdatabase[i].display() << endl;
            }


        }

        bool relate(Character& c1, Character& c2, string relation){
            size_t found_first = searchCharacter(c1.get_nome());
            size_t found_second = searchCharacter(c2.get_nome());
            if(found_first == 101 || found_second == 101) return false;
            relationdatabase[rel_size++] = Relation(c1, c2, relation);
            return true;
        }

        bool editCharacter(string name, string oc){
            size_t index = searchCharacter(name);
            if(index == 101) return false;
            database[index].set_ocupacao(oc);
            return true;
        }

        
};

int main(){

CharacterDatabase database;
Character c1 = Character("Rachel Green", "Garçonete");
Character c2 = Character("Monica Geller", "Chef");
Character c3 = Character("Phoebe Buffay", "Massagista");
Character c4 = Character("Chandler Bing", "Desconhecido");
Character c5 = Character("Ross Geller", "Professor");
Character c6 = Character("Joey Tribbiani", "Ator");
database.addCharacter(c1);
database.addCharacter(c2);
database.addCharacter(c3);
database.addCharacter(c4);
database.addCharacter(c5);
database.addCharacter(c6);
database.editCharacter("Rachel Green", "Design de Moda");
cout << database.searchCharacter("Gunther") << endl;
cout << database.searchCharacter("Rachel Green") << endl;
database.relate(c2, c5, "irmãos");
database.relate(c2, c4, "casados");
database.relate(c1, c2, "roommates");
database.relate(c4, c6, "roommates");
database.deleteCharacter("Phoebe Buffay");
database.imprimir();
}