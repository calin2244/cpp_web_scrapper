#ifndef json_utils_h
#define json_utils_h

#include "Data.h"
#include <nlohmann/json.hpp>
#include <set>
using std::map;
using std::set;
using std::pair;

using json = nlohmann::json;


namespace map_utils{
    
    struct comparator{
        template<typename T>
        bool operator()(const T& a, const T&b){
            if(a.second == b.second)
                return a.second;
            return a.second > b.second;
        }
    };

    void sortMap(map<string, int>& m, unsigned int num_of_items_to_show){
        set<pair<string, int>, comparator> set_(m.begin(), m.end());

        for(auto& x: set_){
            if(num_of_items_to_show){
                std::cout<<'\n'<<x.first<<" "<<x.second<<" \n";
                num_of_items_to_show--;
            }
            else break;
        }
    }
}

class myJson{
    json j;
    map<string, int> j_map;
    public:
        myJson(std::ifstream& f){
            try{
                j = json::parse(f);
                populateMap(j_map, j);
            }catch(std::exception& e){
                std::cout<<"Fisierul este gol, introdu date pentru a-l popula.\n";
            }
        }

        ~myJson(){
            delete this;
        }

        void populateMap(map<string, int>& m, json j){
            for(json::iterator it = j.begin(); it != j.end(); ++it){
                m.insert({it.key(), it.value()});
            }
        }
        
        void insertInMap(string& name){
            if(j_map.find(name) == j_map.end()){
                j_map.insert({name, 1});
            }
            else j_map[name]++;
        }

        void showMostSearchedStocks(unsigned int& num_of_stocks){
            if(j_map.size() < num_of_stocks){
                std::cout<<"Stack overflow.";
                return;
            }else{
                map_utils::sortMap(j_map, num_of_stocks);
            }
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

#endif