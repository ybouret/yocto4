#ifndef PYCK_RUNNABLE_INCLUDED
#define PYCK_RUNNABLE_INCLUDED 1

#include "./thread.hpp"

class Runnable
{
public:
    Mutex &mutex;
    virtual ~Runnable() throw();
    
    virtual void run() throw() = 0; //!< the executed method
    
    void start();
    void join() throw();
    
    
protected:
    explicit Runnable( Mutex &shared_mutex) throw();
    
private:
    Runnable( const Runnable & );
    Runnable&operator=( const Runnable & );
    static void Execute(void*) throw();
    
    Thread  *thr;
    uint64_t arr[ PYCK_U64_FOR_ITEM(Thread) ];
    
    void clear() throw();
};


#endif
