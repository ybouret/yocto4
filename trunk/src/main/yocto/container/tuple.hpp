#ifndef YOCTO_TUPLE_INCLUDED
#define YOCTO_TUPLE_INCLUDED 1

#include "yocto/type/traits.hpp"
#include <iostream>

#define YOCTO_PAIR_DECL(CLASS,T1,N1,T2,N2) \
class CLASS \
{ \
public:\
typedef typename type_traits<T1>::parameter_type PARAM1;\
typedef typename type_traits<T2>::parameter_type PARAM2;\
inline CLASS( PARAM1 P1, PARAM2 P2) :\
N1(P1),N2(P2) {} \
inline CLASS(const CLASS &other) : \
N1(other.N1), N2(other.N2) {}\
inline ~CLASS() throw() {} \
friend inline \
std::ostream & operator<<( std::ostream &os, const CLASS &self )\
{\
os << '(' << self.N1 << ',' << self.N2 << ')';\
return os;\
}\
T1 N1;\
T2 N2

#define YOCTO_PAIR_END() }


#define YOCTO_TRIPLE_DECL(CLASS,T1,N1,T2,N2,T3,N3) \
class CLASS \
{ \
public:\
typedef typename type_traits<T1>::parameter_type PARAM1;\
typedef typename type_traits<T2>::parameter_type PARAM2;\
typedef typename type_traits<T3>::parameter_type PARAM3;\
inline CLASS( PARAM1 P1, PARAM2 P2, PARAM3 P3) :\
N1(P1),N2(P2),N3(P3) {} \
inline CLASS(const CLASS &other) : \
N1(other.N1), N2(other.N2), N3(other.N3) {}\
inline ~CLASS() throw() {} \
friend inline \
std::ostream & operator<<( std::ostream &os, const CLASS &self )\
{\
os << '(' << self.N1 << ',' << self.N2 << ',' << self.N3 << ')';\
return os;\
}\
T1 N1;\
T2 N2;\
T3 N3

#define YOCTO_TRIPLE_END() }


#endif

