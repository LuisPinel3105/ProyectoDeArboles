#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Persona{
    int id;
    string name;
    string last_name;
    char gender;
    int age;
    int id_father;
    bool is_dead;
    bool was_king;
    bool is_king;
    Persona* left;
    Persona* right;
    
    Persona(int i, string n, string ln, char g, int a, int f, bool dead, bool wk, bool ik): 
        id(i), name(n), last_name(ln), gender(g), age(a), id_father(f),
        is_dead(dead), was_king(wk), is_king(ik), left(nullptr), right(nullptr) {}
};

class ArbolGenealogico{
private:
    Persona* root;
    vector<Persona*> nodos;
    
    vector<string> split(const string& str, char delimiter){
        vector<string> tokens;
        string token;
        for(size_t i = 0; i < str.length(); i++){
            if(str[i] == delimiter){
                tokens.push_back(token);
                token.clear();
            }else{
                token += str[i];
            }
        }
        tokens.push_back(token);
        return tokens;
    }
    
    int stringToInt(const string& str){
        int result = 0;
        for(size_t i = 0; i < str.length(); i++){
            if(str[i] >= '0' && str[i] <= '9'){
                result = result * 10 + (str[i] - '0');
            }
        }
        return result;
    }
    
    bool stringToBool(const string& str){
        return str == "1";
    }
    
    Persona* buscarPorId(int id){
        for(size_t i = 0; i < nodos.size(); i++){
            if(nodos[i]->id == id){
                return nodos[i];
            }
        }
        return nullptr;
    }
    
    void construirArbol(){
        for(size_t i = 0; i < nodos.size(); i++){
            Persona* persona = nodos[i];
            if(persona->id_father == 0){
                root = persona;
            }else{
                Persona* padre = buscarPorId(persona->id_father);
                if (padre){
                    if(padre->left == nullptr){
                        padre->left = persona;
                    }else{
                        padre->right = persona;
                    }
                }
            }
        }
    }
    
    void buscarSucesoresPrimogenitos(Persona* nodo, vector<Persona*>& sucesion){
        if(nodo == nullptr)
		return;
        
        if(!nodo->is_dead && nodo->age < 70){
            bool existe = false;
            for(size_t i = 0; i < sucesion.size(); i++){
                if(sucesion[i]->id == nodo->id){
                    existe = true;
                    break;
                }
            }
            if(!existe){
                sucesion.push_back(nodo);
            }
        }
        
        if(nodo->left){
            buscarSucesoresPrimogenitos(nodo->left, sucesion);
        }
        
        if(nodo->right){
            buscarSucesoresPrimogenitos(nodo->right, sucesion);
        }
    }
    
    Persona* obtenerReyActual(){
        for(size_t i = 0; i < nodos.size(); i++){
            if(nodos[i]->is_king){
                return nodos[i];
            }
        }
        return nullptr;
    }
    
    Persona* buscarPrimerVaronVivoPrimogenito(Persona* nodo) {
        if(nodo == nullptr)
		return nullptr;
        
        if(!nodo->is_dead && nodo->age < 70 && nodo->gender == 'H'){
            return nodo;
        }
        
        if(nodo->left){
            Persona* candidato = buscarPrimerVaronVivoPrimogenito(nodo->left);
            if (candidato) return candidato;
        }
        
        if(nodo->right){
            Persona* candidato = buscarPrimerVaronVivoPrimogenito(nodo->right);
            if(candidato)
			return candidato;
        }
        
        return nullptr;
    }
    
    Persona* buscarMejorMujer(Persona* nodo){
        if(nodo == nullptr)
		return nullptr;
        
        vector<Persona*> mujeres;
        colectarMujeresValidas(nodo, mujeres);
        
        if (mujeres.empty())
		return nullptr;
        
        sort(mujeres.begin(), mujeres.end(), [this](Persona* a, Persona* b) {
            if (a->age != b->age) return a->age < b->age;
            return calcularDistanciaRaiz(a) < calcularDistanciaRaiz(b);
        });
        
        return mujeres[0];
    }
    
    void colectarMujeresValidas(Persona* nodo, vector<Persona*>& mujeres) {
        if (nodo == nullptr) return;
        
        if (!nodo->is_dead && nodo->age >= 15 && nodo->age < 70 && nodo->gender == 'M') {
            mujeres.push_back(nodo);
        }
        
        colectarMujeresValidas(nodo->left, mujeres);
        colectarMujeresValidas(nodo->right, mujeres);
    }
    
    void colectarCandidatos(Persona* nodo, vector<Persona*>& candidatos){
        if(nodo == nullptr)
		return;
        
        if(!nodo->is_dead && nodo->age < 70) {
            candidatos.push_back(nodo);
        }
        
        colectarCandidatos(nodo->left, candidatos);
        colectarCandidatos(nodo->right, candidatos);
    }
    
    int calcularDistanciaRaiz(Persona* nodo){
        if (nodo == nullptr) return 0;
        
        int distancia = 0;
        Persona* actual = nodo;
        
        while(actual->id_father != 0){
            actual = buscarPorId(actual->id_father);
            if(actual == nullptr) break;
            distancia++;
        }
        
        return distancia;
    }
    
    Persona* buscarHermano(Persona* persona){
        if(persona->id_father == 0) return nullptr;
        
        Persona* padre = buscarPorId(persona->id_father);
        if(padre){
            if(padre->left && padre->left->id != persona->id){
                return padre->left;
            }
            if(padre->right && padre->right->id != persona->id){
                return padre->right;
            }
        }
        return nullptr;
    }
    
    Persona* buscarTio(Persona* persona){
        if(persona->id_father == 0) return nullptr;
        
        Persona* padre = buscarPorId(persona->id_father);
        if(padre && padre->id_father != 0){
            Persona* abuelo = buscarPorId(padre->id_father);
            if(abuelo){
                if(abuelo->left && abuelo->left->id != padre->id){
                    return abuelo->left;
                }
                if(abuelo->right && abuelo->right->id != padre->id){
                    return abuelo->right;
                }
            }
        }
        return nullptr;
    }
    
    Persona* buscarAncestroConDosHijos(Persona* nodo) {
        if (nodo == nullptr) return nullptr;
        
        Persona* actual = nodo;
        while (actual != nullptr && actual->id_father != 0) {
            Persona* padre = buscarPorId(actual->id_father);
            if (padre && padre->left && padre->right) {
                return padre;
            }
            actual = padre;
        }
        return nullptr;
    }
    
    void mostrarNodo(Persona* nodo, int nivel){
        if(nodo == nullptr) return;
        
        string indentacion = "";
        for(int i = 0; i < nivel; i++){
            indentacion += "  ";
        }
        
        string estado = "";
        if(nodo->is_king) estado = " [REY ACTUAL]";
        else if(nodo->was_king) estado = " [EX-REY]";
        if(nodo->is_dead) estado += " [MUERTO]";
        
        cout << indentacion << "- " << nodo->name << " " << nodo->last_name << " (" << nodo->age << " years old, " << (nodo->gender == 'H' ? "Hombre" : "Mujer") << ")" << estado << endl;
        
        mostrarNodo(nodo->left, nivel + 1);
        mostrarNodo(nodo->right, nivel + 1);
    }
}