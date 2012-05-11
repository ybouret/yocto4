
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
        
        template <>
        bool operator<( const level<ZTYPE> &lhs_level, const level<ZTYPE> &rhs_level ) throw()
        {
            return lhs_level.value < rhs_level.value;
        }
        
        template <>
        level_set<ZTYPE>:: level_set() throw() : 
        levels()
        {
            
        }
        
        template <>
        level_set<ZTYPE>:: ~level_set() throw() {}
        
        template <>
        level_set<ZTYPE>::level_set( const level_set &other ) : 
        levels( other.levels )
        {
        }
        
        template <>
        void level_set<ZTYPE>:: add( param_key key, param_type value )
        {
            for( size_t i=1; i <= levels.size(); ++i )
            {
                if( levels[i].key == key )
                    throw exception( "level_set( multiple key <%d> )", key  );
            }
            
            const level_type l(key,value);
            if( ! levels.insert(l) )
            {
                throw exception("level_set( multiple value <%g> )", value ); 
            }
        }
        
        template <>
        size_t level_set<ZTYPE>:: size() const throw() { return levels.size(); }
        
        template <>
        const level<ZTYPE> & level_set<ZTYPE>:: operator[]( size_t i ) const throw()
        {
            assert(i>0);
            assert(i<=levels.size());
            return levels[i];
        }
        
        
    }
}
