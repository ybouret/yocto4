#ifndef YOCTO_SPADE_ARRAY2D_INCLUDED
#define YOCTO_SPADE_ARRAY2D_INCLUDED 1

#include "yocto/spade/array1d.hpp"
#include "yocto/spade/in2d.hpp"
#include "yocto/spade/memory.hpp"

namespace yocto
{
    namespace spade
    {
        
        template <typename T>
        class array2D : public linear_of<T,layout2D>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef array2D<T>                     array_type;
            typedef linear_of<T,layout2D>          linear_type;
            typedef layout2D                       layout_type;
            typedef array1D<T>                     row;

            const layout1D   row_layout;
            
            explicit array2D( const layout_type &L ) throw() :
            linear_type(L),
            row_layout( this->lower.x, this->upper.x ),
            mrow(this->width.y),
            rows(memory_acquire<row>(mrow) - this->lower.y )
            {
                for( unit_t i= this->upper.y;i>=this->lower.y;--i)
                    new (rows+i) row(row_layout); //!< no-throw
            }
            
            virtual ~array2D() throw()
            {
                for( unit_t i=this->upper.y;i>=this->lower.y;--i)
                    rows[i].~row();
                rows += this->lower.y;
                memory_release(rows, mrow);
            }

            virtual void link( void *data ) throw()
            {
                assert(data);
                T *p = (this->entry = static_cast<T*>(data));
                for( unit_t i=this->lower.y;i<=this->upper.y;++i, p += row_layout.items)
                {
                    rows[i].link(p);
                }
            }
            
            inline row        & operator[](unit_t y) throw()       { assert(y>=this->lower.y);assert(y<=this->upper.y); return rows[y]; }
            inline const row  & operator[](unit_t y) const throw() { assert(y>=this->lower.y);assert(y<=this->upper.y); return rows[y]; }

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array2D);
            size_t    mrow;  //!< for allocation
            row      *rows;  //!< offseted rows
        };
        
    }
}

#endif
