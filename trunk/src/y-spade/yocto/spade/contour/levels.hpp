#ifndef YOCTO_SPADE_CONTOUR_LEVELS_INCLUDED
#define YOCTO_SPADE_CONTOUR_LEVELS_INCLUDED 1

#include "yocto/associative/set.hpp"
#include "yocto/intrusive-ptr.hpp"

namespace yocto
{
    
    namespace spade
    {
        
               
        template <typename KEY,typename T>
        class level : public counted
        {
        public:
            YOCTO_ASSOCIATIVE_KEY_T;
            
            const_key  id;
            type       value;
            
            inline explicit level(param_key k, param_type args) : id(k), value(args) {}
            inline virtual ~level() throw() {}
            
            inline const_key & key() const throw() { return id; }
            
            typedef intrusive_ptr<KEY, level<KEY,T> > ptr;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(level);
        };
        
        template <typename KEY, typename T>
        class levels : public set< KEY,typename level<KEY,T>::ptr>
        {
        public:
            YOCTO_ASSOCIATIVE_KEY_T;
            
            typedef level<KEY,T>          level_t;
            typedef typename level_t::ptr level_ptr;
            typedef set<KEY,level_t>      db_type;
            
            explicit levels() throw() : db_type() {}
            virtual ~levels() throw() {}
            
            inline type & operator[]( param_key id )
            {
                db_type   &self = *this;
                level_ptr *ppL  = self.search(id);
                if( ppL )
                {
                    return (**ppL).value;
                }
                else
                {
                    const_type val(0);
                    level_ptr p( new level_t(id,val) );
                    if(!self.insert(p))
                    {
                        throw exception("");
                    }
                    return p->value;
                }
            }
            
            inline const_type & operator[](param_key id) const
            {
                const db_type   &self = *this;
                const level_ptr *ppL  = self.search(id);
                if(!ppL)
                    throw exception("");
                return (**ppL).value;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(levels);
        };
        
        
        
        
    }
    
}


#endif
