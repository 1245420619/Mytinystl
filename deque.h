#ifndef AFE_DEQUE_H
#define AFE_DEQUE_H
#include"uninitialized.h"
#include"alloc.h"
namespace afe{

//declaration

template<class T,class Ptr,class Ref,size_t Bufsize=0 >
struct deque_iterator{
    typedef deque_iterator              self;
    typedef deque_iterator<T,T*,T&,Bufsize>             iterator;
    typedef deque_iterator<T,const T*,const T&,Bufsize> const_iterator;

    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef Ptr                         pointer;
    typedef Ref                         reference;
    typedef ptrdiff_t                   difference_type;

    typedef T**                         map_pointer;

    pointer cur;
    pointer first;
    pointer last;
    map_pointer node;

    static size_t buffer_size();

    deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}

    void set_node(map_pointer new_node);

    reference operator*() const                     { return *cur; }
    reference operator[](difference_type n) const   { return *(*this+n); }
    pointer operator->() const                      { return &(operator*()); }

    difference_type operator-(const self& x) const;
        
    self& operator++();
    self operator++(int);
    self& operator--();
    self operator--(int);
    self& operator+=(difference_type n);
    self operator+(difference_type n) const;
    self& operator-=(difference_type n);
    self operator-(difference_type n) const;
      
    bool operator==(const self& x) const            { return cur==x.cur; }
    bool operator!=(const self& x) const            { return !(*this==x); }
    bool operator>(const self& x) const             { return x < *this; }
    bool operator<=(const self& x) const            { return !(x < *this); }
    bool operator>=(const self& x) const            { return !(*this < x); }
    bool operator<(const self& x) const             {
        return (node==x.node) ? (cur<x.cur) : (node<x.node); 
    }


};

template<class T,size_t Bufsize=0 >
class deque{
public:
    typedef T                   value_type;
    typedef T*                  pointer;
    typedef T&                  reference;
    typedef ptrdiff_t           difference_type;
    typedef pointer*            map_pointer;
    typedef deque_iterator<T,T*,T&,Bufsize> iterator;

    typedef simple_alloc<value_type>        data_allocator;
    typedef simple_alloc<pointer>           map_allocator;

protected:
    iterator start;
    iterator finish;
    map_pointer map;
    size_t  map_size;

protected:
    static size_t buffer_size();

    void fill_initialize(size_t n,const T& x);

public:
    deque(size_t n,const T& x):start(),finish(),map(0),map_size(0){ 
        fill_initialize(n,x); }
    ~deque(){
        clear();
        destroy_map_and_nodes();
    }
    void create_map_and_nodes(size_t num_elements);
    void destroy_map_and_nodes();
    static size_t initial_map_size() { return 8; }
    pointer allocate_node();
    void deallocate_node(pointer n);
    
    void reserve_map_at_back(size_t add_nodes = 1);
    void reserve_map_at_front(size_t add_nodes = 1);
    void reallocate_map(size_t add_nodes,bool isfront);

    iterator begin()    { return start; }
    iterator end()      { return finish; }
    reference operator[](size_t n)  { return start[difference_type(n)]; }
    reference front()   { return *start; }
    reference back();

    size_t size() const { return finish-start; }
    size_t max_size() const { return size_t(-1); }
    bool empty() const  { return finish==start; }
    void push_back(const T& x);
    void push_back_aux(const T& x);
    void push_front(const T& x);
    void push_front_aux(const T& x);
    void pop_back();
    void pop_back_aux();
    void pop_front();
    void pop_front_aux();
    void clear();
    iterator erase(iterator pos);
    iterator erase(iterator first,iterator last);
    iterator insert(iterator position,const T& x);
    iterator insert_aux(iterator position,const T& x);
};



//realize

//deque_iterator

template<class T,size_t Bufsize>
typename deque<T,Bufsize>::reference 
deque<T,Bufsize>::back(){
    iterator tmp=finish;
    --tmp;
    return *tmp;
}

