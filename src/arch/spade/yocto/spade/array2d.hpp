//! \file

#ifndef YOCTO_SPADE_ARRAY2D_INCLUDED
#define YOCTO_SPADE_ARRAY2D_INCLUDED 1

#include "yocto/spade/array1d.hpp"
#include "yocto/spade/in2d.hpp"
#include "yocto/spade/memory.hpp"

//#include "yocto/gfx/ramp/types.hpp"
#include "yocto/ios/ocstream.hpp"

#if defined(_MSC_VER)
// this in constructor
#pragma warning ( disable : 4355 )
#endif

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
            
            inline row        & operator[](unit_t y) throw()
            { assert(this->entry); assert(y>=this->lower.y);assert(y<=this->upper.y); return rows[y]; }
            inline const row  & operator[](unit_t y) const throw()
            { assert(this->entry); assert(y>=this->lower.y);assert(y<=this->upper.y); return rows[y]; }
            
            inline type & operator[]( const coord2D &u ) throw()
            {
                return (*this)[u.y][u.x];
            }
            
            inline const_type & operator[]( const coord2D &u ) const throw()
            {
                return (*this)[u.y][u.x];
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
                for( unit_t j=sub.lower.y;j<=sub.upper.y;++j)
                {
                    row       &target = self[j];
                    const row &source = other[j];
                    for( unit_t i=sub.lower.x;i<=sub.upper.x;++i)
                        target[i] = source[i];
                }
            }
            
#if 0
            //! save a raw ppm
			void ppm(const string        &filename,
					 const string        &comment,
					 const layout2D      &area,
					 double             (*vproc)( const T & ),
					 const gfx::rgb_t    *colors = NULL,
					 double               vmin   = 0,
					 double               vmax   = 1
					 ) const
            {
                assert( vproc != NULL );
                assert( this->has( area.lower ) );
                assert( this->has( area.upper ) );
                ios::ocstream fp( filename,false );
                fp("P6\n");
                
                //-- comment
                fp("#%s\n", &comment[0] );
                
                //-- size
                fp("%u %u\n", unsigned(area.width.x), unsigned(area.width.y) );
                
                //-- #colors
                fp("255\n");
                
                
                //-- data
                for( unit_t y = area.upper.y; y >= area.lower.y; --y )
                {
                    const row &r_y = (*this)[y];
                    for( unit_t x = area.lower.x; x <= area.upper.x; ++x )
                    {
                        const double v = vproc( r_y[x] );
                        const gfx::rgb_t c = gfx::ramp::to_rgb(v,colors,vmin,vmax);
                        fp.write( c.r );
                        fp.write( c.g );
                        fp.write( c.b );
                        
                    }
                }
            }
            
            inline void ppm(const char          *filename,
                            const char          *comment,
                            const layout2D      &area,
                            double             (*vproc)( const T & ),
                            const gfx::rgb_t    *colors = NULL,
                            double               vmin   = 0,
                            double               vmax   = 1
                            ) const
            {
                const string fn(filename);
                const string cm(comment);
                this->ppm(fn,cm,area,vproc,colors,vmin,vmax);
            }
#endif
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array2D);
            size_t    mrow;  //!< for allocation
            row      *rows;  //!< offseted rows
        };
        
        
    }
}

#endif
