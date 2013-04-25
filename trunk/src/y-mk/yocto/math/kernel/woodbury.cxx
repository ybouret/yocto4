#include "yocto/math/kernel/woodbury.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace  {
            static inline
            void __finalize( matrix<z_type> &B )
            {
                assert(B.is_square());
                const size_t n = B.rows;
                for(size_t i=n;i>0;--i)
                {
                    for(size_t j=n;j>i;--j)
                    {
                        B[i][j] = -B[i][j];
                    }
                    B[i][i] = numeric<z_type>::one - B[i][i];
                    for(size_t j=i-1;j>0;--j)
                    {
                        B[i][j] = -B[i][j];
                    }
                }
                
            }
            
            static inline
            bool __compute_B( matrix<z_type> &B, const matrix<z_type> &Z, matrix<z_type> &tV )
            {
                assert(Z.cols == tV.rows);
                assert(Z.rows == B.rows );
                const size_t p = Z.cols;
                
                //--------------------------------------------------------------
                // compute H = (Id + Z*tV)^(-1)
                //--------------------------------------------------------------
                matrix<z_type> H(p,p);
                algebra<z_type>::mul(H, Z, tV);
                for(size_t i=p;i>0;--i)
                    H[i][i] += numeric<z_type>::one;
                lu<z_type>     L(p);
                if( !L.build(H) )
                {
                    // singular system
                    return false;
                }
                
                //--------------------------------------------------------------
                // compute Z*H*tV
                //--------------------------------------------------------------
                L.solve(H,tV);
                algebra<z_type>::mul(B, Z, tV);
                
                //--------------------------------------------------------------
                // B = Id - B
                //--------------------------------------------------------------
                __finalize(B);
                return true;
            }
        }
        
        
        
        template <>
        bool woodbury<z_type>:: build(matrix<z_type>       &B,
                                      const lu<z_type>     &LU,
                                      const matrix<z_type> &A,
                                      const matrix<z_type> &U,
                                      const matrix<z_type> &V)
        {
            assert(A.rows>0);
            assert(A.cols==A.rows);
            assert(U.rows == A.rows );
            assert(V.rows == A.rows );
            assert(V.cols == U.cols);
            assert(U.cols>0);
            
            const size_t n = A.rows;
            
            B.make(n,n);
            matrix<z_type> Z(U);
            matrix<z_type> tV(V,matrix_transpose);
            
            //-- solve auxiliary problems
            LU.solve(A,Z);
            
            //-- compute the B matrix
            return __compute_B(B, Z, tV);
            
        }
        
        template <>
        bool woodbury<z_type>:: build(matrix<z_type>        &B,
                                      const xtridiag<z_type> &A,
                                      const matrix<z_type>  &U,
                                      const matrix<z_type>  &V)
        {
            assert(A.size()>0);
            assert(U.rows == A.size() );
            assert(V.rows == A.size() );
            assert(U.cols == V.cols );
            assert(U.cols>0);
            
            const size_t n = A.size();
            B.make(n,n);
            
            matrix<z_type> Z(U);
            matrix<z_type> tV(V,matrix_transpose);
            
            
            //-- solve the auxiliary problems
            {
                matrix<z_type> tV0( tV );
                //A.solve(tV, tV0);
            }
            
            //-- compute the B matrix
            return __compute_B(B, Z, tV);

        }
        
    }
    
}
