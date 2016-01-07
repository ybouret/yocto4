#ifndef YOCTO_MATH_TRIDIAG_INCLUDED
#define YOCTO_MATH_TRIDIAG_INCLUDED 1

#include "yocto/sequence/many-arrays.hpp"
#include "yocto/xnumeric.hpp"
#include "yocto/container/matrix.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/math/core/lu.hpp"

namespace yocto
{
    namespace math
    {

        //! tridiagonal base class, for simple or cyclic
        template <typename T>
        class _TriDiag
        {
        public:
            typedef array<T>       array_type;
            typedef memory::global memory_type;
            virtual ~_TriDiag() throw() {}

            //__________________________________________________________________
            //
            // Virtual Interface
            //__________________________________________________________________
            virtual T operator()(size_t i,size_t j) const = 0;
            virtual bool __solve( array_type &x, const array_type &r) = 0;


            //__________________________________________________________________
            //
            // Non-Virtual Interface
            //__________________________________________________________________

            //! solve an array
            inline bool solve(array_type &x, const array_type &r)
            {
                return __solve(x,r);
            }

            inline bool solve(array_type &x)
            {
                assert(x.size()>=size());
                const size_t n = size();
                for(size_t i=n;i>0;--i) rr[i] = x[i];
                return __solve(x,rr);
            }

            //! solve column vectors of a matrix
            inline bool solve(matrix<T> &Z)
            {
                assert(Z.rows>=size());
                const size_t n  = size();
                const size_t nc = Z.cols;
                for(size_t j=1;j<=nc;++j)
                {
                    for(size_t i=n;i>0;--i) rr[i] = Z[i][j];
                    if(!__solve(xx,rr)) return false;
                    for(size_t i=n;i>0;--i) Z[i][j] = xx[i];
                }
                return true;
            }


            inline size_t size() const throw() { return arrays.size; }

            inline void output( std::ostream &os ) const
            {
                const _TriDiag<T> &self = *this;
                const size_t       n    = size();
                os << "[";
                for( size_t i=1; i <= n; ++i )
                {
                    for( size_t j=1; j <= n; ++j )
                    {
                        os << " " << self(i,j);
                    }
                    if( i < n )
                        os << " ;";
                }
                os << "]";
            }

            inline friend std::ostream & operator<<( std::ostream &os, const _TriDiag<T> &M )
            {
                M.output(os);
                return os;
            }

            inline void set( matrix<T> &M ) const
            {
                assert(M.cols==M.rows);
                assert(M.rows==size());
                const _TriDiag<T> &self = *this;
                const size_t       n    = size();
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        M[i][j] = self(i,j);
                    }
                }
            }

            //! u = this * v
            inline void apply( array_type &u, const array_type &v) const
            {
                assert(u.size()>=size());
                assert(v.size()>=size());
                const _TriDiag<T> &self = *this;
                const size_t       n    = size();
                for(size_t i=1;i<=n;++i)
                {
                    T sum = xnumeric<T>::zero();
                    for(size_t j=1;j<=n;++j)
                    {
                        sum += self(i,j) * v[j];
                    }
                    u[i] = sum;
                }
            }

        protected:
            many_arrays<T,memory_type> arrays;

        public:
            array_type &a;
            array_type &b;
            array_type &c;

        protected:
            explicit inline _TriDiag(size_t na) :
            arrays(6+na),
            a(  arrays.next_array() ),
            b(  arrays.next_array() ),
            c(  arrays.next_array() ),
            g(  arrays.next_array() ),
            xx( arrays.next_array() ),
            rr( arrays.next_array() )
            {
            }

