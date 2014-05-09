#ifndef YOCTO_MK_SPARSE_MATRIX_INCLUDED
#define YOCTO_MK_SPARSE_MATRIX_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/math/sparse/item.hpp"
#include "yocto/associative/lexicon.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {
        
        
        template <typename T>
        class sp_matrix : public object
        {
        public:
            typedef memory::global::allocator   allocator;
            typedef sp_item<T>                  item_type;
            typedef typename sp_item<T>::hasher sp_hasher;
            typedef lexicon<sp_key,item_type,sp_hasher,allocator> dok;
            typedef typename real_of<T>::type real_type;
            
            const size_t rows;
            const size_t cols;
            const size_t nmax;
            
            explicit sp_matrix() throw();
            explicit sp_matrix(size_t nr,size_t nc);
            virtual ~sp_matrix() throw();
            
            T & operator()(size_t i,size_t j);
            T   operator()(size_t i,size_t j) const throw();
            
            void   output( std::ostream & ) const;
            
            friend inline std::ostream & operator<<( std::ostream &os, const sp_matrix &m )
            {
                m.output(os);
                return os;
            }
            
            void ldz() throw();
            
            void ensure(size_t n);
            void cleanup( real_type threshold ) throw();
            
            inline size_t            count() const throw()                { return items.size(); }
            inline item_type       & operator[](size_t idx) throw()       { return items[idx];   }
            inline const item_type & operator[](size_t idx) const throw() { return items[idx];   }
            
        private:
            YOCTO_DISABLE_ASSIGN(sp_matrix);
            dok items;
        };
        
    }
}

#endif
