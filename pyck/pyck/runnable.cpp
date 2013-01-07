#include "./runnable.hpp"
#include <cstring>
#include <new>

void Runnable:: clear() throw()
{
    thr = 0;
    memset( arr, 0 , sizeof(arr) );
}


Runnable:: ~Runnable() throw()
{
}


Runnable:: Runnable( Mutex &m) throw() :
mutex(m),
thr(0),
arr()
{
    clear();
}

void Runnable:: Execute(void *args ) throw()
{
    assert(args);
    Runnable *self = static_cast<Runnable *>(args);
    self->run();
}

void Runnable:: join() throw()
{
    assert(thr);
    thr->join();
    clear();
    
}

void Runnable:: start()
{
    assert(!thr);
    thr = new( &arr[0] ) Thread( Runnable::Execute, this );
}
