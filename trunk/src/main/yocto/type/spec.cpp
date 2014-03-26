#include "yocto/type/spec.hpp"
#include <cstring>

namespace yocto {
	
	
	
	type_spec:: type_spec() throw() :
	info_(NULL),
	len_(0)
	{
		class nil {};
		info_ = &typeid(nil);
		assert(info_);
		len_ = strlen( info_->name() );
	}
	
	type_spec:: type_spec( const std::type_info &info ) throw() :
	info_( &info ),
	len_(  strlen( info_->name()) )
	{
		assert( info_ != NULL );
	}
	
	type_spec:: ~type_spec() throw() {
		assert( info_ != NULL );
	}
	
	type_spec::type_spec( const type_spec &other ) throw() :
	info_( other.info_ ),
	len_( other.len_ )
	{
		assert( other.info_ != NULL );
		assert( this->info_ != NULL );
	}
	
	type_spec &type_spec::operator=( const type_spec &other ) throw() {
		assert( other.info_ != NULL );
		info_ = other.info_;
		len_  = other.len_;
		return *this;
	}
	
	const char *type_spec:: name() const throw() {
		assert( info_ != NULL );
		return info_->name();
	}
	
#if defined(_MSC_VER)
	//! int to bool
#pragma warning( disable : 4800 )
#endif
	bool type_spec:: before( const type_spec &other ) const throw() {
		assert( other.info_ != NULL );
		assert( this->info_ != NULL );
		return info_->before( *(other.info_) );
	}
	
	int type_spec:: compare_to( const type_spec &other ) const throw() {
		assert( other.info_ != NULL );
		assert( this->info_ != NULL );
		const std::type_info &lhs = *(this->info_);
		const std::type_info &rhs = *(other.info_);
		
		if( lhs == rhs ) {
			return 0;
		} else {
			return lhs.before(rhs) ? -1 : 1;
		}
	}
	
    bool type_spec:: match( const std::type_info &tid ) const throw()
    {
        return *(this->info_) == tid;
    }
    
    bool type_spec:: match( const type_spec &other ) const throw()
    {
        return *(this->info_) == *(other.info_);
    }
    
	const void * type_spec:: get_address() const throw()
	{
		assert( info_ != NULL );
		return (void *)( info_->name() );
	}
	
	size_t type_spec:: length() const throw()
	{
		return len_;
	}
	
	
#define YOCTO_TYPE_INFO_IMPL(OP) \
bool operator OP ( const type_spec &lhs, const type_spec &rhs ) throw() \
{ return lhs.compare_to( rhs ) OP 0 ; }
	
	YOCTO_TYPE_INFO_IMPL(==)
	YOCTO_TYPE_INFO_IMPL(!=)
	YOCTO_TYPE_INFO_IMPL(<=)
	YOCTO_TYPE_INFO_IMPL(>=)
	YOCTO_TYPE_INFO_IMPL(<)
	YOCTO_TYPE_INFO_IMPL(>)
	
	
	
	
}
