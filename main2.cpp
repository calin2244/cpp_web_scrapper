#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <json/json.h>
#include <fstream>
#include <set>
#include <nlohmann/json.hpp>
#include "program_settings.h"
#include <ios>
#include <sstream>
#include <iomanip>
using std::map;

using json = nlohmann::json;


int main() {
  std::string filename{"csv.txt"};
  std::ifstream input{filename};

  if (!input.is_open()) {
    std::cerr << "Couldn't read file: " << filename << "\n";
    return 1; 
  }

  std::vector<std::vector<std::string>> csvRows;

  for (std::string line; std::getline(input, line);) {
    std::istringstream ss(std::move(line));
    std::vector<std::string> row;
    if (!csvRows.empty()) {
       // We expect each row to be as big as the first row
      row.reserve(csvRows.front().size());
    }
    // std::getline can split on other characters, here we use ','
    for (std::string value; std::getline(ss, value, ',');) {
      row.push_back(std::move(value));
    }
    csvRows.push_back(std::move(row));
  }

  // Print out our table
  for(unsigned int i = +1; i < csvRows.size(); ++i){
    for(unsigned int j = 0; j < csvRows[i].size(); ++j){
        std::cout<<csvRows[i][0]<<" "<<'\n';
    }
  }
}
