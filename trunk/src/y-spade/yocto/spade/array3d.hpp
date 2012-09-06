#ifndef YOCTO_SPADE_ARRAY3D_INCLUDED
#define YOCTO_SPADE_ARRAY3D_INCLUDED 1

#include "yocto/spade/array2d.hpp"
#include "yocto/spade/in3d.hpp"

namespace yocto
{
    namespace spade
    {
        
        template <typename T>
        class array3D : public linear_of<T,layout3D>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef array3D<T>                     array_type;
            typedef linear_of<T,layout3D>          linear_type;
            typedef layout3D                       layout_type;
            typedef array2D<T>                     slice;
            
            const layout2D slice_layout;
            
            explicit array3D( const layout_type &L ) :
            linear_type( L ),
            slice_layout( coord2D( this->lower.x, this->lower.y), coord2D( this->upper.x, this->upper.y)),
            mslice( this->width.z ),
            nslice(0),
            slices( memory_acquire<slice>(mslice) - this->lower.z )
            {
                try
                {
                    for( unit_t k=this->lower.z; k <= this->upper.z;++k)
                    {
                        new (slices+k) slice(slice_layout);
                        ++nslice;
                    }
                    assert(size_t(this->width.z) == nslice );
                }
                catch(...)
                {
                    kill();
                    throw;
                }
            }
            
            virtual ~array3D() throw()
            {
                kill();
            }
            
            virtual void link( void *data ) throw()
            {
                T *p = ( this->entry = static_cast<T*>(data) );
                for( unit_t k=this->lower.z; k <= this->upper.z;++k, p += slice_layout.items )
                {
                    slices[k].link(p);
                }
            }
            
            inline slice & operator[]( unit_t z ) throw()
            { assert(this->entry); assert(z>=this->lower.z);assert(z<=this->upper.z); return slices[z]; }
            inline const slice & operator[]( unit_t z ) const throw()
            { assert(this->entry); assert(z>=this->lower.z);assert(z<=this->upper.z); return slices[z]; }
            
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

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array3D);
            size_t mslice; //!< memory
            size_t nslice; //!< width.z
            slice *slices; //!< memory for slices, offseted
            
            //! kill current nslice and free memory
            inline void kill() throw()
            {
                slices += this->lower.z;
                while( nslice>0 )
                {
                    slices[--nslice].~slice();
                }
                memory_release(slices,mslice);
            }
            
        };
        
    }
}


#endif
