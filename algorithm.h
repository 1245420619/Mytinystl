#ifndef AFE_ALGORITHM_H
#define AFE_ALGORITHM_H
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

}
#endif
