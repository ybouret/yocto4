#ifndef YOCTO_MATH_FIT_GNL_LSF_INCLUDED
#define YOCTO_MATH_FIT_GNL_LSF_INCLUDED 1


#include "yocto/math/kernel/tao.hpp"
#include "yocto/counted.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        class LeastSquares
        {
        public:
            typedef array<T>  Array;
            typedef vector<T> Vector;
            typedef matrix<T> Matrix;
            typedef matrix<ptrdiff_t> iMatrix;
            
            virtual  ~LeastSquares() throw();
            explicit  LeastSquares();

            
            //! a sample to wrap data+local memory
            class Sample : public object, public counted
            {
            public:
                virtual ~Sample() throw();
                explicit Sample(const Array &userX,
                                const Array &userY,
                                const Array &userZ) throw();
                
                typedef arc_ptr<Sample> Pointer;
                
                const Array &X;
                const Array &Y;
                const Array &Z;
                const size_t N;     //!< initially 0, #data
                const size_t Q;     //!< initially 0, #local variables
                const size_t M;     //!< initially 0, #global variables
                iMatrix      Gamma; //!< [QxM]
                Vector       u;     //!< [Q] local variables
                Vector       beta;  //!< [Q] local gradient
                
                
                //! set N and memory
                void prepare(size_t local_nvar, size_t global_nvar);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Sample);
            };
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(LeastSquares);
        };
        
    }
}

#endif
