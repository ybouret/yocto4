#ifndef YOCTO_SWAMP_ARRAY3D_INCLUDED
#define YOCTO_SWAMP_ARRAY3D_INCLUDED 1

#include "yocto/swamp/in3d.hpp"
#include "yocto/swamp/array2d.hpp"

#include <iosfwd>

namespace yocto 
{
    namespace swamp
    {
        
        
        //! three dimensional array
        template <typename T>
        class array3D : public linear<T,layout3D>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef array3D<T>                     array_type;
            typedef linear<T,layout3D>             linear_type;
            typedef layout3D                       layout_type;
            typedef array2D<T>                     slice_type;
            typedef array1D<T>                     row_type;
            typedef typename linear_type::callback callback;
            typedef typename linear_type::const_cb const_cb;
            typedef typename linear_type::call_two call_two;
            
            explicit array3D( const layout_type & L );           
            virtual ~array3D() throw();
            
            slice_type       & operator[]( unit_t z ) throw();
            const slice_type & operator[]( unit_t z ) const throw();
            
            
            virtual void link( void *data ) throw();
            virtual void foreach( const layout_type &sub, callback proc, void *args );
            virtual void foreach( const layout_type &sub, const_cb proc, void *args ) const;
            void         foreach( const array_type  &other, const layout_type &sub, call_two proc, void *args);
            
            void set( const array_type &other, const layout_type &sub ) throw();
            void add( const array_type &other, const layout_type &sub ) throw();
            
            template <typename U>
			friend std::ostream & operator<<( std::ostream &os, const array3D<U> &a);
            
            const layout2D slice_layout;
            
            static void       *ctor( const layout_type &L, linear_base **info );
            static void        dtor(void*) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array3D);
            size_t      num_slices;
            slice_type *slice; //!< offseted slices
        };
        
        template <typename T,size_t DIM>
        struct __array;
        
        template <typename T>
        struct __array<T,1> {
            typedef array1D<T> type;
        };
        
        template <typename T>
        struct __array<T,2> {
            typedef array2D<T> type;
        };

        template <typename T>
        struct __array<T,3> {
            typedef array3D<T> type;
        };


        
    }
}

#endif
