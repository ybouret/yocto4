#ifndef YOCTO_MK_SPARSE_ITEM_INCLUDED
#define YOCTO_MK_SPARSE_ITEM_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace math
    {
        //! extraneous component
        class sp_key
        {
        public:
            const size_t row;
            const size_t col;
            
            explicit sp_key(size_t r,size_t c) throw();
            virtual ~sp_key() throw();
            sp_key( const sp_key &k ) throw();
            
            friend inline bool operator==( const sp_key &lhs, const sp_key &rhs ) throw()
            {
                return lhs.row == rhs.row && lhs.col == rhs.col;
            }
            
            //! dedicated hasher
            class hasher
            {
            public:
                hasher() throw();
                ~hasher() throw();
                size_t operator()(const sp_key &) const throw();
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(hasher);
            };
            
        private:
            YOCTO_DISABLE_ASSIGN(sp_key);
        };
        
        template <typename T>
        class sp_item : public sp_key
        {
        public:
            T            value;
            inline sp_item(size_t r,size_t c,T x) throw() :
            sp_key(r,c),
            value(x)
            {
                assert(row!=col);
            }
            inline virtual ~sp_item() throw() {}
            
            sp_item(const sp_item &other ) throw() :
            sp_key(other),
            value( other.value )
            {
            }
            
            inline const sp_key & key() const throw() { return *this; }
            
        private:
            YOCTO_DISABLE_ASSIGN(sp_item);
        };
        
        
    }
    
}

#endif
