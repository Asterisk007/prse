#pragma once
#include <vector>
#include <string>

using namespace std;

class List {
public:
   vector<string> list;
   List();
   void push_back(string);
   void append_list(List*);
   string get_item(int);
   int size();
};