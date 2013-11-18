/** \file type_traits.hpp
 \brief Types traits settings.
 */

#ifndef YOCTO_TYPE_TRAITS_INCLUDED
#define YOCTO_TYPE_TRAITS_INCLUDED 1


#include "yocto/type/list.hpp"

namespace yocto
{
	
	//! default is_same_type helper
	template <class T,class U>
	struct is_same_type
	{
		enum { value = false };
	};
	
	//! specialized is_same_type
	template <class T>
	struct is_same_type<T,T>
	{
		enum { value = true };
	};
	
	namespace tl {
		
		typedef TL5(signed char, short, int, long, long long)                                       std_sints_list;
		typedef TL5(unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long) std_uints_list;
		typedef TL4(uint8_t,uint16_t,uint32_t,uint64_t)                         sys_uints_list;
		typedef TL4(int8_t,int16_t,int32_t,int64_t)                             sys_sints_list;
		typedef TL2(bool,char)                                                  misc_ints_list;
		typedef TL3(float,double,long double)                                   std_reals_list;
		
		
	}
	
	
	//! type traits.
	template <class T>
	class type_traits
	{
		//==================================================================
		// changing qualifiers
		//==================================================================
	private:
		template <class U>
		struct unconst
		{
			typedef U result;
			enum { value = false };
		};
		
		template <class U>
		struct unconst<const U>
		{
			typedef U result;
			enum { value = true };
		};
		
		template <class U>
		struct unconst<const U *>
		{
			typedef U *result;
			enum { value = true };
		};
		
		template <class U>
		struct unconst<const U &>
		{
			typedef U &result;
			enum { value = true };
		};
		
		
		
		template <class U>
		struct unvolatile
		{
			typedef U result;
			enum { value = false };
		};
		
		template <class U>
		struct unvolatile<volatile U>
		{
			typedef U result;
			enum { value = true };
		};
		
		template <class U>
		struct unvolatile<volatile U *>
		{
			typedef U *result;
			enum { value = true };
		};
		
		template <class U>
		struct unvolatile<volatile U &>
		{
			typedef U &result;
			enum { value = true };
		};
		
		
	public:
		typedef typename unconst<T>::result    mutable_type; //!< non const 'T'
		enum { is_const = unconst<T>::value /*!< true if T is const */ };
		
		typedef T                   item_type;
		
		
		typedef typename unvolatile<T>::result non_volatile_type; //!< non volatile 'T'
		enum { is_volatile = unvolatile<T>::value /*!< true if T is volatile */};
		
		typedef typename unvolatile< typename unconst<T>::result >::result non_qualified_type; //!< unqualified 'T'
		enum { is_qualified = is_const || is_volatile /*!< true if T is qualified */};
		
		
		
		//==================================================================
		// is_pointer
		//==================================================================
	private:
		template <class U>
		struct pointer_traits
		{
			enum { value = false };
			typedef null_type pointee_type;
		};
		
		template <class U>
		struct pointer_traits<U *>
		{
			enum { value = true };
			typedef U pointee_type;
		};
		
	public:
		enum { is_pointer = pointer_traits<T>::value /*!< true if 'T' can be written as 'U *'. */ };
		typedef typename pointer_traits<T>::pointee_type pointee_type; //!< returns 'U' for 'U *', null_type otherwise.
		
		//==================================================================
		// is_pointer_to_member
		//==================================================================
	private:
		template <class U>
		struct pointer_to_memberTraits
		{
			enum { value = false };
		};
		
		template <class U,class V>
		struct pointer_to_memberTraits<U V::*>
		{
			enum { value = true };
		};
		
	public:
		enum { is_pointer_to_member = pointer_to_memberTraits<T>::value /*!< true if 'T' is a pointer to member. */ };
		
		
		
		//==================================================================
		// is_reference
		//==================================================================
	private:
		template <class U>
		struct reference_traits
		{
			enum { value = false };
			typedef U referred_type;
		};
		
		template <class U>
		struct reference_traits<U &>
		{
			enum { value = true };
			typedef U referred_type;
		};
		
	public:
		enum { is_reference = reference_traits<T>::value /*!< true is 'T' can be written as 'U &'. */ };
		typedef typename reference_traits<T>::referred_type                  &reference_type;      //!< returns 'U' for 'U &' and 'U'.
		typedef const typename reference_traits<mutable_type>::referred_type &const_reference_type; //!< returns 'const U'.
		
		
		
	public:
		enum { is_std_integer   = ( tl::index_of<tl::std_sints_list,T>::value >= 0 ) };
		enum { is_std_unsigned  = ( tl::index_of<tl::std_uints_list,T>::value >= 0 ) };
		enum { is_sys_integer   = ( tl::index_of<tl::sys_sints_list,T>::value >= 0 ) };
		enum { is_sys_unsigned  = ( tl::index_of<tl::sys_uints_list,T>::value >= 0 ) };
		enum { is_misc_integer  = ( tl::index_of<tl::misc_ints_list,T>::value >= 0 ) };
		enum { is_integral      = ( is_std_integer || is_std_unsigned || is_sys_integer || is_sys_unsigned || is_misc_integer ) };
		enum { is_std_real      = ( tl::index_of<tl::std_reals_list,T>::value >= 0 ) };
		enum { is_arithmetic    = ( is_integral || is_std_real )                 };
		enum { is_fundamental   = ( is_arithmetic  || is_same_type<T,void>::value) };
		enum { is_primitive     = ( is_fundamental || is_pointer || is_pointer_to_member ) };
		
		
		/** \brief optimized parameter type.
		 
		 - U & \f$\rightarrow\f$ U &
		 - const U & \f$\rightarrow\f$ const U &
		 - U \f$\rightarrow\f$ U is primitive ? U : const U &
		 - const U \f$\rightarrow\f$ U is primitive ? const U : const U &
		 */
		typedef typename select<
		is_reference,
		// if T is a reference (U &, const U &)
		T,
		// else if T is not a reference
		typename select< type_traits<mutable_type>::is_primitive,
		// T is a primitive U or const U: returns T
		T,
		// T is a non primitive U or const U: returns const T &
		const_reference_type>::result
		>::result parameter_type;
		
		
	};
	
}

#endif
