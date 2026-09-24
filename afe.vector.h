#ifndef AFE_VECTOR_H_
#define AFE_VECTOR_H_ 
#include<memory>
#include<algorithm>

template<class T>

class vector{
public:
    typedef T value_type;
    typedef value_type * iterator;
    typedef value_type & reference;
    typedef std::allocator<T> allocator_type;
protected:
    iterator start;
    iterator finish;
    iterator end_of_storage;
    allocator_type alloc;

    void insert_aux(iterator position,const T& x);
    void fill_initialize(size_t n,const T& x){
        if(n==0){
            start=0;
        }else{
            start=alloc.allocate(n);
            std::uninitialized_fill_n(start,n,x);
        }
        finish=start+n;
        end_of_storage=finish;
    }
    void deallocate(){
        if(start){
            size_t n=end_of_storage-start;
            if(n!=0){
                alloc.deallocate(start,n);
            }
        }
    }
public:
    iterator begin(){return start;}
    iterator end(){return finish;}
    size_t size(){return finish-start;}
    size_t capacity(){return end_of_storage-start;}
    bool empty(){return start==finish;}
    reference operator[](size_t n){ return *(start+n);}

    vector():start(0),finish(0),end_of_storage(0) { }
    explicit vector(size_t n) { fill_initialize(n,value_type() ); }
    vector(size_t n,const T& value){ fill_initialize(n,value); }
    ~vector() { 
        deallocate();
    }

    reference front() { return *start; }
    reference back() { return *(finish-1) ;}

    void push_back(const T& x){
        if(finish!=end_of_storage){
            new (finish) T(x);
            finish++;
        }
        else 
            insert_aux(finish,x); 
    }
    void pop_back(){
        finish--;
        finish->~T();
    }
    iterator erase(iterator position){
        if(position+1!=finish)
            std::copy(position+1,finish,position);
        finish--;
        finish->~T();
        return position;
    }
    iterator erase(iterator first,iterator last){
        iterator new_finish=std::copy(last,finish,first);
        for(iterator p=new_finish;p!=finish;++p){
            p->~T();
        }
        finish=new_finish;
        return first;
    }
    void resize(size_t new_size,const T& x){
        if(new_size<size())
            erase(start+new_size,finish);
        else
            insert(finish,new_size-size(),x);
    }
    void insert(iterator position,size_t n,const T& x){
        if(n==0) return;
        if(end_of_storage-finish>=n){
            finish = std::uninitialized_fill_n(position, n, x);
            return;
        }

        size_t new_size=size()+n;
        iterator new_start = alloc.allocate(new_size);
        iterator new_finish = new_start;
        try{
            new_finish = std::uninitialized_copy(start,position,new_start);
            new_finish = std::uninitialized_fill_n(new_finish, n, x);
            new_finish=std::uninitialized_copy(position,finish,new_finish);
        }catch(...){
            for(iterator p = new_start ; p != new_finish ; p++){
                p->~T();
            alloc.deallocate(new_start,new_size);
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
    void resize(size_t new_size) { resize(new_size,value_type()); }
    void clear() { erase(start,finish); }
};

template<class T>

void vector<T>::insert_aux(iterator position,const T& x){
    const size_t old_size =size();
    const size_t new_size=old_size==0?1:2*old_size;
    iterator new_start =alloc.allocate(new_size);
    iterator new_finish = new_start;
    try{
        new_finish = std::uninitialized_copy(start,position,new_start);
        new (new_finish) T(x);
        new_finish++;
        new_finish=std::uninitialized_copy(position,finish,new_finish);
    }catch(...){
        for(iterator p = new_start ; p != new_finish ; p++){
            p->~T();
        alloc.deallocate(new_start,new_size);
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
#endif
