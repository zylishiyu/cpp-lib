#include <iostream>
#include <algorithm>
#include <functional>
#include <ext/functional>
#include <algorithm>
#include <cstdlib>
#include <numeric>
#include <ext/numeric>
#include <vector>
#include <iterator>
#include <ext/hash_map>

namespace std {
    using namespace __gnu_cxx;
}

using std::ostream_iterator;
using std::cout;
using std::endl;
using std::vector;
using std::bind2nd;
using std::less;
using std::not1;
using std::copy;
using std::compose1;
using std::multiplies;
using std::plus;
using std::ptr_fun;
using std::mem_fun_ref;

void print(int i) {
    cout << i << ' ' ;
}

class Int {
    public:
        explicit Int(int i) : _i(i) {}
        void print1() const {
            cout << '[' << _i << ']';
        }
    private:
        int _i;
};

void functor_adaptor_test() {
    ostream_iterator<int> out_iter(cout, " ");

    int ia[6] = {2, 21, 12, 7, 19, 23};
    vector<int> iv(ia, ia+6);
    cout << count_if(iv.begin(), iv.end(), not1(bind2nd(less<int>(), 12)));
    cout << endl;

    transform(iv.begin(), iv.end(), out_iter, compose1(bind2nd(multiplies<int>(), 3), bind2nd(plus<int>(), 2) )); 
    cout << endl;

    const int count = 5;
    char* foo[] = {"10","20","30","40","50"};
    int   bar[count];
    transform(foo, foo + count, bar, ptr_fun(atoi));
    copy(bar, bar + count, out_iter);
    cout << endl;

    Int t1(3), t2(7), t3(20), t4(14), t5(68);
    vector<Int> Iv;
    Iv.push_back(t1);
    Iv.push_back(t2);
    Iv.push_back(t3);
    Iv.push_back(t4);
    Iv.push_back(t5);
    for_each(Iv.begin(), Iv.end(), mem_fun_ref(&Int::print1));
    cout << endl;
}

int main() {
    functor_adaptor_test();

    return 0;
}

