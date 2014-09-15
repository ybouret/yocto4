#ifndef YOCTO_MATH_FIT_GNL_LSF_INCLUDED
#define YOCTO_MATH_FIT_GNL_LSF_INCLUDED 1



#include "yocto/counted.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
	namespace math
	{
        
		template <typename T>
		class LeastSquares
		{
		public:
			typedef array<T>                        Array;
			typedef vector<T>                       Vector;
			typedef matrix<T>                       Matrix;
			typedef matrix<ptrdiff_t>               iMatrix;
			typedef functor<T,TL2(T,const Array &)> Function;
			typedef typename numeric<T>::function   Function1;
            
			virtual  ~LeastSquares() throw();
			explicit  LeastSquares();
            
            
			//! a sample to wrap data+local memory
			class Sample : public object, public counted
			{
			public:
				virtual ~Sample() throw();
				explicit Sample(const Array &userX,
                                const Array &userY,
                                Array       &userZ);
                
				typedef arc_ptr<Sample> Pointer;
                
				const Array &X;
				const Array &Y;
				Array       &Z;
				const size_t N;      //!< initially 0, #data
				const size_t Q;      //!< initially 0, #local variables
				const size_t M;      //!< initially 0, #global variables
				iMatrix      Gamma;  //!< [QxM], local variables from global variables
				Vector       u;      //!< [Q] local variables
				Vector       dFdu;   //!< [Q] local function gradient
				Vector       beta;   //!< [Q] local least square gradient
				Matrix       alpha;  //!< [QxQ] local curvature
				Matrix       __ag;   //!< [Q*M] alpha * Gamma
				Matrix       Alpha;  //!< [MxM] Gamma'*__ag
                
				T compute_D(Function &F, const Array &a);
				T compute_D(Function &F, const Array &a, derivative<T> &drvs, T h);
                
				//! set N and memory
				void prepare(size_t local_nvar, size_t global_nvar);
                
				//! set N and memory, Gamma=Id
				void prepare(size_t nvar);
                
				void collect( Matrix &global_alpha, Array &global_beta ) const throw();
                
                
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Sample);
				class Wrapper
				{
				public:
					explicit Wrapper(Sample&) throw();
					virtual ~Wrapper() throw();
					Sample   &S;
					Function *F;
					T         x;
					size_t    q;
					T        Eval(T U);
                    
				private:
					YOCTO_DISABLE_COPY_AND_ASSIGN(Wrapper);
				};
				Wrapper              w;
				Function1            f; //!< for gradient evaluation
			};
            
			//! a set of samples
			class Samples : public vector<typename Sample::Pointer>
			{
			public:
				explicit Samples() throw();
				virtual ~Samples() throw();
				Sample & append( const Array &X, const Array &Y, Array &Z );
                
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Samples);
			};
            
            
			void operator()(
                            Samples           &user_S,
                            Function          &user_F,
                            Array             &user_aorg,
                            const array<bool> &user_used,
                            Array             &user_aerr);
            
            
			static const int LAMBDA_MIN_POW10;
			static const int LAMBDA_MAX_POW10;
			static T compute_lam(int p) throw();
            
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(LeastSquares);
			Samples      *S;
			Function     *F;
			size_t        ns;    //! #samples
			size_t        nvar;  //! #variables
			Vector        aorg;  //!< current values
			vector<bool>  used;  //!< which one are used
            Vector        atmp;  //!< for function evaluation
			Vector        aerr;  //!< error
			Vector        beta;  //!< reduced gradient
			Matrix        alpha; //!< approx. curvature
			Matrix        curv;  //!< from alpha with lam
			Vector        step;  //!< approx. step
			derivative<T> drvs;
			T             h;
            
			T       computeD();
			bool    build_curvature(T lam); //!< curv from alpha and try lu
            T       evalD(T x);             //!< atmp = aorg+x*step
            
		};
        
	}
}

#endif
