#include <iostream>
#include <string>
#include <bits/stdc++.h>
using std::string;

long repeatedString(string s, long n) {
    if(s.size() == 1 && s == "a"){
        return n;
    }
    
    long nr = 0;
    auto count = [](char c) -> bool{return c == 'a';};
    nr = std::count_if(s.begin(), s.end(), count);
    
    if(n % s.size() == 0){
        return nr = nr * (n / s.size());
    }else{
        unsigned int str_size = n / s.size();
        nr *= str_size;
        for(int i = n % s.size(); i >= 0; --i)
            nr += count(s[i]);
        return nr;
    }
}

int main(){
    std::cout<<repeatedString("bcbccacaacbbacabcabccacbccbababbbbabcccbbcbcaccababccbcbcaabbbaabbcaabbbbbbabcbcbbcaccbccaabacbbacbc", 649606239668);
}