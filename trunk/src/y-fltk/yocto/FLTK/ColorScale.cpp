
#include "yocto/FLTK/ColorScale.hpp"
#include "yocto/math/dat/linear.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{

	namespace FLTK 
	{
		HColorScale:: HColorScale( int X, int Y, int W, int H, const char *l ) :
	Fl_Box(X,Y,W,H,l),
		xaxis(),
		yaxis(),
		data(),
		color1(FL_RED),
		color2(FL_BLUE)
	{
	}

	HColorScale:: ~HColorScale() throw() {}


	void HColorScale:: draw()
	{
		if( data.size() <= 0 )
		{
			Fl_Box::draw();
		}
		else 
		{
			Fl_Box::draw();

			const int    sx0  = x();
			const int    sw   = w() - 1;
			const int    sy0  = y();
			const int    sy1  = y() + h() - 1;
			for( int xs = 0; xs <= sw; ++xs )
			{
				const double user_x = xaxis.vmin  + (xs*xaxis.length)/sw;
				const double w      = clamp<double>(0,math::linear<double>(user_x,data),1);
				fl_color( fl_color_average(color1, color2, float(w)) );
				fl_yxline(sx0+xs, sy0, sy1);
			}

		}
	}

	}

}


