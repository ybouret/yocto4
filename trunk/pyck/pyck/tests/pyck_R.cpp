#include "../os.hpp"
#include "../R++.hpp"

extern "C"
SEXP dumpVec( SEXP args ) throw()
{
    try
    {
        const RVector<double> v(args);
        Rprintf("\t#items=%u\n", unsigned(v.size));
        for(size_t i=0; i < v.size;++i)
        {
            Rprintf("\tv[%2u] = %g\n",unsigned(i), v[i]);
        }
        return R_NilValue;
    }
    catch(...)
    {
        Rprintf("an error occured!\n");
        return R_NilValue;
    }
}

extern "C"
SEXP sumVec( SEXP args ) throw()
{
    try
    {
        const RVector<double> v(args);
        RVector<double>       ans(1);
        double sum = 0;
        for( size_t i=0; i < v.size;++i)
        {
            sum += v[i];
        }
        ans[0] = sum;
        
        return *ans;
    }
    catch(...)
    {
        Rprintf("an error occured!\n");
        return R_NilValue;
    }
}

extern "C"
SEXP dumpMat( SEXP args ) throw()
{
    try
    {
        const RMatrix<double> m(args);
        Rprintf("Matrix %ux%u\n", unsigned(m.rows), unsigned(m.cols));
        for(size_t i=0; i < m.rows; ++i )
        {
            for(size_t j=0; j < m.cols; ++j)
            {
                Rprintf(" %g", m[j][i]);
            }
            Rprintf("\n");
        }
        
        CMatrix<double> M(m.rows,m.cols);
        for(size_t i=0; i < m.rows; ++i )
        {
            for(size_t j=0; j < m.cols; ++j )
            {
                M[i][j] = m[j][i];
            }
        }
        Rprintf("M=\n");
        M.print();
        
        CMatrix<double> Q(args);
        Rprintf("Q=\n");
        Q.print();

        RMatrix<double> q( m.rows, m.cols );
        memcpy( &q[0][0], &m[0][0], sizeof(double) * q.items );
        
        return *q;
    }
    catch(...)
    {
        return R_NilValue;
    }
}

extern "C"
SEXP getList() throw()
{
    try
    {
        const char  *names[] = { "A", "B" };
        const size_t count   = sizeof(names)/sizeof(names[0]);
        RList L(names,count);
        RMatrix<double> q(2,3);
        L.set(0,q);
        memset( &q[0][0], 0, sizeof(double) * q.items );
        RVector<int> v(1);
        v[0] = 7;
        L.set(1,v);
        return *L;
    }
    catch(...)
    {
        return R_NilValue;
    }
}

