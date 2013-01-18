#ifndef PYCK_TEAM_INCLUDED
#define PYCK_TEAM_INCLUDED 1

#include "./runnable.hpp"
#include "./exception.hpp"

#include <cstring>

//! balance jobs
static inline
void TeamBalance(size_t        workload,
                 size_t       &offset,
                 size_t       &length,
                 const size_t &rank,
                 const size_t &size
                 ) throw()
{
    assert(rank<size);
    offset = 0;
    length = 0;
    for( size_t i=0; i <= rank; ++i )
    {
        offset   += length;
        length    = workload/(size-i);
        workload -= length;
    }
}


template <typename RUNNABLE>
class Team
{
public:
	Mutex mutex; //!< sharable mutex
    
	explicit Team(size_t n) :
	mutex(),
    task( static_cast<RUNNABLE *>(operator new( __check(n) * sizeof(RUNNABLE) )) ),
    maxi( n ),
    size( 0 )
	{
	}
    
	virtual ~Team() throw()
	{
		finish();
		assert(0==size);
		operator delete( task );
		(size_t&)maxi = 0;
	}
    
	inline void finish() throw()
	{
		size_t &n = (size_t&)size;
		while(n>0)
		{
			RUNNABLE *t = &task[--n];
			t->join();
			t->~RUNNABLE();
			clear(t);
		}
	}
    
	template <typename T>
	inline void enqueue( const T &args )
	{
		assert(size<maxi);
		RUNNABLE *t = &task[size];
		clear(t);
		new (t) RUNNABLE(mutex,args);
		t->start();
		++((size_t&)size);
	}
    
    
    template <typename T, typename U>
	inline void enqueue( const T &arg1, const U &arg2 )
	{
		assert(size<maxi);
		RUNNABLE *t = &task[size];
		clear(t);
		new (t) RUNNABLE(mutex,arg1,arg2);
		t->start();
		++((size_t&)size);
	}
    
    
private:
	static inline size_t __check(size_t n )
	{
		if(n<=0) throw Exception("Team: invalid maxi");
		return n;
	}
    
	static inline void clear(RUNNABLE *t) throw()
	{
		assert(t);
		memset((void*)t,0,sizeof(RUNNABLE));
	}
    
	RUNNABLE     *task;
    
public:
	const size_t  maxi;
	const size_t  size;
    
	Team(const Team &);
	Team &operator=( const Team & );
};


#endif
