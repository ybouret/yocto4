#ifndef YOCTO_MK_SPARSE_MATRIX_INCLUDED
#define YOCTO_MK_SPARSE_MATRIX_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/math/sparse/item.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace math
    {
        
        
             
        template <typename T>
        class spmatrix : public object
        {
        public:
            typedef memory::global::allocator allocator;
            typedef sp_item<T>                item_type;
            
            const size_t       size;
            
            explicit spmatrix() throw();
            explicit spmatrix(size_t n);
            virtual ~spmatrix() throw();
            
            T & operator()(size_t i,size_t j);
            T   operator()(size_t i,size_t j) const throw();
            
            size_t extras() const throw();
            void   output( std::ostream & ) const;
            
            friend inline std::ostream & operator<<( std::ostream &os, const spmatrix &m )
            {
                m.output(os);
                return os;
            }
            
            void ldz() throw();
            void ld1() throw(); 
            void transpose();
            
        private:
            YOCTO_DISABLE_ASSIGN(spmatrix);
            typedef set<sp_key,item_type,sp_key::hasher,allocator> item_set;
            T       *sa; //!< 1..size diagonal
            size_t   na; //!< for memory allocation
            item_set items;
            void build();
        };
        
    }
}

#endif
