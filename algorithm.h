#ifndef AFE_ALGORITHM_H
#define AFE_ALGORITHM_H
#include"iterator.h"
namespace afe{

//copy

template <class Intiter, class Outiter>
Outiter copy(Intiter first, Intiter last, Outiter result){
  while(first!=last){
    *result=*first;
    first++;
    result++;
  }
  return result;
}

template <class Intiter, class Outiter>
Outiter copy_backward(Intiter first, Intiter last, Outiter result){
  while(first!=last){
    --last;
    --result;
    *result=*last;
  }
  return result;
}

//fill

template <class Outiter, class Size, class T>
Outiter fill_n(Outiter first, Size n, const T& value)
{
  while(n>0){
    *first = value;
    n--;
    first++;
  }
  return first;
}

//max

template<class T>
const T& max(const T& a, const T& b) {
    return a > b ? a : b;
}

//push_heap

template<class RandomIter,class Distance,class T>
void push_heap_aux2(RandomIter first,Distance holeindex,Distance topindex,T value){
    Distance parent=(holeindex-1)/2;
    while(holeindex>topindex&&*(first+parent)<value){
        *(first+holeindex)=*(first+parent);
        holeindex=parent;
        parent=(holeindex-1)/2;
    }
    *(first+holeindex)=value;
}

template<class RandomIter,class Distance,class T>
void push_heap_aux(RandomIter first,RandomIter last,Distance*,T*){
    push_heap_aux2(first,Distance((last-first)-1),Distance(0),T(*(last-1)));
}

template<class RandomIter>
void push_heap(RandomIter first,RandomIter last){
    push_heap_aux(first,last,difference_type(first),value_type(first));
}

//adjust_heap

template<class RandomIter,class Distance,class T>
void adjust_heap(RandomIter first,Distance holeindex,Distance len,T value){
    Distance topindex=holeindex;
    Distance rchild=2*holeindex+2;
    while(rchild<len){
        if(*(first+rchild)<*(first+rchild-1))
            rchild--;
        *(first+holeindex)=*(first+rchild);
        holeindex=rchild;
        rchild=2*rchild+2;
    }
    if(rchild==len){
        *(first+holeindex)=*(first+rchild-1);
        holeindex=rchild-1;
    }
    push_heap_aux2(first,holeindex,topindex,value);
}

//pop_heap

template<class RandomIter,class Distance,class T>
void pop_heap_aux2(RandomIter first,RandomIter last,RandomIter result,T value,Distance*){
    *result=*first;
    adjust_heap(first,Distance(0),Distance(last-first),value);
}

template<class RandomIter,class T>
void pop_heap_aux(RandomIter first,RandomIter last,T*){
    pop_heap_aux2(first,last-1,last-1,T(*(last-1)),difference_type(first));
}

template<class RandomIter>
void pop_heap(RandomIter first,RandomIter last){
    pop_heap_aux(first,last,value_type(first));
}

//sort_heap

template<class RandomIter>
void sort_heap(RandomIter first,RandomIter last){
    while(last-first>1){
        pop_heap(first,last--);
    }
}

template<class RandomIter,class Distance,class T>
void make_heap_aux(RandomIter first,RandomIter last,Distance*,T*){
    if(last-first<2) return;
    Distance len=last-first;
    Distance holeindex=(len-2)/2;
    adjust_heap(first,holeindex,len,*(first+holeindex));
    while(holeindex!=0){
        holeindex--;
        adjust_heap(first,holeindex,len,*(first+holeindex));
    }
}

template<class RandomIter>
void make_heap(RandomIter first,RandomIter last){
    make_heap_aux(first,last,value_type(first),difference_type(first));
}

}
#endif