template<class T,class Ptr,class Ref,size_t Bufsize >
inline size_t deque_iterator<T,Ptr,Ref,Bufsize>::buffer_size(){
    return Bufsize != 0 ? Bufsize : (sizeof(T) < 512 ? size_t(512/sizeof(T)) : size_t(1) );
}

template<class T,class Ptr,class Ref,size_t Bufsize >
void deque_iterator<T,Ptr,Ref,Bufsize>::set_node(map_pointer new_node){
    node=new_node;
    first=*new_node;
    last=first+difference_type(buffer_size());
}

//deque

template<class T,size_t Bufsize >
inline size_t deque<T,Bufsize>::buffer_size(){
    return Bufsize != 0 ? Bufsize : (sizeof(T) < 512 ? size_t(512/sizeof(T)) : size_t(1) );
}

template<class T,size_t Bufsize>
typename deque<T,Bufsize>::iterator 
deque<T,Bufsize>::insert(iterator position,const T& x){
    if(position.cur==start.cur){
        push_front(x);
        return start;
    }else if(position.cur==finish.cur){
        push_back(x);
        iterator tmp=finish;
        tmp--;
        return tmp;
    }else{
        return insert_aux(position,x);
    }
}

template<class T,size_t Bufsize>
typename deque<T,Bufsize>::iterator 
deque<T,Bufsize>::insert_aux(iterator position,const T& x){
    difference_type index=position-start;
    T x_copy=x;
    if(index<(size()/2)){
        push_front(front());
        iterator front1=start;
        front1++;
        iterator front2=front1;
        front2++;
        position=start+index;
        iterator pos1=position;
        pos1++;
        copy(front2,pos1,front1);
    }else{
        push_back(back());
        iterator back1=finish;
        back1--;
        iterator back2=back1;
        back2--;
        position=start+index;
        copy_backward(position,back2,back1);
    }
    *position=x_copy;
    return position;
}

template<class T,size_t Bufsize>
typename deque<T,Bufsize>::iterator 
deque<T,Bufsize>::erase(iterator pos){
    iterator next=pos;
    ++next;
    difference_type index=pos-start;
    if(index<(size()/2)){
        copy_backward(start,pos,next);
        pop_front();
    }else{
        copy(next,finish,pos);
        pop_back();
    }
    return start+index;
}

