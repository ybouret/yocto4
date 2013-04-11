#ifndef YOCTO_MATH_ITRIANGLE_INCLUDED
#define YOCTO_MATH_ITRIANGLE_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/code/static-check.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
 
    namespace math
    {
        
        class iEdge
        {
        public:
            iEdge(size_t a, size_t b) throw(); //!< a != b
            iEdge(size_t a, size_t b, void *) throw(); //!< a < b
            iEdge(const iEdge &other) throw();
            ~iEdge() throw();
            
            const size_t i1, i2; //!< i1 < i2
    
            friend bool operator==( const iEdge &lhs, const iEdge &rhs) throw();
            friend bool operator!=( const iEdge &lhs, const iEdge &rhs) throw();
            
            typedef vector<iEdge> Buffer;
            
        private:
            YOCTO_DISABLE_ASSIGN(iEdge);
        };
        
        class iTriangle : public object
        {
        public:
            explicit iTriangle(size_t a, size_t b, size_t c) throw(); //!< a != b != c
            virtual ~iTriangle() throw();
            
            const size_t i1, i2, i3; //!< i1 < i2 < i3
            iTriangle *next;
            iTriangle *prev;
            
            void insert_into( iEdge::Buffer &edges ) const;
            
            template <typename T,size_t I>
            inline T & get() throw()
            {
                YOCTO_STATIC_CHECK(I<2,invalid_index);
                return *(T *)&wksp[I];
            }
            
            template <typename T,size_t I>
            inline const T & get() const throw()
            {
                YOCTO_STATIC_CHECK(I<2,invalid_index);
                return *(const T *)&wksp[I];
            }
            
            class List : public core::list_of<iTriangle>
            {
            public:
                explicit List() throw();
                virtual ~List() throw();
                void kill() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(List);
            };
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(iTriangle);
            uint64_t wksp[2];
        };
        
        
        
    }
    
}

#endif
