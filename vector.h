#ifndef AFE_VECTOR_H_
#define AFE_VECTOR_H_
#include"uninitialized.h"   //for algorithm 
#include"alloc.h"   
namespace afe{

template<class T>
class vector{

//declaration

public:
    typedef T                   value_type;
    typedef T*                  iterator;
    typedef T&                  reference;
    
private:
    typedef simple_alloc<T> data_allocator;
    iterator start;
    iterator finish;
    iterator end_of_storage;

protected:
    void vallocate(iterator position,size_t n,const T& x,size_t new_size);
    void deallocate();
    void fill_initialize(size_t n,const T& x);
    
public:
    vector():start(0),finish(0),end_of_storage(0) { }
    explicit vector(size_t n)       { fill_initialize(n,value_type() );}
    vector(size_t n,const T& value) { fill_initialize(n,value);}
    vector(int n, const T& value)   { fill_initialize(n,value);}
    vector(long n, const T& value)  { fill_initialize(n,value);}
    ~vector()                       { deallocate();}

    iterator begin()                {return start;}
    iterator end()                  {return finish;}
    
    size_t size()                   {return finish-start;}
    size_t capacity()               {return end_of_storage-start;}
    bool empty()                    {return start==finish;}

    reference operator[](size_t n)  { return *(start+n);}
    reference front()               { return *start; }
    reference back()                { return *(finish-1) ;}


    void push_back(const T& x);
    void insert(iterator position,const T& x);
    void insert(iterator position,size_t n,const T& x);
    
    void pop_back();
    iterator erase(iterator position);
    iterator erase(iterator first,iterator last);
    void clear()                    { erase(start,finish); }

    void resize(size_t new_size,const T& x);
    void resize(size_t new_size)    { resize(new_size,value_type()); }
    
};

//realize

//allocate

template<class T>
void vector<T>::vallocate(iterator position,size_t n,const T& x,size_t add_size){
    size_t new_size=size()+add_size;
    iterator new_start = data_allocator::allocate(new_size);
    iterator new_finish = new_start;
    try{
        new_finish=uninitialized_copy(start,position,new_start);
        new_finish=uninitialized_fill_n(new_finish, n, x);
        new_finish=uninitialized_copy(position,finish,new_finish);
    }catch(...){
        for(iterator p = new_start ; p != new_finish ; p++){
            p->~T();
        data_allocator::deallocate(new_start,new_size);
        throw;
        }
    }
    
    for(iterator p =start ;p!= finish ; p++ ){
        p->~T();
    }
    deallocate();
    start=new_start;
    finish=new_finish;
    end_of_storage= new_start+new_size;
}

template<class T>
void vector<T>::deallocate(){
    if(start)
        data_allocator::deallocate(start,end_of_storage-start);
}


template<class T>
void vector<T>::fill_initialize(size_t n,const T& x){
    start=data_allocator::allocate(n);
    uninitialized_fill_n(start,n,x);
    finish=start+n;
    end_of_storage=finish;
}

//insert

template<class T>
void vector<T>::push_back(const T& x){
    if(finish!=end_of_storage){
        new (finish) T(x);
        finish++;
    }else 
        vallocate(finish,1,x,size()); 
}

template<class T>
void vector<T>::insert(iterator position,const T& x){
    insert(position,1,x);
}

template<class T>
void vector<T>::insert(iterator position,size_t n,const T& x){
    if(n==0)
        return;
    else if(end_of_storage-finish>=n)
        finish = uninitialized_fill_n(position, n, x);
    else{
        size_t need_capacity=capacity()*2;
        size_t need_size=size()+n;
        while(need_capacity<need_size){
            need_capacity*=2;
        }
        vallocate(position,n,x,need_capacity);
    }
}

//delete

template<class T>
void vector<T>::pop_back(){
    finish--;
    finish->~T();
}

template<class T>
typename vector<T>::iterator vector<T>::erase(iterator position){
    if(position+1!=finish)
        copy(position+1,finish,position);
    finish--;
    finish->~T();
    return position;
}

template<class T>
typename vector<T>::iterator vector<T>::erase(iterator first,iterator last){
    iterator new_finish=copy(last,finish,first);
    for(iterator p=new_finish;p!=finish;++p){
        p->~T();
    }
    finish=new_finish;
    return first;
}

//resize

template<class T>
void vector<T>::resize(size_t new_size,const T& x){
    if(new_size<size())
        erase(start+new_size,finish);
    else
        insert(finish,new_size-size(),x);
}

}

#endif
