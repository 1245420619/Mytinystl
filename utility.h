#ifndef AFE_UTILITY_H
#define AFE_UTILITY_H
#include"type_traits.h"
namespace afe{

// move

template<class T>
typename remove_reference<T>::type&& move(T&& arg) noexcept{
    return static_cast<typename remove_reference<T>::type&&>(arg);
}

//forward

template<class T>
T&& forward(typename remove_reference<T>::type& arg) noexcept{
    return static_cast<T&&>(arg);
}

template<class T>
T&& forward(typename remove_reference<T>::type&& arg) noexcept{
    static_assert(!is_lvalue_reference<T>::value, "forward wrong");
    return static_cast<T&&>(arg);
}

}

#endif
