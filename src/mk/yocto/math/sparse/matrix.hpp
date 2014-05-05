#ifndef YOCTO_MK_SPARSE_MATRIX_INCLUDED
#define YOCTO_MK_SPARSE_MATRIX_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        class sp_item
        {
        public:
            const size_t row;
            const size_t col;
            T            value;
            
            friend inline bool operator==( const sp_item &lhs, const sp_item &rhs ) throw()
            {
                return lhs.row == rhs.row && lhs.col == rhs.col;
            }
            
            inline const sp_item &key() const throw() { return *this; }
            
            class hasher
            {
            public:
                inline  hasher() throw() {}
                inline ~hasher() throw() {}
                size_t operator()( const sp_item &it ) const throw()
                {
                    
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(hasher);
            };
            
        private:
            YOCTO_DISABLE_ASSIGN(sp_item);
        };
        
        template <typename T>
        class spmatrix : public object
        {
        public:
            const size_t size;
            
            explicit spmatrix(size_t n);
            virtual ~spmatrix() throw();
            
        private:
            T     *sa; //!< 1..size diagonal
            size_t na; //!< for memory allocation
            YOCTO_DISABLE_ASSIGN(spmatrix);
            void build();
        };
        
    }
}

#endif
