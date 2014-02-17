#ifndef YOCTO_MATH_FIT_LEAST_SQUARES_INCLUDED
#define YOCTO_MATH_FIT_LEAST_SQUARES_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/counted.hpp"
#include "yocto/functor.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/math/kernel/crout.hpp"

namespace yocto
{
    
    namespace math
    {
        
        //! return from least_squares
        enum least_squares_result
        {
            least_squares_success, //!< everyting OK
            least_squares_failure, //!< singular point
            least_squares_spurious //!< likely a minimum, should check
        };
        
        
        //! performing a non linear least squares fit on multiple samples
        template <typename T>
        class least_squares
        {
        public:
            
            typedef functor<T,TL2(T,const array<T>&)> function;
            
            //! a sample: holds data and algebra
            class sample : public object, public counted
            {
            public:
                const array<T> &X; //!< the X value
                const array<T> &Y; //!< the Y value
                array<T>       &Z; //!< the fitted value of Y
                const size_t    N; //!< when prepared
                
                virtual ~sample() throw();
                explicit sample( const array<T> &aX, const array<T> &aY, array<T> &aZ ) throw();
                
                typedef arc_ptr<sample> pointer;
                
                vector<T> beta;   //!< partial gradient    [1..nvar]
                matrix<T> alpha;  //!< partial curvature   [1..nvar:1..nvar]
                matrix<T> Gamma;  //!< variables           [1..npar:1..nvar]
                vector<T> u;      //!< reduced variables   [1..npar]. u = Gamma * a
                vector<T> dFdu;   //!< reduced gradient    [1..npar].
                vector<T> dFda;   //!< full gradient       [1..nvar]. dFda = Gamma' * dFdu
                T         D2;     //!< least squares value
                
                T compute_D2( function &F, const array<T> &a );
                T compute_D2( function &F, const array<T> &a, const array<bool> &used, derivative<T> &drvs, const T h);
                
                //! prepare all
                /**
                 \param nvar total number of variables
                 \param npar number of parameters for the fit function
                 if(nvar==npar) then Gamma is set to identity
                 */
                void prepare( size_t nvar, size_t npar );
                void release() throw();

                inline void prepare(size_t nvar) { prepare(nvar,nvar); }
                
                //! set parameter ipar to variable ivar
                /**
                 set Gamma[ipar][ivar] = 1.
                 **/
                void set_parameter(size_t ipar, size_t ivar) throw();
                
                void polynomial( array<T> &aorg, const array<bool> &used, array<T> &aerr, typename numeric<T>::function *transform = 0);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(sample);
            };
            
            
            //! a sequence of samples
            class samples : public vector<typename sample::pointer>
            {
            public:
                explicit samples() throw();
                virtual ~samples() throw();
                void     append(const array<T> &aX, const array<T> &aY, array<T> &aZ);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(samples);
            };
            
            typedef functor<bool,TL2(function &,const  samples &)> callback;
            
            //! default ctor
            explicit least_squares();
            
            //! default dtor
            virtual ~least_squares() throw();
            
            T             D2;      //!< global D2
            derivative<T> drvs;    //!< to compute derivatives
            T             h;       //!< for derivatives, default = 1e-4
            T             ftol;    //!< for convergence, default = numeric<T>::ftol
            bool          verbose; //!< little printout, default is false
            
            //! generic call
            least_squares_result operator()(function                        &F,
                                            samples                         &Samples,
                                            array<T>                        &Aorg,
                                            const array<bool>               &Used,
                                            array<T>                        &Aerr,
                                            callback                        *cb = 0
                                            );
            
           
            
            
        private:
            function                        *proc;
            samples                         *data;
            size_t                           nvar;
            size_t                           nums;
            matrix<T>                        alpha;
            matrix<T>                        curv;
            vector<T>                        beta;
            vector<T>                        step;
            vector<T>                        aorg;
            vector<bool>                     used;
            vector<T>                        atmp;
            crout<T>                         LU;
            
            T    compute_D2_org();
            T    compute_D2_tmp(T);
            bool build_curvature(T lambda);
            void cleanup() throw();
            static const char fn[];
        };
        
        
        
    }
    
}


#endif

