#include "./lockable.hpp"

Lockable::  Lockable() throw() {}
Lockable:: ~Lockable() throw() {}


ScopedLock:: ScopedLock( Lockable &obj ) throw() :
host_( obj )
{
    host_.lock();
}

ScopedLock:: ~ScopedLock() throw()
{
    host_.unlock();
}
