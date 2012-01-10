#ifndef YOCTO_CLIFF_IN3D_INCLUDED
#define YOCTO_CLIFF_IN3D_INCLUDED 1

#include "yocto/cliff/layout.hpp"
#include "yocto/geom/v3d.hpp"

namespace yocto
{
	
	namespace cliff
	{
		typedef geom::v3d<unit_t>    coord3D;
		typedef layout<coord3D>      layout3D;
		template <typename U> struct vertex3D { typedef geom::v3d<U> type;       };
		template <typename U> struct region3D { typedef region<U,vertex3D> type; };
		
		template <typename U>
		class triangle3D
		{
		public:
			typedef typename vertex3D<U>::type vertex_t;
				
			vertex_t p0, p1, p2;
			inline triangle3D() throw() : p0(), p1(), p2() {}
			inline triangle3D( const triangle3D &f ) throw() : p0(f.p0), p1(f.p1), p2(f.p2) {}
			inline triangle3D & operator=( const triangle3D &f ) throw() 
			{
				p0 = f.p0; p1 = f.p1; p2 = f.p2;
				return *this;
			}
			
			inline ~triangle3D() throw() {}
			
			inline vertex_t       & operator[](size_t i) throw()       { assert(i<3); return *(&p0 + i); }
			inline const vertex_t & operator[](size_t i) const throw() { assert(i<3); return *(&p0 + i); }
		};
		
	}
}

#endif
