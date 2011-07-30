#include "yocto/utest/run.hpp"

#include "yocto/spade/workspace1d.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace spade;


static inline int get_rand()
{
    return 100 - int( 200 * alea<double>() );
}

template <typename LAYOUT>
static inline void displayLayout( const LAYOUT &L )
{
    std::cerr << "Layout" << L.dims << "D: ";
    std::cerr << L.lower << " -> " << L.upper << " : " << L.width << std::endl;
    std::cerr << "\titems=" << L.items << std::endl;
}

namespace
{

    template <typename T>
    void perform1D()
    {
        for( size_t k=0; k < 256; ++k ) {
            const layout1D         L( get_rand(), get_rand() );
            const composition      C(1,8);


            workspace1D<T> w(C,L);

            displayLayout( w );

            for( unit_t i= w.index_min; i <= w.index_max; ++i ) {
                array1D<T> &A = w[i];
                A.ld( T(i) );
                for( unit_t x = A.lower; x <= A.upper; ++x ) {
                    A[x] = T(x);
                }
            }

        }

    }


}

YOCTO_UNIT_TEST_IMPL(w1D)
{
    perform1D< float > ();
    perform1D< double > ();
    perform1D< math::complex<float> > ();
    perform1D< math::complex<double> > ();


}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/workspace2d.hpp"

namespace
{

    template <typename T>
    void perform2D()
    {
        for( size_t k=0; k < 32; ++k ) {
            const coord2D  lo( get_rand(), get_rand() );
            const coord2D  hi( get_rand(), get_rand() );

            const layout2D         L( lo, hi );
            const composition      C(1,5);


            workspace2D<T> w(C,L);

            displayLayout( w );
            for( unit_t i= w.index_min; i <= w.index_max; ++i ) {
                array2D<T> &A = w[i];
                A.ld( T(i) );
                for( unit_t x = A.lower.x; x <= A.upper.x; ++x ) {
                    for( unit_t y = A.lower.y; y <= A.upper.y; ++y ) {
                        A[y][x] = T(x+y);
                    }
                }
            }

        }

    }


}

YOCTO_UNIT_TEST_IMPL(w2D)
{
    perform2D< float > ();
    perform2D< double > ();
    perform2D< math::complex<float> > ();
    perform2D< math::complex<double> > ();


}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/workspace3d.hpp"

namespace
{

    template <typename T>
    void perform3D()
    {
        for( size_t k=0; k < 4; ++k ) {
            const coord3D  lo( get_rand(), get_rand(), get_rand() );
            const coord3D  hi( get_rand(), get_rand(), get_rand() );

            const layout3D         L( lo, hi );
            const composition      C(1,5);


            workspace3D<T> w(C,L);

            displayLayout( w );

            for( unit_t i= w.index_min; i <= w.index_max; ++i ) {
                array3D<T> &A = w[i];
                A.ld( T(i) );

                for( unit_t x = A.lower.x; x <= A.upper.x; ++x ) {
                    for( unit_t y = A.lower.y; y <= A.upper.y; ++y ) {
                        for( unit_t z = A.lower.z; z <= A.upper.z; ++z ) {
                            const coord3D p(x,y,z);
                            assert( A.has( p ) );
                            A[z][y][x] = T(x+y+z);
                        }
                    }
                }

            }

        }
    }

}

YOCTO_UNIT_TEST_IMPL(w3D)
{

    perform3D< float > ();
    perform3D< double > ();
    perform3D< math::complex<float> > ();
    perform3D< math::complex<double> > ();

}
YOCTO_UNIT_TEST_DONE()

