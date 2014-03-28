#ifndef YOCTO_FOVEA_ARRAY2D_INCLUDED
#define YOCTO_FOVEA_ARRAY2D_INCLUDED 1

#include "yocto/fovea/array1d.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T>
        class array2D : public linear<T,layout2D>
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            YOCTO_ARGUMENTS_DECL_T;
            typedef layout2D              layout_type;
            typedef linear<T,layout_type> linear_type;
            typedef array2D<T>            varray_type;
            typedef array1D<T>            row_type;
            
            //__________________________________________________________________
            //
            // public data
            //__________________________________________________________________
            const layout1D  row_layout;
            
            //__________________________________________________________________
            //
            // public API
            //__________________________________________________________________
            
            //! ctor
            inline explicit array2D(const string      &user_name,
                                    const layout_type L,
                                    void *            user_data=0) :
            linear_type(user_name,L,user_data),
            row_layout(this->lower.x,this->upper.x),
            nrow(0),
            rows(0)
            {
                const size_t  max_row = this->width.y;
                nrow        = max_row;
                rows        = memory::kind<memory_kind>::acquire_as<row_type>(nrow);
                type  *p    = this->entry; assert(p!=0);
                size_t irow = 0;
                try
                {
                    while(irow<max_row)
                    {
                        const string id = this->make_id(irow+this->lower.y);
                        new (rows+irow) row_type(id,row_layout,p);
                        ++irow;
                        p += row_layout.width;
                    }
                    rows -= this->lower.y;
                }
                catch(...)
                {
                    while(irow>0)
                    {
                        rows[--irow].~row_type();
                    }
                    memory::kind<memory_kind>::release_as<row_type>(rows,nrow);
                    throw;
                }
                
            }
            
            //! dtor
            inline virtual ~array2D() throw()
            {
                rows += this->lower.y;
                size_t irow = this->width.y;
                while(irow>0)
                {
                    rows[--irow].~row_type();
                }
                memory::kind<memory_kind>::release_as<row_type>(rows,nrow);
            }
            
            inline row_type & operator[](unit_t y) throw()
            {
                assert(y>=this->lower.y);
                assert(y<=this->upper.y);
                return rows[y];
            }
            
            inline const row_type & operator[](unit_t y) const throw()
            {
                assert(y>=this->lower.y);
                assert(y<=this->upper.y);
                return rows[y];
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array2D);
            virtual void                 *get_handle() const throw() { return (void*)this; }
            virtual const std::type_info &get_typeid() const throw() { return typeid(varray_type); }
            size_t    nrow;
            row_type *rows;
        };
        
        template <typename T> struct array_for<2,T> { typedef array2D<T> type; };

    }
}
#endif
