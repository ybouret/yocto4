#ifndef YOCTO_SWAMP_ARRAY2D_INCLUDED
#define YOCTO_SWAMP_ARRAY2D_INCLUDED 1

#include "yocto/swamp/in2d.hpp"
#include "yocto/swamp/array1d.hpp"

#include <iosfwd>

namespace yocto 
{
    namespace swamp
    {
        
        
        template <typename T>
        class array2D : public linear<T,layout2D>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef array2D<T>                     array_type;
            typedef linear<T,layout2D>             linear_type;
            typedef layout2D                       layout_type;
            typedef array1D<T>                     row_type;

            typedef typename linear_type::callback callback;
            typedef typename linear_type::const_cb const_cb;
            typedef typename linear_type::call_two call_two;
            
            explicit array2D( const layout_type & L );           
            virtual ~array2D() throw();
            
            row_type       & operator[]( unit_t y ) throw();
            const row_type & operator[]( unit_t y ) const throw();
            
            
            virtual void link( void *data ) throw();
            virtual void foreach( const layout_type &sub, callback proc, void *args );
            virtual void foreach( const layout_type &sub, const_cb proc, void *args ) const;
            void         foreach( const array_type  &other, const layout_type &sub, call_two proc, void *args);
            
            void set( const array_type &other, const layout_type &sub ) throw();
            void add( const array_type &other, const layout_type &sub ) throw();
            
            template <typename U>
			friend std::ostream & operator<<( std::ostream &os, const array2D<U> &a);

            const layout1D row_layout;
            
            static inline void       *ctor( const layout_type &L, linear_base **info);
            static inline void        dtor(void*) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array2D);
            size_t    num_rows;
            row_type *row; //!< offseted rows
        };
        
    }
}

#endif
