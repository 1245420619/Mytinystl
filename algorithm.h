#ifndef AFE_ALGORITHM_H
#define AFE_ALGORITHM_H
#include"iterator.h"
namespace afe{

//copy

template <class Intiter, class Outiter>
Outiter unchecked_copy_cat(Intiter first, Intiter last, Outiter result,input_iterator_tag){
  for (; first != last; ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

template <class Rdmiter, class Outiter>
Outiter unchecked_copy_cat(Rdmiter first, Rdmiter last, Outiter result,random_access_iterator_tag){
  for (auto n = last - first; n > 0; --n, ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

template <class Intiter, class Outiter>
Outiter copy(Intiter first, Intiter last, Outiter result){
  return unchecked_copy_cat(first,last,result,iterator_category(first));
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

}
#endif
