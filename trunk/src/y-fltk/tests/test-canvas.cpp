#include "yocto/utest/run.hpp"
#include "canvas.h"
#include "yocto/ptr/auto.hpp"
#include "yocto/math/types.hpp"

using namespace yocto;
using namespace FLTK;

YOCTO_UNIT_TEST_IMPL(canvas)
{
    auto_ptr<Fl_Window> win( MakeCanvasWindow() );
    
    Curve &crv = Ca->curves["data"];
    
    Ca->xaxis.set_range(-1, 1);
    Ca->yaxis.set_range(-1, 1);
    
    for( size_t i=0; i <= 100; ++i )
    {
        const double angle = (20.0*i)/100;
        const double radius = 0.1 + 0.1 * angle;
        crv.push_back( Point(radius*sin(angle),radius*cos(angle) ) );
    }
    
    win->show(argc,argv);

    (void)Fl::run();
}
YOCTO_UNIT_TEST_DONE()
