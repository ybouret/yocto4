#ifndef YOCTO_FAME_ARRAY3D_INCLUDED
#define YOCTO_FAME_ARRAY3D_INCLUDED 1

#include "yocto/fame/array2d.hpp"

namespace yocto
{
    
    namespace fame
    {
        //______________________________________________________________________
        //
        //! three dimensional array
        //______________________________________________________________________

        template <typename T>
        class array3D : public linear<T,layout3D>
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            YOCTO_ARGUMENTS_DECL_T;
            typedef layout3D              layout_type;
            typedef linear<T,layout_type> linear_type;
            typedef array3D<T>            varray_type;
            typedef array2D<T>            slice_type;
            
            //__________________________________________________________________
            //
            // public data
            //__________________________________________________________________
            const coord2D   slice_lower;
            const coord2D   slice_upper;
            const layout2D  slice_layout;
            
            
            //! ctor
            inline explicit array3D(const string      &user_name,
                                    const layout_type  L) :
            linear_type(user_name,L,0),
            slice_lower(this->lower.x,this->lower.y),
            slice_upper(this->upper.x,this->upper.y),
            slice_layout(slice_lower,slice_upper),
            nslice(0),
            slices(0)
            {
                const size_t max_slices = this->width.z;
                nslice  = max_slices;
                slices  = memory::kind<memory_kind>::acquire_as<slice_type>(nslice);
                
                type  *p      = this->entry;
                size_t islice = 0;
                try
                {
                    while(islice<max_slices)
                    {
                        const string id = this->make_id(islice+this->lower.z);
                        new (slices+islice) slice_type(id,slice_layout,p);
                        ++islice;
                        p += slice_layout.items;
                    }
                    slices -= this->lower.z;
                }
                catch(...)
                {
                    while(islice>0)
                    {
                        slices[--islice].~slice_type();
                    }
                    memory::kind<memory_kind>::release_as<slice_type>(slices, nslice);
                    throw;
                }
            }
            
            
            //! dtor
            inline virtual ~array3D() throw()
            {
                slices += this->lower.z;
                size_t islice = this->width.z;
                while(islice>0)
                {
                    slices[--islice].~slice_type();
                }
                memory::kind<memory_kind>::release_as<slice_type>(slices, nslice);
            }
            
            inline slice_type & operator[](unit_t k) throw()
            {
                assert(k>=this->lower.z);
                assert(k<=this->upper.z);
                return slices[k];
            }
            
            inline const slice_type & operator[](unit_t k) const throw()
            {
                assert(k>=this->lower.z);
                assert(k<=this->upper.z);
                return slices[k];
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array3D);
            virtual void                 *get_handle() const throw() { return (void*)this; }
            virtual const std::type_info &get_typeid() const throw() { return typeid(varray_type); }
            size_t      nslice;
            slice_type *slices;
        };
        
        template <typename T> struct array_for<3,T> { typedef array3D<T> type; };

    }
    
}


#endif
