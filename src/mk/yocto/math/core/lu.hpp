#ifndef YOCTO_MATH_CORE_LU_INCLUDED
#define YOCTO_MATH_CORE_LU_INCLUDED 1


#include "yocto/math/matrix.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace math
    {

        //! work for: float,double, complex<float>, complex<double>, mpq
        template <typename T>
        struct LU
        {
            typedef matrix<T>                         matrix_t;
            typedef array<T>                          array_t;
            typedef typename matrix_t::mutable_type   type;
            typedef typename matrix_t::row            row_t;
            typedef typename real_of<T>::type         scalar_t;
            typedef lw_array<size_t>                  indices;
            typedef lw_array<scalar_t>                scalars;

            //! require a matrix with extra memory
            static inline
            bool build( matrix_t &a, bool *dneg=0)
            {
                assert(a.cols>0);
                assert(a.rows>0);
                assert(a.is_square());
                assert(a.memory_kind==matrix_large_memory);

                const scalar_t  __zero = xnumeric<scalar_t>::zero();
                const scalar_t  __one  = xnumeric<scalar_t>::one();
                const size_t    n      = a.rows;
                indices         indx( a.get_indices(), n);
                scalars         scal( a.get_scalars(), n);
                if(dneg) *dneg = false;

                //______________________________________________________________
                //
                // initializing scaling/pivot
                //______________________________________________________________
                const scalar_t amin = xnumeric<scalar_t>::minimum();
                for( size_t i=n;i>0;--i)
                {
                    const row_t & a_i = a[i];
                    scalar_t      piv = __zero;

                    for( size_t j=n;j>0;--j)
                    {
                        const scalar_t tmp = Fabs( a_i[j] );
                        if (tmp>piv)
                        {
                            piv = tmp;
                        }
                    }
                    if( piv <= amin )
                    {
                        return false;
                    }
                    scal[i] = __one/piv;
                }


                //______________________________________________________________
                //
                // Crout's algorithm
                //______________________________________________________________
                for(size_t j=1;j<=n;++j)
                {
                    for(size_t i=1;i<j;++i)
                    {
                        row_t &a_i = a[i];
                        type   sum = a_i[j];
                        for(size_t k=1;k<i;++k)
                        {
                            sum -= a_i[k]*a[k][j];
                        }
                        a_i[j]=sum;
                    }

                    scalar_t piv  = __zero;
                    size_t   imax = j;
                    for( size_t i=j;i<=n;++i)
                    {
                        row_t &a_i = a[i];
                        type   sum = a_i[j];
                        for(size_t k=1;k<j;++k)
                        {
                            sum -= a_i[k]*a[k][j];
                        }
                        a_i[j]=sum;

                        const scalar_t tmp = scal[i]*Fabs(sum);
                        if( tmp >= piv )
                        {
                            piv  = tmp;
                            imax = i;
                        }
                    }


                    assert( imax> 0 );
                    if (j != imax)
                    {
                        a.swap_rows( j, imax );
                        if(dneg)
                        {*dneg = ! *dneg;
                        }
                        scal[imax]=scal[j];
                    }

                    indx[j]=imax;

                    if( Fabs(a[j][j]) <= amin )
                    {
                        return false;
                    }

                    if (j != n)
                    {
                        const type fac = __one/(a[j][j]);
                        for(size_t i=j+1;i<=n;++i)
                        {
                            a[i][j] *= fac;
                        }
                    }
                }

#if !defined(NDEBUG)
                for( size_t i=1; i <= n; ++i )
                {
                    assert( indx[i] >  0 );
                    assert( indx[i] <= n );
                }
#endif
                return true;
                
            }


            //! solve a vector using a previously LU build matrix
            static inline
            void solve( const matrix_t &a, array_t &b ) throw()
            {
                assert(a.cols>0);
                assert(a.rows>0);
                assert(a.is_square());
                assert(b.size()>=a.rows);
                assert(a.memory_kind==matrix_large_memory);

                const size_t    n = a.rows;
                
                //______________________________________________________________
                //
                // first pass
                //______________________________________________________________
                {
                    const indices   indx( a.get_indices(), n);
                    for(size_t i=1;i<=n;++i)
                    {
                        const size_t ip  = indx[i]; assert(ip>0);assert(ip<=n);
                        type         sum = b[ip];
                        const row_t &a_i = a[i];
                        b[ip] = b[i];
                        for(size_t j=1;j<i;++j)
                        {
                            sum -= a_i[j] * b[j];
                        }
                        b[i] = sum;
                    }
                }

                //______________________________________________________________
                //
                // second pass
                //______________________________________________________________
                {
                    for(size_t i=n;i>0;--i)
                    {
                        const row_t &a_i = a[i];
                        type         sum = b[i];
                        for(size_t j=i+1;j<=n;++j)
                        {
                            sum -= a_i[j]*b[j];
                        }
                        b[i]=sum/a_i[i];
                    }

                }
            }

            //! solve columns of a matrix
            static inline
            void solve( const matrix_t &a, matrix_t &b ) throw()
            {
                assert(a.cols>0);
                assert(a.rows>0);
                assert(a.is_square());
                assert(b.rows==a.rows);
                assert(a.memory_kind==matrix_large_memory);

                const size_t    n  = a.rows;
                const size_t    nc = b.cols;
                const indices   indx( a.get_indices(), n);
                for(size_t c=nc;c>0;--c)
                {
                    //__________________________________________________________
                    //
                    // first pass
                    //__________________________________________________________
                    for(size_t i=1;i<=n;++i)
                    {
                        const size_t ip  = indx[i]; assert(ip>0);assert(ip<=n);
                        type         sum = b[ip][c];
                        const row_t &a_i = a[i];
                        b[ip][c] = b[i][c];
                        for(size_t j=1;j<i;++j)
                        {
                            sum -= a_i[j] * b[j][c];
                        }
                        b[i][c] = sum;
                    }

                    //__________________________________________________________
                    //
                    // second pass
                    //__________________________________________________________
                    for(size_t i=n;i>0;--i)
                    {
                        const row_t &a_i = a[i];
                        type         sum = b[i][c];
                        for(size_t j=i+1;j<=n;++j)
                        {
                            sum -= a_i[j]*b[j][c];
                        }
                        b[i][c]=sum/a_i[i];
                    }

                }
            }

            //! solve x = inv(a)*b, using a previously LU decomposed matrix
            static inline
            void solve( array_t &x, const matrix_t &a, const array_t &b ) throw()
            {
                assert(a.cols>0);
                assert(a.rows>0);
                assert(a.is_square());
                assert(b.size()>=a.rows);
                assert(x.size()>=a.rows);
                assert(a.memory_kind==matrix_large_memory);
#define YOCTO_LU_CPY(I) x[I] = b[I]
                YOCTO_LOOP_FUNC(a.rows,YOCTO_LU_CPY,1);
#undef  YOCTO_LU_CPY
                LU<T>::solve(a,b);
            }
            
        };
        
    }
}

#endif
