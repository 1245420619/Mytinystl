#ifndef AFE_STACK_H
#define AFE_STACK_H
#include"deque.h"
namespace afe{

//declaration

template<class T,class Container=deque<T> >
class stack{
public:
    typedef typename Container::value_type  value_type;
    typedef typename Container::size_t      size_t;
    typedef typename Container::reference   reference;

protected:
    Container c;

public:
    bool empty() const      { return c.empty(); }
    size_t size() const     { return c.size(); }
    reference top()         { return c.back(); }
    void push(const T& x)   { c.push_back(x); }
    void pop()              { c.pop_back(); }

    friend bool operator==(const stack& x,const stack& y)
        return x.c==y.c;
    friend bool opertor<(const stack& x,const stack& y)
        return x.c<y.c;

};
}
#endif
