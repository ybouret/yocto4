#ifndef YOCTO_MATH_FIT_GLSF_SPEC_INCLUDED
#define YOCTO_MATH_FIT_GLSF_SPEC_INCLUDED 1

#include "yocto/math/fit/glsf.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/math/core/lu.hpp"

namespace yocto
{
    namespace math
    {

        struct _GLS
        {
            //__________________________________________________________________
            //
            //! template creation of a fitting function
            //__________________________________________________________________
            template <
            typename T,
            template <typename> class FN
            >
            static inline
            typename GLS<T>::Function Create()
            {
                const FN<T> fn;
                return typename GLS<T>::Function(fn);
            }

            //__________________________________________________________________
            //
            //! template creation of a fitting function, 1 arg
            //__________________________________________________________________
            template <
            typename T,
            template <typename> class FN,
            typename U
            >
            static inline
            typename GLS<T>::Function Create(const U u)
            {
                const FN<T> fn(u);
                return typename GLS<T>::Function(fn);
            }

            //__________________________________________________________________
            //
            //! template creation of a fitting function, 1 arg
            //__________________________________________________________________
            template <
            typename T,
            template <typename> class FN,
            typename U,
            typename V
            >
            static inline
            typename GLS<T>::Function Create(const U u, const V v)
            {
                const FN<T> fn(u,v);
                return typename GLS<T>::Function(fn);
            }



            //__________________________________________________________________
            //
            //! polynomial function
            //__________________________________________________________________
            template <typename T>
            class Polynomial
            {
            public:
                inline  Polynomial() throw() {}
                inline  Polynomial(const Polynomial &) throw() {}
                inline ~Polynomial() throw() {}

                inline  T operator()(const T X, const array<T> &aorg)
                {
                    return Eval(X,aorg);
                }

                static inline
                T Eval(const T X, const array<T> &aorg) throw()
                {
                    const size_t n = aorg.size();
                    if(n>0)
                    {
                        T ans = aorg[n];
                        for(size_t i=n;i>1;)
                        {
                            ans *= X;
                            ans += aorg[--i];
                        }
                        return ans;
                    }
                    else
                    {
                        return 0;
                    }

                }

                static inline
                bool Start( const typename GLS<T>::Sample &S,  array<T> &a )
                {
                    assert(S.X.size()==S.Y.size());
                    assert(S.X.size()==S.Z.size());
                    tao::ld(S.Z,0);
                    const size_t n = a.size();
                    if( n>0 )
                    {
                        for(size_t i=n;i>0;--i) a[i] = 0;
                        matrix<T> mu(n);
                        for(size_t i=S.X.size();i>0;--i)
                        {
                            const T xi = S.X[i];
                            for(size_t j=n;j>0;--j)
                            {
                                const T xipjm = ipower<T>(xi,j-1);
                                a[j] += xipjm*S.Y[i];
                                for(size_t k=n;k>=j;--k)
                                {
                                    const T xx = xipjm * ipower<T>(xi,k-1);
                                    mu[k][j] = (mu[j][k] += xx);
                                }
                            }
                        }
                        if( !LU<T>::build(mu) )
                        {
                            return false;
                        }
                        LU<T>::solve(mu,a);
                        for(size_t i=S.Z.size();i>0;--i)
                        {
                            S.Z[i] = Eval(S.X[i],a);
                        }
                    }
                    return true;
                }

            private:
                YOCTO_DISABLE_ASSIGN(Polynomial);
            };


            //__________________________________________________________________
            //
            //! Gaussian functions
            //__________________________________________________________________
            template <typename T>
            class Gauss
            {
            public:
                inline Gauss() throw() {}
                inline ~Gauss() throw() {}
                inline Gauss(const Gauss &) throw() {}

                //! #coefficients = 3*gaussians: amplitude*exp( -(scaling*(x-shift))^2 )
                inline  T operator()(const T X, const array<T> &aorg)
                {
                    const size_t nc = aorg.size();
                    assert(nc>=3);
                    assert(0==nc%3);
                    const size_t n = nc/3;
                    size_t       i = 0;
                    T            g = 0;

                    for(size_t j=1;j<=n;++j)
                    {
                        const T amplitude = aorg[++i];
                        const T scaling   = aorg[++i];
                        const T shift     = aorg[++i];

                        const T arg       = scaling*(X-shift);
                        g += amplitude * Exp( - arg*arg );
                    }

                    return g;
                }
            };


            //__________________________________________________________________
            //
            //! Pade approximant
            //__________________________________________________________________
            template <typename T>
            class Pade
            {
            public:
                size_t p;
                size_t q;
                inline  Pade(const size_t P, const size_t Q) throw() : p(P), q(Q) {}
                inline ~Pade() throw() {}
                inline  Pade(const Pade &other) throw() : p(other.p), q(other.q) {}

                inline T operator()(const T X, const array<T> &a)
                {
                    const size_t n = a.size();
                    assert(p+q==n);
                    if(n>0)
                    {
                        const size_t nden = q;
                        const size_t nnum = p;

                        T num = 0;
                        for(size_t i=1;i<=nnum;++i)
                        {
                            num += a[i] * ipower<T>(X,i-1);
                        }

                        T den = 1;
                        for(size_t i=1;i<=nden;++i)
                        {
                            num += a[i+nnum] * ipower<T>(X,i);
                        }
                        return num/den;
                    }
                    else
                    {
                        return 0;
                    }
                }

            private:
                YOCTO_DISABLE_ASSIGN(Pade);
            };

        };


    }

}

#endif

