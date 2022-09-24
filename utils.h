#ifndef utils_h
#define utils_h

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <fstream>

using std::vector;
using std::string;

class Methods{
    public:
        static string refactorOutput(string &s){
        
        //Return true if char a is numerical
        auto valid = [](char a) -> bool{
            return a >= '0' && a <= '9' ? true : false;
        };

        int index = s.size()-1;
        while(!valid(s[index--])){
            s.pop_back();
        }
        
        return s;
    }

    static std::string getFmtDataFromJson(string& s){
        char copy[s.size()];
        strcpy(copy, s.c_str());
        char *p = strtok(copy, ",\"");
        while(p){
            p = strtok(NULL, ":\"");
            if(strcmp(p, "fmt") == 0){
                p = strtok(NULL, ":\"");
                return p;
            }
        }
        return string();
    }

    static std::string toUpperCase(std::string& s){
        std::transform(s.begin(), s.end(), s.begin(), [](char ch){return toupper(ch);});
        return s;
    }

    static void readInput(string* output){
        std::string input;
        while(std::cin.peek() != '\n'){
            std::getline(std::cin, input);
            if(input != "\n"){
                output->append(input);
            }
        };
    }

    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    static inline vector<vector<string>> read_csv(string filename){
        std::ifstream csv_input(filename);
        if(!csv_input.is_open()){
            std::cerr<<"Couldn't read file: "<<filename<<'\n';
            return vector<vector<string>>();
        }

        vector<vector<string>> csvRows;

        for(string line; std::getline(csv_input, line);){
            std::istringstream ss(std::move(line));
            std::vector<std::string> row;
            if(!csvRows.empty()) {
                row.reserve(csvRows.front().size() + 100);
            }

            for(string value; std::getline(ss, value,',');){
                row.push_back(std::move(value));
            }
            csvRows.push_back(std::move(row));
        }
        return csvRows;
    }

    inline string getUniqueStrings(vector<string>& vec){
        return string();
    }
};

class Data{
    string html;
    string name;
    string price;
    string change;
    bool valid_stock = true;
    
public:
    Data(string name_, string html_) {
        name = name_;
        Methods::toUpperCase(name);
        //all the html of the website
        html = html_;
        populate();
    }
    
    bool isValid() const{
        std::string html_exists = "Last Price</th>";
        int pos = html.find(html_exists);
        return pos >= 0 ? true : false;
    }
    
    ~Data(){
        delete this;
    }

    /**
     * @brief Picks through the html to find the needed data.
     */
    void populate(){
        // regularMarketChange":{"
        if(this->isValid()){
            string market_price_json = "regularMarketPrice\":{\"raw\":";
            string change_json = "regularMarketChange\":{\"raw\":";
            unsigned int marketPrice = html.find(market_price_json) + market_price_json.size();
            unsigned int change_ = html.find(change_json) + change_json.size();
            string curr_price = html.substr(marketPrice, 10);
            string curr_change = html.substr(change_, 26);
            

            /*  
                Checks if the stock page exists and returns the actual value from the html <th> element
                The value stored in the JSON is residual
            */
            std::string lastPrice_html = "\"Last Price\">";
            unsigned int pos = html.find(lastPrice_html) + lastPrice_html.length();
            
            if(html[pos] == '-'){
                price = "-";
                change = "-";
            }
            else{
                price = Methods::refactorOutput(curr_price) + " USD";
                change = Methods::getFmtDataFromJson(curr_change);
            }
            
        }
    }
    
    /*
        Prints information about each company's stock
    */
    void show(){
        std::cout<<"\n"<<name<<":\n"<<"\tLast Price: \t\t"<<price <<'\n'<<"\tChange: \t\t"<<change<<'\n';
    }
};


#endif