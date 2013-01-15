#ifndef PYCK_RAND32_INCLUDED
#define PYCK_RAND32_INCLUDED 1

#include "./os.hpp"

//! Marsaglia's 32 bits random number generators
class Rand32
{
public:
	uint32_t z,w,jsr,jcong,a,b;
	uint32_t x,y,bro;
	uint8_t  c;
	uint32_t t[256];

	explicit Rand32() throw();
	virtual ~Rand32() throw();

	typedef uint32_t (Rand32::*Generator)();
	uint32_t mwc()   throw();
	uint32_t shr3()  throw();
	uint32_t cong()  throw();
	uint32_t fib()   throw();
	uint32_t kiss()  throw();
	uint32_t lfib4() throw();
	uint32_t swb()   throw();

	void settable( uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4, uint32_t i5, uint32_t i6 ) throw();
	void reset( uint32_t s ) throw();

	//! make a random 0 or 1
	template <typename T>
	static inline T ToBit( uint32_t u ) throw()
	{
		static const uint32_t threshold = uint32_t(1) << 31;
		static const T __zero(0);
		static const T __one(1);
		return u >= threshold ? __one : __zero;
	}


	//! convert to 0:1 exclusive
	static inline double ToDouble( uint32_t u ) throw()
	{
		return (0.5+double(u))/4294967296.0;
	}

	//! convert to 0:1 exclusive
	static inline float ToFloat( uint32_t u ) throw()
	{
		return (0.5f+float(u))/4294967296.0f;
	}

	//! should print 7 '0'
	void test() throw();

private:
	Rand32( const Rand32 & );
	Rand32&operator=(const Rand32 &);
};


//! interface to a 32 bits uniform generator
class UniformGenerator
{
public:
	virtual ~UniformGenerator() throw();

	virtual uint32_t next() throw() = 0;
	virtual void     seed(uint32_t) throw() = 0;

	template <typename T> T get() throw(); //!< valid for uin32_t, float, double

	inline double alea() throw()
	{
		return Rand32::ToDouble(next());
	}

	template <typename T>
	inline T full() throw()
	{
		return __full<T>( int2type<(sizeof(T)>sizeof(uint32_t))>() );
	}

	//! return random index in 0..n
	inline size_t leq( size_t n ) throw()
	{
		return full<size_t>() % (++n);
	}

	//! return random index in 0..n-1
	inline size_t lt( size_t n ) throw()
	{
		assert(n>0);
		return full<size_t>() % n;
	}


	//! card desk shuffling algorithm
	template <typename T>
	inline void shuffle( T *a, size_t n ) throw()
	{
		assert(!(NULL==a&&n>0));
		if( n > 1 )
		{
			for( size_t i=n-1;i>0;--i)
			{
				const size_t j   = leq(i); assert(j<=i);
				const T tmp(a[i]);
				a[i] = a[j];
				a[j] = tmp;
			}
		}
	}

	void fill_array( double a, double b, double *x, size_t n ) throw();

protected:
	explicit UniformGenerator() throw();

private:
	UniformGenerator(const UniformGenerator &);
	UniformGenerator&operator=(const UniformGenerator &);
	template <typename T>
	inline T __full( int2type<false> ) throw()
	{
		assert(sizeof(T)<=sizeof(uint32_t));
		return T(next());
	}

	template <typename T>
	inline T __full( int2type<true> ) throw()
	{
		assert(sizeof(T)>sizeof(uint32_t));
		T ans(0);
		for( size_t i=sizeof(T)/sizeof(uint32_t);i>0;--i)
		{
			ans <<= 32;
			ans |=  T(next());
		}
		return ans;
	}

};

//! generic random number generator
template <Rand32::Generator G>
class UniformGeneratorOf : public UniformGenerator
{
public:
	explicit UniformGeneratorOf() throw() : UniformGenerator(), r() {}
	virtual ~UniformGeneratorOf() throw() {}
	virtual uint32_t next() throw() { return (r.*G)(); }
	virtual void     seed(uint32_t s) throw() { r.reset(s); }

private:
	Rand32 r;
	UniformGeneratorOf(const UniformGeneratorOf & );
	UniformGeneratorOf&operator=(const UniformGeneratorOf &);
};

//==========================================================================
// available random number generators
//==========================================================================
struct rand32
{
	typedef UniformGeneratorOf<&Rand32::mwc>   mwc;
	typedef UniformGeneratorOf<&Rand32::shr3>  shr3;
	typedef UniformGeneratorOf<&Rand32::cong>  cong;
	typedef UniformGeneratorOf<&Rand32::fib>   fib;
	typedef UniformGeneratorOf<&Rand32::kiss>  kiss;
	typedef UniformGeneratorOf<&Rand32::lfib4> lfib4;
	typedef UniformGeneratorOf<&Rand32::swb>   swb;
};

typedef rand32::kiss DefaultUniformGenerator;

#endif
