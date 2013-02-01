#ifndef PYCK_C_ARRAY_INCLUDED
#define PYCK_C_ARRAY_INCLUDED 1

#include "./exception.hpp"
#include <iostream>

template <typename T>
class C_Array
{
public:
    
    explicit C_Array() throw() : data(0), size(0) {}
    
	explicit C_Array( size_t n ) :
	data( static_cast<T*>(operator new( __check(n) * sizeof(T) )) ),
    size(n)
	{
	}
    
	virtual ~C_Array() throw()
	{
        if(data)
        {
            assert(size);
            operator delete(data);
            data          = 0;
            (size_t&)size = 0;
        }
	}
    
    inline void swap_with( C_Array &other ) throw()
    {
        cswap(data,other.data);
        cswap_const(size,other.size);
    }
    
    inline void make( size_t n )
    {
        if( n != size )
        {
            C_Array   tmp(n);
            swap_with(tmp);
        }
    }
    
    inline void ensure( size_t n )
    {
        if( n > size )
        {
            C_Array   tmp(n);
            swap_with(tmp);
        }
    }
    
	inline T &       operator[](size_t indx) throw()       { assert(indx<size); return data[indx]; }
	inline const T & operator[](size_t indx) const throw() { assert(indx<size); return data[indx]; }
    
    //! scilab/octave output
    friend inline std::ostream & operator<<( std::ostream &os, const C_Array &arr)
    {
        os << "[";
        for( size_t i=0; i < arr.size; ++i ) os << " " << arr[i];
        os << "]'";
        return os;
    }
    
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
