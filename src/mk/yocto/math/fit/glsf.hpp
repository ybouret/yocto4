#ifndef YOCTO_MATH_FIT_GLSF_INCLUDED
#define YOCTO_MATH_FIT_GLSF_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/container/matrix.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace math
    {



        ////////////////////////////////////////////////////////////////////////
        //
        // Generalized Least Squares
        //
        ////////////////////////////////////////////////////////////////////////
        template <typename T>
        class GLS
        {
        public:
            static int GET_P10_MAX() throw();

            typedef array<T>                           Array;
            typedef vector<T>                          Vector;
            typedef functor<T,TL2(T,const Array &)>    Function;
            typedef typename numeric<T>::function      Function1;

            //__________________________________________________________________
            //
            //
            // a sample: holds reference to data+fit
            //
            //__________________________________________________________________
            class Sample : public counted_object
            {
            public:
                typedef arc_ptr<Sample> Pointer;

                explicit Sample( const Array &XX, const Array &YY, Array &ZZ );
                virtual ~Sample() throw();

                const Array &X; //!< ref to X data, unchanged
                const Array &Y; //!< ref to Y data, unchanged
                Array       &Z; //!< ref to fitted data, updated

                const size_t N;     //!< current max number of points, recomputed
                const size_t L;     //!< local  #variables
                const size_t M;     //!< global #variables
                matrix<int>  Gamma; //!< L*M matrix
                Vector       u;     //!< #L local variables = Gam*a
                Vector       dFdu;  //!< #L local gradient
                Vector       dFda;  //!< #M global gradient
                Vector       beta;  //!< #M descent direction
                matrix<T>    curv;  //!< M*M global curvature

                //! link global variable index to local variable index
                void link( const size_t iLocal, const size_t iGlobal);

                //!compute D2 and update the Z term
                T computeD2(Function    &F,
                            const Array &a);

                //! compute D2, beta and curvature, update Z term
                T computeD2(Function          &F,
                            const Array       &a,
                            derivative<T>     &drvs,
                            const T            du);

                //! prepare memory with Gamma=0 for simultaneous fit
                void prepare( size_t global_nvar, size_t local_nvar );

                //! prepare memory with Gamma=Id, assuming single fit...
                void prepare( size_t global_nvar );

            private:
                Function1    F1;    //!< call Eval for gradient
                size_t       ivar;  //!< for gadient
                T            xval;  //!< for gradient
                Function    *hook;  //!< for gradient
                YOCTO_DISABLE_COPY_AND_ASSIGN(Sample);

                T Eval( const T U );
            };


            //__________________________________________________________________
            //
            //
            // Samples: hold multiple samples
            //
            //__________________________________________________________________
            typedef vector<typename Sample::Pointer> _Samples;
            class Samples;
            typedef functor<bool,TL2(const Samples &,const Array&)> Callback;

            class Samples : public _Samples
            {
            public:
                const size_t   M;
                Vector         beta;
                Vector         step;
                Vector         atry;
                matrix<T>      curv;
                matrix<T>      cinv;
                derivative<T>  drvs;
                T              scale;    //! scaling for fit, may be important !
                const int      p10_min;
                const int      p10_max;
                size_t         cycle;   //!< cycle index
                
                explicit Samples(size_t n=1);
                virtual ~Samples() throw();


                //______________________________________________________________
                //
                //! create and manage a new sample
                //______________________________________________________________
                Sample &append( const Array &X, const Array &Y, Array &Z);

                //______________________________________________________________
                //
                //! prepare this and all samples
                //______________________________________________________________
                void prepare(size_t global_nvar, size_t local_nvar);

                //______________________________________________________________
                //
                //! prepare this and all samples
                //______________________________________________________________
                void prepare(size_t global_nvar);

                //______________________________________________________________
                //
                //! simple evaluation for all samples
                //______________________________________________________________
                T computeD2_(Function &F, const Array &a );

                //______________________________________________________________
                //
                //! full evaluation at aorg
                //______________________________________________________________
                T computeD2(Function          &F,
                            const Array       &aorg,
                            const array<bool> &used);



                //______________________________________________________________
                //
                //! fit algorithm
                //______________________________________________________________
                bool fit_with(Function          &F,
                              Array             &aorg,
                              const array<bool> &used,
                              Array             &aerr,
                              Callback          *cb=0);


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Samples);

                //______________________________________________________________
                //
                // allocate own variables
                //______________________________________________________________
                void setup(size_t nvar);

                //______________________________________________________________
                //
                // compute lambda from p10
                //______________________________________________________________
                T compute_lambda(const int p10);

                //______________________________________________________________
                //
                //! try to approximate the inverse of curvature
                //______________________________________________________________
                bool compute_cinv(const T lambda);

                //______________________________________________________________
                //
                //! find acceptable lambda so that curvature is invertible
                //______________________________________________________________
                T find_acceptable_lambda(int &p10);
            };


            static void display( std::ostream &os, const Array &aorg, const Array &aerr);

            
        };
        
    }
}

#endif
