#ifndef AFE_QUEUE_H
#define AFE_QUEUE_H
#include"deque.h"
#include"vector.h"
#include"functional.h"
#include"algorithm.h"
namespace afe{

//declaration

template<class T,class Container=deque<T> >
class queue{
public:
    typedef typename Container::value_type          value_type;
    typedef typename Container::size_type           size_type;
    typedef typename Container::reference           reference;
    typedef typename Container::const_reference     const_reference;

protected:
    Container c;

public:
    bool empty() const              { return c.empty(); }
    size_type size() const          { return c.size(); }
    reference front()               { return c.front(); }
    const_reference front() const   { return c.front(); } 
    reference back()                { return c.back(); }
    const_reference back()  const   { return c.back(); }       
    void push(const T& x)           { return c.push_back(x); }
    void pop()                      { return c.pop_front(); }


    friend bool operator==(const queue& x,const queue& y)
        { return x.c==y.c; }
    friend bool operator<(const queue& x,const queue& y)
        { return x.c<y.c; }


};

template<class T,class Container=vector<T>,class Compare=less<typename Container::value_type> >
class priority_queue{
public:
    typedef typename Container::value_type          value_type;
    typedef typename Container::size_type           size_type;
    typedef typename Container::reference           reference;
    typedef typename Container::const_reference     const_reference;

protected:
    Container c;
    Compare comp;

public:
    priority_queue(): c(),comp() {}
    explicit priority_queue(const Compare& x): c(),comp(x) {}

    template <class Inputiter>
    priority_queue(Inputiter first,Inputiter last,const Compare& x): c(first, last),comp(x) {
        make_heap(c.begin(),c.end(),comp);
    }

    template <class Inputiter>
    priority_queue(Inputiter first,Inputiter last): c(first, last) {
        make_heap(c.begin(),c.end(),comp);
    }

    bool empty() const          { return c.empty(); }
    size_type size() const      { return c.size(); }
    const_reference top() const { return c.front(); } 
    void push(const T& x) {
        c.push_back(x);
        push_heap(c.begin(),c.end(),comp);
    }

    void pop() {
        pop_heap(c.begin(),c.end(),comp);
        c.pop_back();
    }

};

}
#endif
