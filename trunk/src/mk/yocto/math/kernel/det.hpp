#ifndef YOCTO_DET_INCLUDED
#define YOCTO_DET_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sort/quick.hpp"

namespace yocto
{
    namespace math
    {
        
        //! M is destroyed on ouput
        /**
         T: float, double, complex<>
         */
        template <typename T>
        inline T __determinant_of( matrix<T> &M ) throw()
        {
            assert(M.is_square());
            assert(M.rows>0);
            
            T       s(1);
            const size_t n = M.rows;
            for(size_t i=1,i1=i+1;i<=n;++i,++i1)
            {
                //______________________________________________________________
                //
                // find max pivot
                // if i==n, just check no singular matrix
                //______________________________________________________________
                typename real_of<T>::type piv = Fabs(M[i][i]);
                size_t                    ipv = i;
                for(size_t k=i1;k<=n;++k)
                {
                    const typename real_of<T>::type tmp = Fabs(M[k][i]);
                    if( tmp > piv )
                    {
                        piv = tmp;
                        ipv = k;
                    }
                }
                if(piv<=0)
                    return 0; // singular matrix
                
                //______________________________________________________________
                //
                // set the pivot row
                //______________________________________________________________
                if(ipv!=i)
                {
                    s=-s;
                    M.swap_rows(ipv,i);
                }
                const T pivot(M[i][i]);
                for(size_t k=i1;k<=n;++k)
                {
                    const T coef(M[k][i]);
                    M[k][i] = T(0);
                    for(size_t j=i1;j<=n;++j)
                    {
                        M[k][j] -= (M[i][j]*coef)/pivot;
                    }
                }
            }
            
            //__________________________________________________________________
            //
            // Compute determinant
            //__________________________________________________________________
            T d(1);
            for(size_t i=n;i>0;--i)
            {
                d *= M[i][i];
            }
            
            return s*d;
        }
        
        
        
        //! M is not destroyed on output
        template <typename T>
        inline T determinant_of( const matrix<T> &M )
        {
            matrix<T> MM(M);
            return __determinant_of(MM);
        }
        
        template <>
        inline ptrdiff_t determinant_of<ptrdiff_t>( const matrix<ptrdiff_t> &M )
        {
            matrix<double> MM(M.rows,M.cols);
            for(size_t i=M.rows;i>0;--i)
            {
                for(size_t j=M.cols;j>0;--j)
                {
                    MM[i][j] = M[i][j];
                }
            }
            std::cerr << "ideterminant" << std::endl;
            return Rint( __determinant_of(MM) );
        }
        
        
        template <typename T,typename U>
        inline T cofactor_of(const matrix<T> &M,
                             const size_t     I,
                             const size_t     J,
                             matrix<U>       &m )
        {
            assert(M.is_square());
            assert(M.rows>=1);
            assert(m.rows==M.rows-1);
            if(M.rows>1)
            {
                m.template minor_of(M,I,J);
                const T ddet(__determinant_of(m));
                const T mdet = ( ((I+J)&1) != 0 ) ? -ddet : ddet;
                return mdet;
            }
            else
            {
                return M[1][1];
            }
        }
        
        template <>
        inline ptrdiff_t cofactor_of(const matrix<ptrdiff_t> &M,
                                     const size_t             I,
                                     const size_t             J,
                                     matrix<double>          &m)
        {
            assert(M.is_square());
            assert(M.rows>=1);
            assert(m.rows==M.rows-1);
            if(M.rows>1)
            {
                m.template minor_of(M,I,J);
                const ptrdiff_t ddet(RInt(__determinant_of(m)));
                const ptrdiff_t mdet = ( ((I+J)&1) != 0 ) ? -ddet : ddet;
                return mdet;
            }
            else
            {
                return M[1][1];
            }
        }
        
        
        
        
        template <typename T>
        inline void adjoint( matrix<T> &A, const matrix<T> &M )
        {
            assert(M.is_square());
            assert(A.is_square());
            assert(M.rows==A.rows);
            switch(M.rows)
            {
                case 0:
                    return;
                    
                case 1:
                    A[1][1] = T(1);
                    return;
                    
                default: {
                    const size_t   n  = M.rows;
                    const size_t   n1 = n-1;
                    matrix<T>      m(n1,n1);
                    for(size_t i=n;i>0;--i)
                    {
                        for(size_t j=n;j>0;--j)
                        {
                            A[i][j] = cofactor_of(M, j, i, m);
                        }
                    }
                    
                }
            }
        }
        
