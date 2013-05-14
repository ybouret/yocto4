//! \file
/**
 \brief type list and related algorithms.
 
 */

#ifndef YOCTO_TYPE_LIST_INCLUDED
#define YOCTO_TYPE_LIST_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto {
	
	//! 'N/A' class for typelists.
	class null_type
	{
	};
	
	//! 'do nothing' class for typelists.
	struct empty_type
	{
	};
	
	
	//! basic type_list.
	template <class T, class U>
	struct type_list {
		typedef T head;
		typedef U tail;
	};
	
#define TL1(T1)                type_list< T1, null_type>
#define TL2(T1,T2)             type_list< T1, TL1(T2) >
#define TL3(T1,T2,T3)          type_list< T1, TL2(T2,T3) >
#define TL4(T1,T2,T3,T4)       type_list< T1, TL3(T2,T3,T4) >
#define TL5(T1,T2,T3,T4,T5)    type_list< T1, TL4(T2,T3,T4,T5) >
#define TL6(T1,T2,T3,T4,T5,T6) type_list< T1, TL5(T2,T3,T4,T5,T6) >
	
	
	namespace tl {
		
		//======================================================================
		// length
		//======================================================================
		//! default length algorithms.
		template <class TList> struct length_of;
		
		//! specialized length algorithm.
		template <> struct length_of<null_type> {
			enum { value = 0 };
		};
		
		//! recursive length algorithm.
		template <class T, class U>
		struct length_of< type_list<T,U> > {
			enum { value = 1 + length_of<U>::value };
		};
		
		//======================================================================
		// type_at
		//======================================================================
		//! default type_at algorithm.
		template <class TList, size_t index> struct type_at;
		
		//! specialized type_at algorithm.
		template <class head, class tail> struct type_at< type_list<head,tail>, 0> {
			typedef head result;
		};
		
		//! recursive type_at algorithm.
		template <class head, class tail, size_t i> struct type_at< type_list<head,tail>,i > {
			typedef typename type_at<tail,i-1>::result result;
		};
		
		
		//======================================================================
		// safe_type_at
		//======================================================================
		//! default safe_type_at algorithm.
		template <class TList, size_t index,
		typename default_type = null_type>
		struct safe_type_at
		{
			typedef default_type result;
		};
		
		//! specialized safe_type_at algorithm.
		template <class head, class tail, typename default_type>
		struct safe_type_at< type_list<head,tail>, 0, default_type> {
			typedef head result;
		};
		
		//! recursive safe_type_at algorithm.
		template <class head, class tail, size_t i, typename default_type>
		struct safe_type_at< type_list<head, tail>, i, default_type> {
			typedef typename
			safe_type_at< tail, i-1, default_type>::result result;
		};
		
		//======================================================================
		// index_of
		//======================================================================
		//! default index_of algorithm.
		template <class TList, class T> class index_of;
		
		//! specialized index_of algorithm (not found).
		template <class T>
		class index_of<null_type, T> {
		public:
			enum {
			    value = -1 //!< when not in type_list.
			};
		};
		
		//! specialized index_of algorithm (at tail).
		template <class T, class tail>
		class  index_of< type_list<T, tail>, T>
		{
		public:
			enum { value = 0 };
		};
		
		//! recursive index_of algorithm.
		template <class head, class tail, class T>
		class index_of<type_list<head, tail>, T> {
		private:
			enum { temp = index_of<tail, T>::value };
		public:
			enum { value = (temp == -1 ? -1 : 1 + temp) };
		};
		
		//======================================================================
		// append_to
		//======================================================================
		template <class TList,class T> struct append_to;
		
		template <> struct append_to<null_type,null_type> {
			typedef null_type result;
		};
		
		template <class T> struct append_to<null_type,T> {
			typedef TL1(T) result;
		};
		
		template <class head,class tail> struct append_to<null_type, type_list<head,tail> >
		{
			typedef type_list<head,tail> result;
		};
		
		template <class head,class tail,class T> struct append_to< type_list<head,tail>, T>
		{
			typedef type_list< head, typename append_to<tail,T>::result> result;
		};
		
	} // tl
	
	
}

#endif
