#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <json/json.h>
#include <fstream>
#include <set>
#include <nlohmann/json.hpp>
using std::map;

using json = nlohmann::json;

//CurlObj is used to get the html from the given webpage
//credit to stack overflow for help on the CurlObj class
#define jsmap std::map<std::string, int>

struct comparator{
    template<typename T>
    bool operator()(const T& l, const T& r) const{
        return l.second < r.second;
    }
};

void sortM(map<std::string, int>& m){
    std::set<std::pair<std::string ,int>, comparator> set_(m.begin(), m.end());

    for(auto &x: set_){
        std::cout<<x.first<<" "<<x.second<<'\n';
    }
}

int main() {
    
    jsmap m = {
        {"a", 2}, {"b", 4}, {"c", 1}
    };
    sortM(m);

    return 0;
}