            array_type &g;
            array_type &xx;
            array_type &rr;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(_TriDiag);
        };

        namespace kernel
        {
            template <typename T>
            struct __tridiag
            {
                typedef typename real_of<T>::type scalar_type;

                // solve [a,b,c]*u = r, using auxiliary u
                static inline
                bool solve_simple(const array<T>  &a,
                                  const array<T>  &b,
                                  const array<T>  &c,
                                  array<T>        &g,
                                  array<T>        &u,
                                  const array<T>  &r
                                  )
                {
                    const size_t n = a.size();
                    assert(b.size()==n);
                    assert(c.size()==n);
                    assert(g.size()==n);
                    assert(u.size()>=n);
                    assert(r.size()>=n);
                    const scalar_type pmin = xnumeric<scalar_type>::minimum();
                    T piv = b[1];
                    if( Fabs( piv ) <= pmin )
                        return false;

                    u[1] = r[1] / piv;

                    //__________________________________________________________
                    //
                    // first pass
                    //__________________________________________________________

                    for( size_t j=2, jm=1; j <= n; ++j,++jm )
                    {
                        g[j] = c[jm] / piv;
                        piv  = b[j] - a[j] * g[j];
                        if( Fabs( piv ) <= pmin )
                            return false;
                        u[j] = (r[j] - a[j] * u[jm])/piv;
                    }

                    //__________________________________________________________
                    //
                    // second pass
                    //__________________________________________________________
                    for( size_t j=n-1, jp=n;j>0;--j,--jp)
                    {
                        assert(j+1==jp);
                        u[j] -= g[jp] * u[jp];
                    }

                    return true;
                }

                static inline
                bool solve_cyclic(const array<T>  &a,
                                  const array<T>  &b,
                                  const array<T>  &c,
                                  array<T>        &g,
                                  array<T>        &u,
                                  array<T>        &z,
                                  array<T>        &bb,
                                  array<T>        &x,
                                  const array<T>  &r
                                  )
                {
                    const size_t n      =  a.size(); assert(n>=2);
                    const T      alpha  =  a[1];
                    const T      beta   =  c[n];
                    const T      gamma  = -b[1];

                    //__________________________________________________________
                    //
                    // solve A.x = r
                    //__________________________________________________________
                    bb[1]=b[1]-gamma;
                    bb[n]=b[n]-(alpha*beta)/gamma;
                    for(size_t i=2;i<n;i++)
                        bb[i]=b[i];
                    if( ! solve_simple(a,bb,c,g,x,r) )
                        return false;

                    //__________________________________________________________
                    //
                    // compute the vector u
                    //__________________________________________________________
                    const T __zero = xnumeric<T>::zero();
                    const T __one  = xnumeric<T>::one();
                    u[1]=gamma;
                    u[n]=alpha;
                    for(size_t i=2;i<n;i++)
                        u[i]=__zero;

                    //__________________________________________________________
                    //
                    // solve A.z = u
                    //__________________________________________________________
                    if(! solve_simple(a,bb,c,g,z,u) )
                        return false;

                    //__________________________________________________________
                    //
                    // compute the factor
                    //__________________________________________________________
                    const T fact=
                    (x[1]+beta*x[n]/gamma)/
                    (__one+z[1]+beta*z[n]/gamma);
                    for(size_t i=n;i>0;--i)
                        x[i] -= fact*z[i];
                    return true;
                }


            };
        }


        //! simple tridiagonal matrix
        template <typename T>
        class TriDiag : public _TriDiag<T>
        {
        public:
            typedef typename _TriDiag<T>::array_type array_type;

            explicit TriDiag(size_t n) : _TriDiag<T>(0)
            {
                this->arrays.allocate(n);
            }

            virtual ~TriDiag() throw()
            {
            }

            virtual T operator()(size_t i,size_t j) const
            {
                assert(i>0);assert(i<=this->size());
                assert(j>0);assert(j<=this->size());

                const ptrdiff_t k = ptrdiff_t(i) - ptrdiff_t(j);
                switch( k )
                {
                    case -1: return this->c[i];
                    case  0: return this->b[i];
                    case  1: return this->a[i];

                    default:
                        break;
                }
                return xnumeric<T>::zero();
            }

            virtual bool __solve( array_type &x, const array_type &r)
            {
                return kernel::__tridiag<T>::solve_simple(this->a,this->b,this->c,this->g,x,r);
            }

            //! solve (this + U (x) V) * x = r
            inline bool ShermanMorrison( array_type &x, const array_type &U, const array_type &V, const array_type &r)
            {
                const T __one  = xnumeric<T>::one();
                const T __min  = xnumeric< typename real_of<T>::type >::minimum();
                array_type &Y  = this->xx;
                array_type &Z  = this->rr;
                if( !__tridiag(this->a, this->b, this->c, this->g, Z, U) ) return false;
                if( !__tridiag(this->a, this->b, this->c, this->g, Y, r) ) return false;
                const T num = tao::dot(V,Y);
                const T den = __one +  tao::dot(V,Z);
                if( Fabs(den) <= __min)
                    return false;
                const T fac = num / den;
                for(size_t i=x.size();i>0;--i)
                {
                    x[i] = Y[i] - fac * Z[i];
                }
                return true;
            }

            //! solve (this+U.Vt) x = r
            inline bool Woodbury( array_type &x, const matrix<T> &U, const matrix<T> &V, const array_type &r )
            {
                const T __one  = xnumeric<T>::one();

                assert(x.size()>=this->size());
                assert(r.size()>=this->size());
                assert(U.rows==this->size());
                assert(V.rows==this->size());
                assert(U.cols==V.cols);

                const size_t p = U.cols;
                //______________________________________________________________
                //
                //-- solve auxiliary problems
                //______________________________________________________________
                matrix<T> Z(U);
                if( ! solve(Z) )
                    return false;

                //______________________________________________________________
                //
                //-- compute the H matrix
                //______________________________________________________________
                matrix<T> H(p);
                tao::mmul_ltrn(H, V, Z);
                for(size_t i=p;i>0;--i) H[i][i] += __one;

                if( !LU<T>::build(H) )
                    return false;

                //______________________________________________________________
                //
                //-- solve the auxiliary problem A.y = r
                //______________________________________________________________
                array<T>  &y = this->xx;
                if( ! __solve(y,r) )
                    return false;

                //______________________________________________________________
                //
                //-- apply the woodbury formula
                //______________________________________________________________
                vector<T> tVy(p);
                tao::mul_trn(tVy, V, y);
                LU<T>::solve(H, tVy);
                tao::mul_sub(y, Z, tVy);
                return true;
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(TriDiag);
        };


        template <typename T>
        class cTriDiag : public _TriDiag<T>
        {
        public:
            typedef typename _TriDiag<T>::array_type array_type;

            explicit cTriDiag(size_t n) :
            _TriDiag<T>(3),
            u(  this->arrays.next_array() ),
            z(  this->arrays.next_array() ),
            bb( this->arrays.next_array() )
            {
                this->arrays.allocate(n);
            }

            virtual ~cTriDiag() throw()
            {
            }


            //! cyclic access
            /**
             M[1][n] = c[n]
             M[n][1] = a[1]
             */
            virtual T operator()(size_t i, size_t j) const
            {
                assert(i>0);assert(i<=this->size());
                assert(j>0);assert(j<=this->size());
                const size_t n = this->size();

                if(i==1&&j==n) return this->c[n];
                if(i==n&&j==1) return this->a[1];

                const ptrdiff_t k = ptrdiff_t(i) - ptrdiff_t(j);
                switch( k )
                {
                    case -1: return this->c[i];
                    case  0: return this->b[i];
                    case  1: return this->a[i];

                    default:
                        break;
                }
                return xnumeric<T>::zero();
            }

            virtual bool __solve( array_type &x, const array_type &r)
            {
                return kernel::__tridiag<T>::solve_cyclic(this->a,
                                                          this->b,
                                                          this->c,
                                                          this->g,
                                                          this->u,
                                                          this->z,
                                                          this->bb,
                                                          x,
                                                          r);
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(cTriDiag);
            array_type &u;
            array_type &z;
            array_type &bb;
        };
        
    }
    
}

#endif
