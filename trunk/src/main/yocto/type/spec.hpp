//! \file type-spec.hpp

#ifndef YOCTO_TYPE_SPEC_INCLUDED
#define YOCTO_TYPE_SPEC_INCLUDED 1


#include "yocto/memory/buffer.hpp"
#include <typeinfo>

namespace yocto {
    
	class type_spec : public memory::ro_buffer
    {
    public:
        type_spec() throw();
        type_spec( const std::type_info & ) throw();
        type_spec( const type_spec & ) throw();
        type_spec &operator=( const type_spec & ) throw();
        virtual ~type_spec() throw();
        
        const char *name() const throw();
        bool        before( const type_spec &other ) const throw();
        
        int compare_to( const type_spec &other ) const throw();
        
        virtual size_t length() const throw();
        
        bool match( const std::type_info & ) const throw();
        bool match( const type_spec      & ) const throw();
        
        
        
    private:
        const std::type_info *info_;
        size_t                len_;
        virtual const void *  get_address() const throw();
	};
    
	bool operator==( const type_spec &lhs, const type_spec &rhs ) throw();
	bool operator!=( const type_spec &lhs, const type_spec &rhs ) throw();
	bool operator< ( const type_spec &lhs, const type_spec &rhs ) throw();
	bool operator<=( const type_spec &lhs, const type_spec &rhs ) throw();
	bool operator>=( const type_spec &lhs, const type_spec &rhs ) throw();
	bool operator >( const type_spec &lhs, const type_spec &rhs ) throw();
    
    
}


#endif