template<class T,size_t Bufsize>
typename deque<T,Bufsize>::iterator 
deque<T,Bufsize>::erase(iterator first,iterator last){
    if(first==start&&last==finish){
        clear();
        return finish;
    }else{
        difference_type n=last-first;
        difference_type n_before=first-start;
        if(n_before<(size()/2)){
            copy_backward(start,first,last);
            iterator new_start=start+n;
            destroy(start,new_start);
            for(map_pointer cur=start.node;cur<new_start.node;cur++)
                data_allocator::deallocate(*cur,buffer_size());
            start=new_start;
        }else{
            copy(last,finish,first);
            iterator new_finish=finish-n;
            destroy(new_finish,finish);
            for(map_pointer cur=new_finish.node+1;cur<=finish.node;cur++)
                data_allocator::deallocate(*cur,buffer_size());
            finish=new_finish;
        }
        return start+n_before;
    }
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::clear(){
    for(map_pointer node=start.node+1;node<finish.node;node++){
        destroy(*node,*node+buffer_size());
        deallocate_node(*node);
    }
    if(start.node!=finish.node){
        destroy(start.cur,start.last);
        destroy(finish.first,finish.cur);
        deallocate_node(finish.first);
    }else
        destroy(start.cur,finish.cur);
    finish=start;
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::pop_back(){
    if(finish.cur!=finish.first){
        --finish.cur;
        destroy(finish.cur);    
    }else
        pop_back_aux();
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::pop_back_aux(){
    deallocate_node(finish.first);
    finish.set_node(finish.node-1);
    finish.cur=finish.last-1;
    destroy(finish.cur);
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::pop_front(){
    if(start.cur!=start.last-1){
        destroy(start.cur);    
        ++start.cur;
    }else
        pop_front_aux();
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::pop_front_aux(){
    destroy(start.cur);
    deallocate_node(start.first);
    start.set_node(start.node+1);
    start.cur=start.first;
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::push_back(const T& x){
    if(finish.cur!=finish.last-1){
        construct(finish.cur,x);    
        ++finish.cur;
    }else
        push_back_aux(x);
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::push_back_aux(const T& x){
    T temp=x;
    reserve_map_at_back();
    *(finish.node+1)=allocate_node();
    try{
        construct(finish.cur,temp);
        finish.set_node(finish.node+1);
        finish.cur=finish.first;
    }catch(...){
        deallocate_node(*(finish.node+1));
        throw;
    }
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::push_front(const T& x){
    if(start.cur!=start.first){
        construct(start.cur-1,x);    
        --start.cur;
    }else
        push_front_aux(x);
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::push_front_aux(const T& x){
    T temp=x;
    reserve_map_at_front();
    *(start.node-1)=allocate_node();
    try{
        start.set_node(start.node-1);
        start.cur=start.last-1;
        construct(start.cur,temp);
    }catch(...){
        start.set_node(start.node+1);
        start.cur=start.first;
        deallocate_node(*(start.node-1));
        throw;
    }
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::reserve_map_at_back(size_t add_nodes){
    if(add_nodes>map_size-(finish.node-map+1))
        reallocate_map(add_nodes,false);
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::reserve_map_at_front(size_t add_nodes){
    if(add_nodes>start.node-map)
        reallocate_map(add_nodes,true);
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::reallocate_map(size_t add_nodes,bool isfront){
    size_t old_nnum=finish.node-start.node+1;
    size_t new_nnum=old_nnum+add_nodes;
    map_pointer new_start;
    if(map_size>2*new_nnum){
        new_start=map+(map_size-new_nnum)/2+(isfront?add_nodes:0);
        if(new_start<start.node)
            copy(start.node,finish.node+1,new_start);
        else
            copy_backward(start.node,finish.node+1,new_start+old_nnum);
    }else{
        size_t new_map_size=map_size+max(map_size,add_nodes)+2;
        map_pointer new_map=map_allocator::allocate(new_map_size);
        new_start=new_map+(new_map_size-new_nnum)/2+(isfront?add_nodes:0);
        copy(start.node,finish.node+1,new_start);
        map_allocator::deallocate(map,map_size);
        map=new_map;
        map_size=new_map_size;
    }
    start.set_node(new_start);
    finish.set_node(new_start+old_nnum-1);
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::fill_initialize(size_t n,const T& x){
    create_map_and_nodes(n);
    map_pointer cur;
    try{
        for(cur=start.node;cur<finish.node;cur++)
            uninitialized_fill_n(*cur,buffer_size(),x);
        uninitialized_fill_n(finish.first,finish.cur-finish.first,x);
    }catch(...){
        for (map_pointer n = start.node; n < cur; ++n)
            destroy(*n, *n + buffer_size());
        destroy_map_and_nodes();
        throw;
    }
}

template<class T,size_t Bufsize>
void deque<T,Bufsize>::create_map_and_nodes(size_t num_elements){
    size_t num_nodes=num_elements/buffer_size()+1;
    map_size=max(initial_map_size(),num_nodes+2);
    map=map_allocator::allocate(map_size);
    map_pointer nstart=map+(map_size-num_nodes)/2;
    map_pointer nfinish=nstart+num_nodes-1;
    map_pointer cur;
    try{
        for(cur=nstart;cur<=nfinish;cur++)
            *cur=allocate_node();
    }catch(...){
        for (map_pointer n=nstart;n<cur;n++) {
            deallocate_node(*n);
        }
        map_allocator::deallocate(map, map_size);
        throw;
    }
    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    finish.cur = finish.first + num_elements % buffer_size();
}

template <class T,size_t Bufsize>
void deque<T, Bufsize>::destroy_map_and_nodes() {
    for (map_pointer cur = start.node; cur <= finish.node; ++cur)
        deallocate_node(*cur);
    map_allocator::deallocate(map, map_size);
}

template <class T,size_t Bufsize>
typename deque<T,Bufsize>::pointer 
deque<T, Bufsize>::allocate_node() {
    return data_allocator::allocate(buffer_size());
}

template <class T,size_t Bufsize>
void deque<T, Bufsize>::deallocate_node(pointer n) {
    data_allocator::deallocate(n, buffer_size());
}

//assignment

template<class T,class Ptr,class Ref,size_t Bufsize >
typename deque_iterator<T,Ptr,Ref,Bufsize>::difference_type 
deque_iterator<T,Ptr,Ref,Bufsize>::operator-(const self& x) const{
    return difference_type(buffer_size())*(node-x.node-1)+(cur-first)+(x.last-x.cur);
}

template<class T,class Ptr,class Ref,size_t Bufsize >
typename deque_iterator<T,Ptr,Ref,Bufsize>::self& 
deque_iterator<T,Ptr,Ref,Bufsize>::operator++(){
    cur++;
    if(cur==last){
        set_node(node+1);
        cur=first;
    }
    return *this;
}

template<class T,class Ptr,class Ref,size_t Bufsize >
typename deque_iterator<T,Ptr,Ref,Bufsize>::self 
deque_iterator<T,Ptr,Ref,Bufsize>::operator++(int){
    self temp=*this;
    ++(*this);
    return temp;
}

template<class T,class Ptr,class Ref,size_t Bufsize >
typename deque_iterator<T,Ptr,Ref,Bufsize>::self& 
deque_iterator<T,Ptr,Ref,Bufsize>::operator--(){
    if(cur==first){
        set_node(node-1);
        cur=last;
    }
    cur--;
    return *this;
}

template<class T,class Ptr,class Ref,size_t Bufsize >
typename deque_iterator<T,Ptr,Ref,Bufsize>::self 
deque_iterator<T,Ptr,Ref,Bufsize>::operator--(int){
    self temp=*this;
    --(*this);
    return temp;
}

template<class T,class Ptr,class Ref,size_t Bufsize >
typename deque_iterator<T,Ptr,Ref,Bufsize>::self& 
deque_iterator<T,Ptr,Ref,Bufsize>::operator+=(difference_type n){
    difference_type offset=n+cur-first;
    difference_type bfsize=difference_type(buffer_size());
    if(offset>=0&&offset<bfsize)
        cur+=n;
    else{
        difference_type node_offset=offset>0 ? offset/bfsize 
                                    : -difference_type((-offset-1)/bfsize)
                                    -1 ;
        set_node(node+node_offset);
        cur=first+(offset-node_offset*bfsize);
    }
    return *this;
}

template<class T,class Ptr,class Ref,size_t Bufsize >
typename deque_iterator<T,Ptr,Ref,Bufsize>::self 
deque_iterator<T,Ptr,Ref,Bufsize>::operator+(difference_type n) const {
    self temp=*this;
    return temp+=n;
}

template<class T,class Ptr,class Ref,size_t Bufsize >
typename deque_iterator<T,Ptr,Ref,Bufsize>::self& 
deque_iterator<T,Ptr,Ref,Bufsize>::operator-=(difference_type n){
    return *this+=-n;
}

template<class T,class Ptr,class Ref,size_t Bufsize >
typename deque_iterator<T,Ptr,Ref,Bufsize>::self 
deque_iterator<T,Ptr,Ref,Bufsize>::operator-(difference_type n) const {
    self temp=*this;
    return temp-=n;
}

//non-member function

template<class T, class Ptr, class Ref, size_t Bufsize>
inline deque_iterator<T, Ptr, Ref, Bufsize>
operator+(ptrdiff_t n, const deque_iterator<T, Ptr, Ref, Bufsize>& it) {
    return it + n;
}


}

#endif
