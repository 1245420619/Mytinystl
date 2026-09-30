#ifndef AFE_TYPE_TRAITS_H
#define AFE_TYPE_TRAITS_H
namespace afe{

template <class...>
using void_t = void;

//integral_constant

template <class T, T V>
struct integral_constant {
    static constexpr T value = V;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <class _Tp, class _Up>
struct is_trivially_assignable
    : public integral_constant<bool, __is_trivially_assignable(_Tp, _Up)> {};

template <class _Tp>
struct is_trivially_destructible
    : public integral_constant<bool, __has_trivial_destructor(_Tp)> {};

// type_traits remove_reference

template<class T>
struct remove_reference{
    typedef T type;
};

template<class T> 
struct remove_reference<T&>{
    typedef T type;
};

template<class T>
struct remove_reference<T&&>{
    typedef T type;
};

//is_lvalue_reference

template <class T>
struct is_lvalue_reference : public false_type {};

template <class T>
struct is_lvalue_reference<T&> : public true_type {};

//add_lvalue_reference

template <class T,class = void >
struct __add_lvalue_reference_helper {
    using type = T;
};

template <class T>
struct __add_lvalue_reference_helper<T, void_t<T&> > {
    using type = T&;
};

template <class T>
struct add_lvalue_reference : __add_lvalue_reference_helper<T> {};

//is_trivially_copy_assignable

template<class T>
struct is_trivially_copy_assignable: is_trivially_assignable<
          typename add_lvalue_reference<T>::type,
          typename add_lvalue_reference<const T>::type
      > {};

}
#endif
