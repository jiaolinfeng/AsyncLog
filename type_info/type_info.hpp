/**
 * @file type_info.hpp
 * @brief
 * @author JiaoLinfeng, jiaolinfeng@esunny.cc
 * @version 1.0
 * @date 2017-10-28
 */

#ifndef _TYPE_INFO_HPP_
#define _TYPE_INFO_HPP_

#if __cplusplus >= 201103L

#include <string>
#include <iostream>
#include <boost/type_index.hpp>

namespace jlf
{

template <typename T>
struct typeinfo
{
    static std::string get()
    {
        return boost::typeindex::type_id_with_cvr<T>().pretty_name();
    }

    static void show()
    {
        std::cout <<  get() << '\n';
    }
};

}

#endif // __cplusplus

#endif // header protector
