#ifndef YOCTO_MATH_FIT_LEAST_SQUARES_INCLUDED
#define YOCTO_MATH_FIT_LEAST_SQUARES_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/counted.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
 
    namespace math
    {
        
        namespace fit
        {
            
            template <typename T>
            class least_squares
            {
            public:
                
                typedef functor<T,TL2(T,const array<T>&)> function;
                
                class sample : public object, public counted
                {
                public:
                    const array<T> &X;
                    const array<T> &Y;
                    array<T>       &Z;
                    const size_t    N; //!< when prepared
                    
                    virtual ~sample() throw();
                    explicit sample( const array<T> &aX, const array<T> &aY, array<T> &aZ ) throw();
                    
                    typedef arc_ptr<sample> pointer;
                    
                    vector<T> beta;   //!< partial gradient
                    matrix<T> alpha;  //!< partial curvature
                    matrix<T> Gamma;  //!< variables
                    vector<T> u;      //!< reduced variables
                    vector<T> dFdu;   //!< reduced gradient
                    T         D2;     //!< least squares value
                    
                    T compute_least_squares( function &F, const array<T> &a );
                    
                    //! prepare all
                    /**
                     \param nvar total number of variables
                     \param npar number of parameters for the fit function
                     */
                    void prepare( size_t nvar, size_t npar );
                    void release() throw();
                    
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(sample);
                };
                
                
                virtual ~least_squares() throw();
                
            private:
                
            };
            
        }
        
    }
    
}


#endif

