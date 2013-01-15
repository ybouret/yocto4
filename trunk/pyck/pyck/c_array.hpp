#ifndef PYCK_C_ARRAY_INCLUDED
#define PYCK_C_ARRAY_INCLUDED 1

#include "./exception.hpp"

template <typename T>
class C_Array
{
public:
	explicit C_Array( size_t n ) :
	data( static_cast<T*>(operator new( __check(n) * sizeof(T) )) ),
		size(n)
	{
	}

	virtual ~C_Array() throw()
	{
		operator delete(data);
		data          = 0;
		(size_t&)size = 0;
	}

	inline T &       operator[](size_t indx) throw()       { assert(indx<size); return data[indx]; }
	inline const T & operator[](size_t indx) const throw() { assert(indx<size); return data[indx]; }

private:
	C_Array( const C_Array & );
	C_Array&operator=( const C_Array & );
	T *data;
	static inline size_t __check( size_t n)
	{
		if(n<=0) throw Exception("C_Array: invalid size");
		return n;
	}
public:
	const size_t size;


};

#endif
