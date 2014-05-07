#ifndef YOCTO_MK_SPARSE_MATRIX_INCLUDED
#define YOCTO_MK_SPARSE_MATRIX_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/math/sparse/item.hpp"
#include "yocto/associative/lexicon.hpp"

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
            typedef lexicon<sp_key,item_type,sp_key::hasher,allocator> dok;
            
            const size_t rows;
            const size_t cols;
            const size_t nmax;
            
            explicit spmatrix() throw();
            explicit spmatrix(size_t nr,size_t nc);
            virtual ~spmatrix() throw();
            
            T & operator()(size_t i,size_t j);
            T   operator()(size_t i,size_t j) const throw();
            
            void   output( std::ostream & ) const;
            
            friend inline std::ostream & operator<<( std::ostream &os, const spmatrix &m )
            {
                m.output(os);
                return os;
            }
            
            void ldz() throw();
            
            void ensure(size_t num_items);
            const dok & get_items() const throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(spmatrix);
            dok items;
        };
        
    }
}

#endif
