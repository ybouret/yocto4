
namespace yocto 
{
    namespace swamp
    {
        template <> level<ZTYPE>:: ~level() throw() {}
        
        template <> 
        level<ZTYPE>:: level( param_key k, param_type v ) throw() :
        key(k),
        value(v)
        {
        }
        
        template <> 
        level<ZTYPE>:: level( const level &other ) throw() :
        key( other.key ),
        value( other.value )
        {
        }
        
        //template <>
        bool operator<( const level<ZTYPE> &lhs_level, const level<ZTYPE> &rhs_level ) throw()
        {
            return lhs_level.key < rhs_level.key;
        }
        
        template <>
        level_set<ZTYPE>:: level_set() throw() : db_type()
        {
            
        }
        
        template <>
        level_set<ZTYPE>:: ~level_set() throw() {}
        
        template <>
        level_set<ZTYPE>::level_set( const level_set &other ) : db_type( other )
        {
        }
        
        template <>
        void level_set<ZTYPE>:: add( param_key key, param_type value )
        {
            const level_type l(key,value);
            if( ! insert(l) )
            {
                throw exception("level_set( multiple key <%d> )", key ); 
            }
        }
        
    }
}