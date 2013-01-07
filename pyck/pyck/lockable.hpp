#ifndef PYCK_LOCKABLE_INCLUDED
#define PYCK_LOCKABLE_INCLUDED 1


class Lockable
{
public:
    explicit Lockable() throw();
    virtual ~Lockable() throw();
    
    virtual void lock() throw() = 0;
    virtual void unlock() throw() = 0;
    
private:
    Lockable( const Lockable &);
    Lockable & operator=( const Lockable &);
};

class ScopedLock
{
public:
    ScopedLock(Lockable &) throw();
    ~ScopedLock() throw();
    
private:
    Lockable &host_;
    ScopedLock(const ScopedLock &);
    ScopedLock &operator=( const ScopedLock & );
};


#endif
