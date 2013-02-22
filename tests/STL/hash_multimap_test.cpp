#include <iostream>
#include <string>
#include <map>
#ifdef __GNUC__                                                                 
#include <ext/hash_map>                                                         
#else                                                                           
#include <hash_map>                                                             
#endif                                                                          

namespace std {
    using namespace __gnu_cxx;
}

struct string_hash_func {
    size_t operator() (const std::string &s) const {
        unsigned long h = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            h = 5 * h + s[i];
        }
        return size_t(h);
    }
};

typedef std::string map_key;
typedef int         map_value;
typedef std::hash_multimap< map_key, map_value, string_hash_func > map_type;
//typedef std::multimap< map_key, map_value > map_type;
typedef std::pair< map_key, map_value > map_element;
typedef map_type::iterator map_iter;

void map_test() {
    map_type    map_instance;
    map_key     key("key", 3);
    map_value   value = 100;
    map_element element(key, value);
    int str_length = 7;
    std::string s("aaa\0bbb", str_length);
    for (size_t i = 0; i < s.size(); ++i) {
        printf("%c", s[i]);
    }
    printf("\n");
    std::string s2("aaa");
    printf("%d\n", s == s2);

    map_instance.insert(element);
    map_instance.find(key);
    printf("success.\n");
}

int main() {
    map_test();

    return 0;
}

