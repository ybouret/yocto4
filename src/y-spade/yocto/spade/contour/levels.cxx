
namespace yocto
{
    namespace spade
    {
        
        template <>
        level<real_t>:: ~level() throw()
        {
        }
        
        
        template <>
        level<real_t>:: level(  param_key id, param_type args ) :
        value(args),
        key(id)
        {
        }
        
        template <>
        level<real_t>:: level(const level &other ) :
        value( other.value ),
        key( other.key )
        {
            
        }
        
        
        bool operator<( const level<real_t> &lhs_level, const level<real_t> &rhs_level ) throw()
        {
            return lhs_level.value < rhs_level.value;
        }
        
    }
}
