#ifndef AFE_ALLOCATOR_H
#define AFE_ALLOCATOR_H                        
#include"utility.h"     //is_trivially_copy_assignable?
                        //for move,forward
                        //is_trivially_destructible?
#include"iterator.h"    
namespace afe{

//declaration

template<class T>
class allocator{
public:
    static T* allocate();
    static T* allocate(size_t n);
    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_t n);

    static void construct(T* ptr);
    static void construct(T* ptr,const T& value);
    static void construct(T* ptr,T&& value);

    template<class...Args>
    static void construct(T* ptr,Args&& ...value);

    static void destroy(T* pointer);
    static void destroy(T* first, T* last);

};

//realize

//allocate

template<class T>
T* allocator<T>::allocate(){ 
    return static_cast<T*>(::operator new(sizeof(T))); 
}

template<class T>
T* allocator<T>::allocate(size_t n){
    if(n==0) 
        return nullptr;
    return static_cast<T*>(::operator new(n* sizeof(T)));
}

//deallocate

template<class T>
void allocator<T>::deallocate(T* ptr){
    if(ptr==nullptr)
        return;
    ::operator delete(ptr);
}

template <class T>
void allocator<T>::deallocate(T* ptr, size_t n){
    if (ptr==nullptr)
        return;
    ::operator delete(ptr);
}

//construct

template<class T>
void allocator<T>::construct(T* ptr){
    ::new ((void*)ptr) T();
}

template<class T>
void allocator<T>::construct(T* ptr,const T& value){
    ::new ((void*)ptr) T(value);
}

template<class T>
void allocator<T>::construct(T* ptr,T&& value){
    ::new ((void*)ptr) T(move(value));
}

template <class T>
template <class ...Args>
void allocator<T>::construct(T* ptr, Args&& ...args){
    construct(ptr, forward<Args>(args)...);
}

//destory

template <class T>
void destroy_one(T*,true_type) {}

template <class T>
void destroy_one(T* pointer, false_type){
    if(pointer != nullptr)  pointer->~T();
}

template <class T>
void destroy_n(T*,T*,true_type) {}

template <class T>
void destroy_n(T* first, T* last, false_type){
    while(first!=last){
        destroy(&*first);
        first++;
    }
}

template <class T>
void allocator<T>::destroy(T* pointer){
    destroy_one(pointer, is_trivially_destructible<T>{});
}

template <class T>
void allocator<T>::destroy(T* first, T* last){
    destroy_n(first,last,is_trivially_destructible<T>{});
}

}

#endif
