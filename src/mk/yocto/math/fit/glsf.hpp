#ifndef YOCTO_MATH_FIT_GLSF_INCLUDED
#define YOCTO_MATH_FIT_GLSF_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/container/matrix.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

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

                const Array &X;
                const Array &Y;
                Array       &Z;

                const size_t N;     //!< current max number of points
                const size_t L;     //!< local  #variables
                const size_t M;     //!< global #variables
                matrix<int>  Gamma; //!< L*M matrix
                Vector       u;     //!< #L local variables = Gam*a
                Vector       dFdu;  //!< #L local gradient
                Vector       dFda;  //!< #M global gradient
                Vector       beta;  //!< #M descent direction
                matrix<T>    curv;  //!< M*M local curvature

                //! link global variable index to local variable index
                void link( size_t iGlobal, size_t iLocal );

                //!compute D2 and update the Z term
                T computeD2(Function    &F,
                            const Array &a);

                //! compute D2 and curvature
                T computeD2(Function      &F,
                            const Array   &a,
                            derivative<T> &drvs,
                            const T        du);

                //! prepare memory with Gamma=0
                void prepare( size_t global_nvar, size_t local_nvar );

                //! prepare memory with Gamma=Id
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

            class Samples : public _Samples
            {
            public:
                const size_t   M;
                Vector         beta;
                Vector         step;
                Vector         atry;
                Vector         desc;
                matrix<T>      curv;
                matrix<T>      cinv;
                derivative<T>  drvs;
                T              scale;
                const int      p10_min;
                const int      p10_max;

                explicit Samples(size_t n) :
                _Samples(n,as_capacity),
                M(0),
                beta(),
                step(),
                atry(),
                desc(),
                curv(),
                drvs(),
                scale(1e-4),
                p10_min( -GLS<T>::GET_P10_MAX() ),
                p10_max( -p10_min               ),
                scan( this, & Samples::probe ),
                hook(0),
                pvar(0)
                {
                }

                virtual ~Samples() throw()
                {
                }

                //______________________________________________________________
                //
                // create and manage a new sample
                //______________________________________________________________
                inline
                Sample &append( const Array &X, const Array &Y, Array &Z)
                {
                    typename Sample::Pointer p( new Sample(X,Y,Z) );
                    this->push_back(p);
                    return *p;
                }

                //______________________________________________________________
                //
                // prepare this and all samples
                //______________________________________________________________
                inline void prepare(size_t global_nvar, size_t local_nvar)
                {
                    _Samples &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        self[i]->prepare(global_nvar,local_nvar);
                    }
                    setup(global_nvar);
                }


                //______________________________________________________________
                //
                // prepare this and all samples
                //______________________________________________________________
                inline void prepare(size_t global_nvar)
                {
                    _Samples &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        self[i]->prepare(global_nvar);
                    }
                    setup(global_nvar);
                }

                //______________________________________________________________
                //
                // simple evaluation for all samples
                //______________________________________________________________
                T computeD2_(Function &F, const Array &a )
                {
                    T D2 = 0;
                    _Samples &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        D2 += self[i]->computeD2(F,a);
                    }
                    return D2;
                }


                //______________________________________________________________
                //
                // full evaluation at aorg
                //______________________________________________________________
                inline T computeD2(Function          &F,
                                   const Array       &aorg,
                                   const array<bool> &used)

                {
                    assert(M==aorg.size());
                    assert(M==used.size());

                    //__________________________________________________________
                    //
                    // initialize
                    //__________________________________________________________
                    tao::ld(beta,0);
                    curv.ldz();
                    T D2   = 0;

                    //__________________________________________________________
                    //
                    // full computation
                    //__________________________________________________________
                    _Samples &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        Sample &S = *self[i];
                        D2 += S.computeD2(F,aorg,drvs,scale);
                        //std::cerr << "beta" << i << "=" << S.beta << std::endl;
                        //std::cerr << "curv" << i << "=" << S.curv << std::endl;
                        for(size_t j=M;j>0;--j)
                        {
                            beta[j] += S.beta[j];
                            for(size_t k=M;k>=j;--k)
                            {
                                curv[j][k] += S.curv[j][k];
                            }
                        }
                    }

                    //__________________________________________________________
                    //
                    // legalizing
                    //__________________________________________________________
                    for(size_t j=M;j>0;--j)
                    {
                        if(used[j])
                        {
                            for(size_t r=1;r<=M;++r)
                            {
                                for(size_t c=1;c<r;++c)
                                {
                                    curv[r][c] = curv[c][r];
                                }
                            }
                        }
                        else
                        {
                            beta[j] = 0;
                            for(size_t i=M;i>0;--i)
                            {
                                curv[i][j] = curv[j][i] = 0;
                            }
                            curv[j][j] = 1;
                        }
                    }

                    std::cerr << "beta=" << beta << std::endl;
                    std::cerr << "curv=" << curv << std::endl;
                    return D2;
                }


                //______________________________________________________________
                //
                // try to approximate the inverse of curvature
                //______________________________________________________________
                inline bool compute_cinv(const T lambda)
                {
                    assert(lambda>=0);
                    const T fac = T(1)+lambda;
                    cinv.assign(curv);
                    for(size_t i=M;i>0;--i)
                    {
                        cinv[i][i] *= fac;
                    }
                    return LU<T>::build(cinv);
                }

                //______________________________________________________________
                //
                // compute lambda from p10
                //______________________________________________________________
                inline T compute_lambda(const int p10)
                {
                    if(p10<=p10_min)
                    {
                        return 0;
                    }
                    else
                    {
                        const int p = min_of<int>(p10,p10_max);
                        if(p>=0)
                        {
                            return ipower<T>(10,size_t(p));
                        }
                        else
                        {
                            return ipower<T>(0.1,size_t(-p));
                        }
                    }
                }


                //______________________________________________________________
                //
                // find acceptable lambda so that curvature is invertible
                //______________________________________________________________
                inline T find_acceptable_lambda(int &p10)
                {
                    assert(p10>=p10_min);
                    assert(p10<=p10_max);
                    T lambda = -1;
                    while( !compute_cinv( (lambda=compute_lambda(p10)) ) )
                    {
                        ++p10;
                        if(p10>p10_max)
                        {
                            return -1;
                        }
                    }
                    return lambda;
                }

                bool fit_with(Function          &F,
                              Array             &aorg,
                              const array<bool> &used)
                {
                    assert(aorg.size()==M);
                    assert(used.size()==M);

                    //__________________________________________________________
                    //
                    // initialize
                    //__________________________________________________________
                    hook      = &F;
                    pvar      = &aorg;
                    int  p10  = -4;
                    T    lam  = -1;
                    T    Horg = computeD2(F,aorg,used);

                    //__________________________________________________________
                    //
                    // main loop
                    //__________________________________________________________
                    size_t count = 0;
                    while(true)
                    {
                        ++count;
                        //if(count>40) break;
                        //______________________________________________________
                        //
                        // adjust p10, curvature is computed@aorg
                        //______________________________________________________
                        lam = find_acceptable_lambda(p10);
                        if(lam<0)
                        {
                            return false; //! no scaling...
                        }

                        const T beta_sq = tao::norm_sq(beta);
                        std::cerr << "beta=" << beta << std::endl;
                        std::cerr << "beta_norm=" << Sqrt(beta_sq) << std::endl;
                        if(beta_sq<=0)
                        {
                            // level 1 success
                            goto EXTREMUM;
                        }

                        //______________________________________________________
                        //
                        // compute step
                        //______________________________________________________
                        LU<T>::solve(step,cinv,beta);
                        const T step_sq = tao::norm_sq(step);
                        std::cerr << "lam =" << lam  << std::endl;
                        std::cerr << "step=" << step << std::endl;
                        std::cerr << "step_norm=" << Sqrt(step_sq) << std::endl;
                        if(step_sq<=0)
                        {
                            // level 2 success
                            goto EXTREMUM;
                        }

                        //______________________________________________________
                        //
                        // |beta|>0, |step|>0
                        //______________________________________________________
                        T Hnew = scan(1);
                        //std::cerr << "H: " << Horg << " -> " << Hnew << std::endl;
                        if(Hnew<Horg)
                        {
                            std::cerr << "SUCCESS" << std::endl;
                            tao::set(aorg,atry);
                        }
                        else
                        {
                            std::cerr << "FAILURE" << std::endl;
                            //__________________________________________________
                            //
                            // find a guess descent
                            //__________________________________________________
                            std::cerr << "step0=" << step << std::endl;
                            for(size_t i=M;i>0;--i)
                            {
                                if( (beta[i]*step[i]) < 0 )
                                {
                                    step[i] = 0;
                                }
                            }
                            std::cerr << "step1=" << step << std::endl;

                            triplet<T> XX = { 0, 1 , 1 };
                            triplet<T> HH = { Horg, scan(1), 0 };
                            HH.c = HH.b;
                            bracket<T>::expand(scan,XX,HH);
                            //std::cerr << "XX=" << XX << std::endl;
                            //std::cerr << "HH=" << HH << std::endl;
                            minimize<T>(scan, XX, HH, 0);
                            std::cerr << "XX=" << XX << std::endl;
                            std::cerr << "HH=" << HH << std::endl;
                            for(size_t i=M;i>0;--i)
                            {
                                aorg[i] = aorg[i] + XX.b * step[i];
                            }
                            if(++p10>p10_max)
                            {
                                std::cerr << "Max decreased reached!" << std::endl;
                                goto EXTREMUM;
                            }
                        }

                        // prepare next step
                        Hnew = computeD2(F,aorg,used);
                        std::cerr << "H: " << Horg << " -> " << Hnew << std::endl;
                        if(Hnew>=Horg)
                            goto EXTREMUM;
                        Horg = Hnew;
                    }

                EXTREMUM:
                    return true;
                }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Samples);
                Function1 scan;
                
                Function *hook;
                Array    *pvar;
                
                inline void setup(size_t nvar)
                {
                    assert(nvar>0);
                    (size_t &)M = nvar;
                    beta.make(M);
                    step.make(M);
                    atry.make(M);
                    desc.make(M);
                    curv.make(M,M);
                    cinv.make(M);    // large memory model for LU
                }
                
                inline T probe(const T u)
                {
                    assert(hook);
                    assert(pvar);
                    tao::setprobe(atry,*pvar,u,step);
                    return computeD2_(*hook,atry);
                }
                
            };
        };
        
    }
}

#endif
