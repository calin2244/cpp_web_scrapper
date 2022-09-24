#ifndef json_utils_h
#define json_utils_h

#include "utils.h"
#include <nlohmann/json.hpp>
#include <fstream>
using std::map;
using std::string;

using json = nlohmann::json;

class myJson{
    json j;
    map<string, int> j_map;
    public:
        myJson(std::ifstream& f){
            try{
                j = json::parse(f);
                populateMap(j);
            }catch(std::exception &e){
                std::cout<<"Fisierul este gol\n. Introdu date pentru a-l popula.";
            }
        }

        ~myJson(){
            delete this;
        }

        void populateMap(json j){
            for(json::iterator it = j.begin(); it != j.end(); ++it){
                j_map.insert({it.key(), it.value()});
            }
        }
        
        void insertInMap(string& name){
            if(j_map.find(name) == j_map.end()){
                j_map.insert({name, 1});
            }
            else j_map[name]++;
        }

        void showJsonContent(){
            if(!j.empty())
                std::cout<<j;
        }

        void populateJsonFile(std::ofstream& g){
            for(auto &el: j_map){
                j[el.first] = el.second;
            }
            g<<j;
        }

};

// std::ifstream f("pref.json");
//     json preffered_stocks = json::parse(f);
//     map<string, int> json_map;
//     for(json::iterator it = preffered_stocks.begin(); it != preffered_stocks.end(); ++it){
//         json_map.insert({it.key(), it.value()});
//     }

#endif