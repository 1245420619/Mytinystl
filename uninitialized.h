#ifndef AFE_UNINITIALIZED_H
#define AFE_UNINITIALIZED_H
#include"algorithm.h"   //for copy,fill_n
#include"allocator.h"   //for construct,destory
                        //is_trivially_copy_assignable?
namespace afe{

//copy

template <class Intiter, class Fwditer>
Fwditer unchecked_uninit_copy(Intiter first, Intiter last, Fwditer result, true_type){
    return copy(first, last, result);
}

template <class Intiter, class Fwditer>
Fwditer unchecked_uninit_copy(Intiter first, Intiter last, Fwditer result, false_type){
    auto cur = result;
    try{
        while(first!= last){            
            construct(&*cur, *first);
            first++;
            cur++;            
        }
    }catch (...){
        while(result!=cur){
            destroy(&*cur);
            cur--;
        }
    }
    return cur;
}

template<class Intiter,class Fwditer>
Fwditer uninitialized_copy(Intiter first,Intiter last,Fwditer result){
    return unchecked_uninit_copy(
    first, last, result
    ,is_trivially_copy_assignable<typename iterator_traits<Fwditer>::value_type>{}
    );
}

//uninitialized_fill_n

template <class Fwditer, class Size, class T>
Fwditer unchecked_uninit_fill_n(Fwditer first, Size n, const T& value, true_type){
    return fill_n(first, n, value);
}

template <class Fwditer, class Size, class T>
Fwditer unchecked_uninit_fill_n(Fwditer first, Size n, const T& value, false_type){
    auto cur = first;
    try{
        while(n>0){
            construct(&*cur, value);
            n--;
            cur++;
        }
    }catch (...){
        while(first!=cur){
            destroy(&*first);
        }
    }
    return cur;
}

template <class Fwditer, class Size, class T>
Fwditer uninitialized_fill_n(Fwditer first, Size n, const T& value)
{
    return unchecked_uninit_fill_n(first, n, value
    , is_trivially_copy_assignable<typename iterator_traits<Fwditer>::value_type>{});
}

}
#endif
