#ifndef YOCTO_SWAMP_ARRAY1D_INCLUDED
#define YOCTO_SWAMP_ARRAY1D_INCLUDED 1

#include "yocto/swamp/in1d.hpp"
#include "yocto/swamp/linear.hpp"

namespace yocto 
{
    namespace swamp
    {
        
        
        template <typename T>
        class array1D : public linear<T,layout1D>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef array1D<T>         array_type;
            typedef linear<T,layout1D> linear_type;
            typedef layout1D           layout_type;
            typedef typename linear_type::callback callback;
            typedef typename linear_type::const_cb const_cb;
            typedef typename linear_type::call_two call_two;

            explicit array1D( const layout_type & L ) throw();            
            virtual ~array1D() throw();
            
            type &       operator[]( unit_t x ) throw();
            const_type & operator[]( const unit_t x) const throw();
            
            
            virtual void link( void *data ) throw();
            virtual void foreach( const layout_type &sub, callback proc, void *args );
            virtual void foreach( const layout_type &sub, const_cb proc, void *args ) const;
            void         foreach( const array_type  &other, const layout_type &sub, call_two proc, void *args);
            
            void set( const array_type &other, const layout_type &sub ) throw();
            void add( const array_type &other, const layout_type &sub ) throw();
            
            template <typename U>
			friend std::ostream & operator<<( std::ostream &os, const array1D<U> &a);
            
            static void       *ctor( const layout_type &L, linear_base **info );
            static void        dtor(void*) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(array1D);
            T *item; //!< offseted address 
        };
        
    }
}

#endif
