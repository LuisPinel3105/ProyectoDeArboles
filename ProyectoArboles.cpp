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
}