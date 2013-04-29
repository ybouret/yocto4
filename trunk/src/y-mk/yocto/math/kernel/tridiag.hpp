#ifndef YOCTO_MATH_KERNEL_TRIDIAG_INCLUDED
#define YOCTO_MATH_KERNEL_TRIDIAG_INCLUDED 1

#include "yocto/sequence/lw-arrays.hpp"
#include "yocto/math/kernel/matrix.hpp"

namespace yocto {
    
	namespace math {
        
        //! base class for tridiag/xtridiag
        template <typename T>
        class tridiag_base
        {
        public:
            typedef lw_array<T> array_type;
            virtual ~tridiag_base() throw();
            
            void   make(size_t n);
            size_t size() const throw();
            
            //==================================================================
            // virtual interface
            //==================================================================
            virtual T operator()(size_t i, size_t j) const throw()  = 0;
            virtual bool __solve( array<T>  &x, const array<T>  &r) const throw() = 0;
            
            //==================================================================
            // non virtual interface
            //==================================================================
            void output( std::ostream &os ) const;
            
            inline friend
            std::ostream & operator<<( std::ostream &os, const tridiag_base &t)
            {
                t.output(os);
                return os;
            }
            
            void apply( array<T>  &v, const array<T>   &u) const throw();
            void apply( matrix<T> &v, const matrix<T>  &u) const throw();
            bool solve( array<T>  &r) const throw();
            bool solve( matrix<T> &x) const throw();
            
        protected:
            lw_arrays<T,memory::global> arrays;
            explicit tridiag_base( size_t nxtra );
            
        public:
            array_type &a;
            array_type &b;
            array_type &c;
            
        protected:
            array_type &g;
            array_type &xx;
            array_type &rr;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tridiag_base);
        };
        
        //! simple tridiagonal matrix
        template <class T>
        class tridiag : public tridiag_base<T>
        {
        public:
            typedef typename tridiag_base<T>::array_type array_type;
            
            explicit tridiag(size_t n=0);
            virtual ~tridiag() throw();
            
            virtual T operator()(size_t i, size_t j) const throw();
            virtual bool __solve( array<T>  &x, const array<T>  &r) const throw();
            
            bool sherman_morrison( array<T> &x, const array<T> &U, const array<T> &V, const array<T> &r) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(tridiag);
        };
        
        
        //! cyclic tridiagonal matrix
        template <class T>
        class ctridiag : public tridiag_base<T>
        {
        public:
            typedef typename tridiag_base<T>::array_type array_type;
            
            explicit ctridiag(size_t n=0);
            virtual ~ctridiag() throw();
            
            //! cyclic access
            /**
             M[1][n] = c[n]
             M[n][1] = a[1]
             */
            virtual T operator()(size_t i, size_t j) const throw();
            virtual bool __solve( array<T>  &x, const array<T>  &r) const throw();
            
            
            
        private:
            array_type &u;
            array_type &z;
            array_type &bb;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(ctridiag);
        };
        
    }
    
}

#endif
