#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <json/json.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

//CurlObj is used to get the html from the given webpage
//credit to stack overflow for help on the CurlObj class

int main() {
    
    json j;
    j["APPL"] = 1;
    std::ofstream o("pref.json");
    o<<j;
    std::cout<<j["APPL"];
    return 0;
}
