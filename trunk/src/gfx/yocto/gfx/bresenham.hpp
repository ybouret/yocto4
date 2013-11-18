#ifndef YOCTO_MATH_GL_BRESENHAM_INCLUDED
#define YOCTO_MATH_GL_BRESENHAM_INCLUDED 1

#include "yocto/functor.hpp"

namespace yocto
{
	
	namespace gfx
	{
		template <typename T>
		struct bresenham
		{
			YOCTO_ARGUMENTS_DECL_T;
			typedef functor<void,TL2(T,T)> callback_type;
			
			static inline
			void line2D(const_type x0, const_type y0, const_type x1, const_type y1, callback_type &callback)
			{
				// starting point of line
				type x = x0, y = y0;
				
				// direction of line
				type dx = x1-x0, dy = y1-y0;
				
				// increment or decrement depending on direction of line
				const_type sx = (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
				const_type sy = (dy > 0 ? 1 : (dy < 0 ? -1 : 0));
				
				// decision parameters for voxel selection
				if ( dx < 0 ) dx = -dx;
				if ( dy < 0 ) dy = -dy;
				type ax = 2*dx, ay = 2*dy;
				type decx, decy;
				
				// determine largest direction component, single-step related variable
				type max = dx, var = 0;
				if ( dy > max ) { var = 1; }
				
				// traverse Bresenham line
				switch ( var )
				{
					case 0:  // single-step in x-direction
						for (decy=ay-dx; /**/; x += sx, decy += ay)
						{
							// process pixel
							callback(x,y);
							
							// take Bresenham step
							if ( x == x1 ) break;
							if ( decy >= 0 ) { decy -= ax; y += sy; }
						}
						break;
					case 1:  // single-step in y-direction
						for (decx=ax-dy; /**/; y += sy, decx += ax)
						{
							// process pixel
							callback(x,y);
							
							// take Bresenham step
							if ( y == y1 ) break;
							if ( decx >= 0 ) { decx -= ay; x += sx; }
						}
						break;
				}
			}
			
			static inline
			void circle(const_type xc, const_type yc, const type r, callback_type &callback)
			{
				for( type x = 0, y = r, dec = 3-2*r; x <= y; x++)
				{
					callback(xc+x,yc+y);
					callback(xc+x,yc-y);
					callback(xc-x,yc+y);
					callback(xc-x,yc-y);
					callback(xc+y,yc+x);
					callback(xc+y,yc-x);
					callback(xc-y,yc+x);
					callback(xc-y,yc-x);
					
					if ( dec >= 0 )
						dec += -4*(y--)+4;
					dec += 4*x+6;
				}
			}
			
			static inline void _hline( const_type xlo, const_type y, const type xhi, callback_type &callback)
			{
				assert( xlo <= xhi );
				for( type x = xlo; x <= xhi; ++x ) callback(x,y);
			}
			
			static inline
			void disk( const_type xc, const_type yc, const type r, callback_type &callback)
			{
				for( type x = 0, y = r, dec = 3-2*r; x <= y; x++)
				{
					{
						const_type xlo=xc-x, xhi=xc+x;
						_hline(xlo,yc-y,xhi,callback);
						_hline(xlo,yc+y,xhi,callback);
					}
					
					{
						const_type xlo=xc-y, xhi=xc+y;
						_hline(xlo,yc-x,xhi,callback);
						_hline(xlo,yc+x,xhi,callback);
					}
					
					if ( dec >= 0 )
						dec += -4*(y--)+4;
					dec += 4*x+6;
				}
				
			}
			
		};
		
	}
}

#endif
