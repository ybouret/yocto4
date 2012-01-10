#include "yocto/cliff/rwops.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
	namespace cliff
	{
		using math::real_t;
		
		template <>
		void rwops<real_t> :: save_vtk(const string          &filename,
									   const string          &name,
									   const array3D<real_t> &field,
									   const layout3D        &sub,
									   const vertex3D_t      &origin,
									   const vertex3D_t      &spacing )
		{
		
			assert( field.has(sub.lower) );
			assert( field.has(sub.upper) );
			ios::ocstream fp( filename, false );
			fp("# vtk DataFile Version 3.0\n");
			fp("# commet\n");
			fp("ASCII\n");
			fp("DATASET STRUCTURED_POINTS\n");
			fp("DIMENSIONS %u %u %u\n", unsigned(sub.width.x), unsigned(sub.width.y), unsigned(sub.width.z) );
			fp("ORIGIN %g %g %g\n", origin.x, origin.y, origin.z );
			fp("SPACING %g %g %g\n", spacing.x, spacing.y, spacing.z );
			fp("POINT_DATA %u\n", unsigned(sub.items) );
			fp("SCALARS %s float\n", name.c_str());
			fp("LOOKUP_TABLE default\n");
			for( unit_t x=sub.lower.x; x <= sub.upper.x; ++x )
			{
				for( unit_t y=sub.lower.y; y <= sub.upper.y; ++y )
				{
					for( unit_t z=sub.lower.z; z <= sub.upper.z; ++z )
					{
						fp("%g\n", field[z][y][x] );
					}
				}
			}
		}
		
	}
}
