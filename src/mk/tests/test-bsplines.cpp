#include "yocto/math/dat/b-splines.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand32.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(bsplines)
{

    typedef v2d<double> vtx_t;

    size_t m=4;
    vector<vtx_t>  P(m,as_capacity);
    vector<double> t(m,as_capacity);

    t.push_back(0);
    P.push_back(vtx_t(0,0));

    for(size_t i=1;i<m;++i)
    {
        const double tt = t.back() + 1;
        const vtx_t  pp( tt, alea<double>());

        t.push_back(tt);
        P.push_back(pp);
    }

    const double tmax = t.back();
    std::cerr << "0 -> " << tmax << std::endl;

    {
        ios::ocstream fp("data.dat",false);
        for(size_t i=1;i<=t.size();++i)
        {
            fp("%g %g\n", P[i].x, P[i].y);
        }
    }



    {
        ios::ocstream fp("bspl.dat",false);
        size_t N = 300;
        for(size_t i=0;i<=N;++i)
        {
            const double tt = (tmax*i)/N;
            const vtx_t v = Cubic_Bsplines(tt, t, P);
            fp("%g %g\n",v.x,v.y);
        }
    }

}
YOCTO_UNIT_TEST_DONE()


namespace {
#if 1

    /// the points of the curve - these are the same as the bezier curve
    /// points demonstrated in the bezier curve example.
    static float Points[4][3] = {
        { 10,10,0 },
        {  5,10,2 },
        { -5,0,0 },
        {-10,5,-2}
    };
#define NUM_POINTS 4

    //	The following sets of 4 indices are the curves that need to
    //	be drawn to create a clamped cubic b-spline. In total there
    //	are 5 curve segments to draw.
    //
    //	0 0 0 1
    //    0 0 1 2
    //      0 1 2 3
    //        1 2 3 3
    //          2 3 3 3
    //
    //	Remember this when trying to understand knot vectors!!
    //

#else

    /// the points of the curve - these are the same as the bezier curve
    /// points demonstrated in the bezier curve example.
    static float Points[7][3] = {
        { 10,10,0 },
        {  5,10,2 },
        { -5,5,0 },
        {-10,5,-2},
        {-4,10,0},
        {-4,5,2},
        {-8,1,0}
    };
#define NUM_POINTS 7

    //	The following sets of 4 indices are the curve segments that need to
    //	be drawn to create a clamped cubic b-spline. In total there
    //	are 8 curve segments to draw.
    //
    //	0 0 0 1
    //    0 0 1 2
    //      0 1 2 3
    //        1 2 3 4
    //          2 3 4 5
    //            3 4 5 6
    //              4 5 6 6
    //                5 6 6 6
    //
    //	Remember this when trying to understand knot vectors!!
    //

#endif
    //------------------------------------------------------------	GetPoint()
    // To handle the clamped curves i imagine the curve to have 3
    // extra control points at the beginning of the curve, and 3 extra
    // at the end. To simply the iteration of the points, i start
    // the indices at -3. Obviously if i use that value as an array
    // index it will crash horribly. Instead i use a function that
    // returns either the first, or last point if the index requested
    // is out of range. This basically simplifies our curve calculation
    // greatly!!
    //
    static inline
    float* GetPoint(int i) throw() {

        // return 1st point
        if (i<0)
        {
            return	Points[0];
        }

        // return last point
        if (i<NUM_POINTS)
        {
            return Points[i];
        }

        return Points[NUM_POINTS-1];
    }


}

#define NUM_SEGMENTS (NUM_POINTS+1)


YOCTO_UNIT_TEST_IMPL(bspl)
{

    {
        ios::ocstream fp("points.dat",false);
        for(size_t i=0;i<NUM_POINTS;++i)
        {
            fp("%g %g\n", Points[i][0], Points[i][1]);
        }
    }

    {
        ios::ocstream fp("bspl.dat",false);
        int LOD = 5;
        for(int start_cv=-3,j=0;j!=NUM_SEGMENTS;++j,++start_cv)
        {
            //glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
            // for each section of curve, draw LOD number of divisions
            for(int i=0;i!=LOD;++i) {

                // use the parametric time value 0 to 1 for this curve
                // segment.
                float t = float(i)/LOD;

                // the t value inverted
                float it = 1.0f-t;

                // calculate blending functions for cubic bspline
                float b0 = it*it*it/6.0f;
                float b1 = (3*t*t*t - 6*t*t +4)/6.0f;
                float b2 = (-3*t*t*t +3*t*t + 3*t + 1)/6.0f;
                float b3 =  t*t*t/6.0f;

                if(0==j)
                {
                    std::cerr << "t=" << t
                    << ", b0= " << b0
                    << ", b1= " << b1
                    << ", b2= " << b2
                    << ", b3= " << b3
                    << std::endl;
                    std::cerr << "points #" << start_cv + 0 << " -> " << start_cv + 3 << std::endl;
                }

                // calculate the x,y and z of the curve point
                float x =
                b0 * GetPoint( start_cv + 0 )[0] +
                b1 * GetPoint( start_cv + 1 )[0] +
                b2 * GetPoint( start_cv + 2 )[0] +
                b3 * GetPoint( start_cv + 3 )[0] ;

                float y =
                b0 * GetPoint( start_cv + 0 )[1] +
                b1 * GetPoint( start_cv + 1 )[1] +
                b2 * GetPoint( start_cv + 2 )[1] +
                b3 * GetPoint( start_cv + 3 )[1] ;

                float z =
                b0 * GetPoint( start_cv + 0 )[2] +
                b1 * GetPoint( start_cv + 1 )[2] +
                b2 * GetPoint( start_cv + 2 )[2] +
                b3 * GetPoint( start_cv + 3 )[2] ;
                
                // specify the points
                fp("%g %g %g\n", x,y,z);
            }
        }
        // and the last point
        fp("%g %g %g\n",Points[NUM_POINTS-1][0],Points[NUM_POINTS-1][1],Points[NUM_POINTS-1][2] );
    }
    
}
YOCTO_UNIT_TEST_DONE()

