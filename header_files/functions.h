#ifndef functions_h
#define functions_h

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
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

    static string toUpperCase(string& s){
        std::transform(s.begin(), s.end(), s.begin(), [](char ch){return toupper(ch);});
        return s;
    }

    static void readInput(string* output){
        string input;
        while(std::cin.peek() != '\n'){
            std::getline(std::cin, input);
            if(input != "\n"){
                output->append(input);
            }
        };
    }

    static inline void rtrim(string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    static inline vector<vector<string>> read_csv(string filename){
        
        //Thank you https://cppbyexample.com/parse_csv.html so much!!💗💗
        std::ifstream csv_input(filename);
        if(!csv_input.is_open()){
            std::cerr<<"Couldn't read file: "<<filename<<'\n';
            return vector<vector<string>>();
        }

        vector<vector<string>> csvRows;

        for(string line; std::getline(csv_input, line);){
            std::istringstream ss(std::move(line));
            std::vector<string> row;
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

    static inline string getUniqueStrings(vector<string>& vec){
        return string();
    }

    static inline string html_erase(string& html, string s){
        size_t pos = html.find(s);
        html.erase(html.begin(), html.begin() + pos + s.length());
        s = html.substr(0, html.find("<"));
        return s;
    }
};

#endif