        template <>
        inline void adjoint<ptrdiff_t>( matrix<ptrdiff_t> &A, const matrix<ptrdiff_t> &M )
        {
            std::cerr << "iadjoint" << std::endl;
            assert(M.is_square());
            assert(A.is_square());
            assert(M.rows==A.rows);
            switch(M.rows)
            {
                case 0:
                    return;
                    
                case 1:
                    A[1][1] = 1;
                    return;
                    
                default: {
                    const size_t   n  = M.rows;
                    const size_t   n1 = n-1;
                    matrix<double> m(n1,n1);
                    for(size_t i=n;i>0;--i)
                    {
                        for(size_t j=n;j>0;--j)
                        {
                            A[i][j] = cofactor_of(M, j, i, m);
                        }
                    }
                    
                }
            }
        }

        
        
        
        
        //! linear improvement
        /**
         assuming that
         we solved M.x = b using x = (A*b)/detM
         \param x a trial solution
         \param M the original matrix
         \param A the adjoint matrix
         \param D the determinant of M
         \param b the right hand argument
         */
        template <typename T>
        inline void improve(array<T>        &x,
                            const matrix<T> &M,
                            const matrix<T> &A,
                            const T          detM,
                            const array<T>   &b ) throw()
        {
            typedef typename real_of<T>::type real_t;
            assert(M.is_square());
            assert(A.is_square());
            assert(M.rows==A.rows);
            const size_t n = M.rows;
            lw_array<T>  r(M.scal,n);
            lw_array<T>  y(A.scal,n);
            lw_array<T>  s(M.template get_aux<T>(),n);
            
            //__________________________________________________________________
            //
            // compute the initial residual
            //__________________________________________________________________
            real_t old_rsq = 0;
            for(size_t i=n;i>0;--i)
            {
                T tmp = numeric<T>::zero;
                for(size_t j=n;j>0;--j)
                {
                    tmp += M[i][j] * x[j];
                }
                const real_t d = Fabs( (r[i]=tmp-b[i]));
                old_rsq += d * d;
            }
            //std::cerr << "old_rsq=" << old_rsq << std::endl;
            
            while(true)
            {
                //______________________________________________________________
                //
                // solve the residual, save x in y and update x
                //______________________________________________________________
                for(size_t i=n;i>0;--i)
                {
                    for(size_t j=n;j>0;--j)
                    {
                        s[j] = A[i][j] * r[j];
                    }
                    quicksort(s,CompareModules<T>);
                    T sum = numeric<T>::zero;
                    for(size_t j=1;j<=n;++j)
                    {
                        sum += s[j];
                    }
                    y[i]  = x[i];
                    x[i] -= sum/detM;
                }
                
                //______________________________________________________________
                //
                // compute the new residual
                //______________________________________________________________
                real_t new_rsq = 0;
                for(size_t i=n;i>0;--i)
                {
                    T tmp = numeric<T>::zero;
                    for(size_t j=n;j>0;--j)
                    {
                        tmp += M[i][j] * x[j];
                    }
                    const real_t d = Fabs( (r[i]=tmp-b[i]));
                    new_rsq += d * d;
                }
                
                //std::cerr << "new_rsq=" << new_rsq << std::endl;
                if(new_rsq>=old_rsq)
                {
                    for(size_t i=n;i>0;--i)
                    {
                        x[i] = y[i];
                    }
                    return;
                }
                
                old_rsq = new_rsq;
                
            }
        }
        
        
    }
}

#endif

