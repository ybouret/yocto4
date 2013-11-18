//! \file
#ifndef YOCTO_SPADE_ARRAY1D_INCLUDED
#define YOCTO_SPADE_ARRAY1D_INCLUDED 1

#include "yocto/spade/in1d.hpp"
#include "yocto/spade/linear.hpp"

namespace yocto
{
    namespace spade
    {
        
        template <typename T>
        class array1D : public linear_of<T,layout1D>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef array1D<T>                     array_type;
            typedef linear_of<T,layout1D>          linear_type;
            typedef layout1D                       layout_type;
            
            explicit array1D( const layout_type &L ) throw() :
            linear_type(L), item(0) {}
            
            virtual ~array1D() throw() {}
            
            inline type       & operator[](unit_t x) throw()       { assert(x>=this->lower);assert(x<=this->upper); return item[x]; }
            inline const_type & operator[](unit_t x) const throw() { assert(x>=this->lower);assert(x<=this->upper); return item[x]; }
            
            virtual void link( void *data ) throw()
            {
                assert(data);
                this->entry = static_cast<T*>(data);
                item        = this->entry - this->lower;
            }
            
            inline static void *ctor( const layout_type &L, linear **handle )
            {
                assert(handle);
                array_type  *arr = new array_type(L);
                *handle   = arr;
                return arr;
            }
            
            inline static void dtor( void *p ) throw()
            {
                assert(p);
                array_type *arr = static_cast<array_type *>(p);
                delete arr;
            }
            
            inline void set( const array_type &other, const layout_type &sub ) throw()
            {
                assert(this->contains(sub));
                assert(other.contains(sub));
                array_type &self = *this;
                for( unit_t i=sub.lower;i<=sub.upper;++i)
                {
                    self[i] = other[i];
                }
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array1D);
            T *item; //!< offseted item
        };
        
        
    }
}

#endif
