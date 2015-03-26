/** \file  zfind.hpp
 \brief Root Finding by Ridders' Method.
 */

#ifndef YOCTO_ZFIND_INCLUDED
#define YOCTO_ZFIND_INCLUDED 1

#include "yocto/math/triplet.hpp"

namespace yocto {

    namespace math {

        template <typename T>
        class zfunction
        {
        public:
            typedef typename numeric<T>::function func_type;
            func_type func;    //!< original function
            T         target;  //!< target value
            func_type call;    //!< original function - target

            inline zfunction( const typename numeric<T>::function &fcn, const T tgt) :
            func(fcn),
            target(tgt),
            call( this, &zfunction<T>::compute__)
            {
            }

            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            inline zfunction( OBJECT_POINTER host, METHOD_POINTER method, const T tgt) :
            func(host,method),
            target(tgt),
            call(this, &zfunction<T>::compute__)
            {

            }

            inline ~zfunction() throw() {}

        private:
            inline T compute__( const T x )
            {
                return func(x)-target;
            }
        };



        //! find zero between x.a and x.c
        /**
         f.a and f.c must be provided and f.a * f.c <= 0
         xerr>=0
         */
        template <typename T>
        class zfind
        {
        public:
            explicit zfind( T default_xerr ) throw();
            virtual ~zfind() throw();

            T       xerr;   //!< error on x
            T       growth; //!< for bracketing: default is 1.6180339887
            size_t  trials; //!< for bracketing: max trials, default is 50

            //! find zero between x.a and x.c
            /**
             f.a and f.c must be provided and f.a * f.c <= 0
             x.b and f.b are the results
             it may fail if proc is not continuous...
             return x.b
             */
            T run( typename numeric<T>::function &proc, triplet<T> &x, triplet<T> &f) const;

            //! best effort solving
            /**
             \param proc a continuous function
             \param xa   an initial range boundary
             \param xc   an initial range boundary
             */
            T operator()(typename numeric<T>::function &proc,
                         T xa,
                         T xc) const;


            //! starting from x.a and x.c, try to bracket one zero
            /**
             \param proc a function
             \param x    the initial [x.a:x.c] range
             \param f    an uninitialized triplet
             f is initialized and updated.
             */
            bool lookup( typename numeric<T>::function &proc, triplet<T> &x, triplet<T> &f) const;


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(zfind);
        };
        
        //YOCTO_DMC_MATH(struct zfind)
    }
    
}

#endif
