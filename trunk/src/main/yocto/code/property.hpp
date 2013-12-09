#ifndef YOCTO_PROPERTY_INCLUDED
#define YOCTO_PROPERTY_INCLUDED 1

#include "yocto/type/args.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
	
	class ppty
    {
    public:
        virtual ~ppty() throw();
        
        bool is_writable() const throw();
        
        virtual const char *name() const throw();
        
    protected:
        explicit ppty( bool rw ) throw();
        ppty( const ppty & ) throw();
        
        void set_writable( bool ) throw();
        
    private:
        YOCTO_DISABLE_ASSIGN( ppty );
        bool writable_;
    };
	
	template <typename T>
	class property : public ppty
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        
        //! destructor, nothing to do
        virtual ~property() throw() {}
        
        
        //! should be use withing classes only
        inline   property( T &value, bool rw ) : ppty( rw ), value_( value )
        {
        }
        
        inline operator  const_type & () const throw() { return get(); }
        
        inline property & operator=( param_type args )
        {
            if( this->is_writable() )
            {
                set( args );
            }
            else
            {
                throw exception("[property] '%s' is read-only", this->name() );
            }
            return *this;
        }
        
        
    private:
        virtual const_type & get() const throw()    { return value_;  }
        virtual void         set( param_type args ) { value_ = args;  }
        YOCTO_DISABLE_COPY_AND_ASSIGN(property);
        T &value_;
        
    };
	
}

#endif
