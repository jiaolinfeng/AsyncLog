#include <iostream>
#include <vector>
#include "type_info.hpp"

int main()
{
    using namespace std;

#if __cplusplus >= 201103L
    auto i = 10;
    auto& pi = i;

    cout << jlf::typeinfo<decltype(i)>::get() << endl;
    cout << jlf::typeinfo<decltype(pi)>::get() << endl;

    std::vector<std::vector<int>> double_vec;

    jlf::typeinfo<decltype(double_vec)>::show();
    jlf::typeinfo<std::vector<int>::iterator>::show();
#endif

    return 0;
}
