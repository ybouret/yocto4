#ifndef YOCTO_SPADE_CONTOUR_LEVELS_INCLUDED
#define YOCTO_SPADE_CONTOUR_LEVELS_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        
        template <typename T>
        class level
        {
        public:
            typedef int KEY;
            YOCTO_ASSOCIATIVE_KEY_T;
            
            level(param_key id, param_type args);
            level(const level &);
            ~level() throw();
            
            const_type value;
            const_key  key;
            
            template <typename U> friend
            bool operator<( const level<U> &lhs_level, const level<U> &rhs_level ) throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(level);
            
        };
        
        template <typename T>
        class levels
        {
        public:
            typedef level<T>                    level_t;
            typedef sorted_vector< level_t >    db_type;
            
            explicit levels() throw();
            virtual ~levels() throw();
            
            const db_type & operator()(void) const throw();
            
            void append(  
            
        private:
            db_type db;
            YOCTO_DISABLE_COPY_AND_ASSIGN(levels);
            
        };
        
    }
    
}


#endif
