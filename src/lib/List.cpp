#include "List.h"

List::List() {
   list = vector<string>();
}

void List::push_back(string s) {
    list.push_back(s);
}

void List::append_list(List* l){
    if (l == nullptr) { return; }
    if (l->size() > 0)
        list.insert(list.end(), l->list.begin(), l->list.end());
}

string List::get_item(int i) {
    if ((int)list.size() <= i) {
        return "";
    } else {
        return list[i];
    }
}

int List::size() {
    return (int)list.size();
}