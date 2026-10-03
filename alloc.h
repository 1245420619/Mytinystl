#ifndef AFE_ALLOC_H
#define AFE_ALLOC_H
#include"allocator.h"
namespace afe{

//declaration

template<class T>
class simple_alloc{
public:
    static T* allocate(void);
    static T* allocate(size_t n);

    static void deallocate(T *p, size_t n);
    static void deallocate(T *p);

};

//realize

template<class T>
T* simple_alloc<T>::allocate(void){ 
    return (T*) allocator<T>::allocate(sizeof (T)); 
}

template<class T>
T* simple_alloc<T>::allocate(size_t n){ 
    return 0 == n ? 0 : (T*) allocator<T>::allocate(n * sizeof (T)); 
}

template<class T>
void simple_alloc<T>::deallocate(T *p, size_t n){ 
    if (0 != n)
        allocator<T>::deallocate(p, n * sizeof (T)); 
}

template<class T>
void simple_alloc<T>::deallocate(T *p){ 
    allocator<T>::deallocate(p, sizeof (T)); 
}

}
#endif
