#ifndef YOCTO_SWAMP_LEVEL_SET_INCLUDED
#define YOCTO_SWAMP_LEVEL_SET_INCLUDED 1

#include "yocto/swamp/types.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        //! a level value, with an auxiliary key
        template <typename T>
        class level
        {
        public:
            typedef int KEY;
            YOCTO_ASSOCIATIVE_KEY_T;
            
            const_key  key;
            const_type value;
            
            level( param_key k, param_type v ) throw();
            level( const level &) throw();
            ~level() throw();
            
	    template <typename U>
            friend bool operator<( const level<U> &lhs_level, const level<U> &rhs_level ) throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(level);
        };
        
        template <typename T>
        class level_set 
        {
        public:
            typedef level<T>                        level_type;
            typedef sorted_vector<level_type>       db_type;
            typedef typename level_type::param_key  param_key;
            typedef typename level_type::param_type param_type;
            
            explicit level_set() throw();
            virtual ~level_set() throw();
            level_set( const level_set &);
            
            void   add( param_key key, param_type value);
            size_t size() const throw();
            const level_type & operator[]( size_t  ) const throw();
            
        private:
            db_type levels;
            YOCTO_DISABLE_ASSIGN(level_set);
        };
        
        
    }
}

#endif
