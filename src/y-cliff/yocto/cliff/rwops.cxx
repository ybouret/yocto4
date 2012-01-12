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
									   const string          &title,
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
			fp("%s\n", title.c_str());
			fp("ASCII\n");
			fp("DATASET STRUCTURED_POINTS\n");
			fp("DIMENSIONS %u %u %u\n", unsigned(sub.width.x), unsigned(sub.width.y), unsigned(sub.width.z) );
			fp("ORIGIN %g %g %g\n", origin.x, origin.y, origin.z );
			fp("SPACING %g %g %g\n", spacing.x, spacing.y, spacing.z );
			fp("POINT_DATA %u\n", unsigned(sub.items) );
			fp("SCALARS %s float\n", name.c_str());
			fp("LOOKUP_TABLE default\n");
			
			for( unit_t z=sub.lower.z; z <= sub.upper.z; ++z )
			{
				for( unit_t y=sub.lower.y; y <= sub.upper.y; ++y )
				{
					for( unit_t x=sub.lower.x; x <= sub.upper.x; ++x )
					{
						fp("%g\n", field[z][y][x] );
					}
				}
			}
		}
		
		template <>
		void rwops<real_t> :: save_vtk(const string          &filename,
									   const string          &title,
									   const string          &name,
									   const array2D<real_t> &field,
									   const layout2D        &sub,
									   const vertex2D_t      &origin,
									   const vertex2D_t      &spacing )
		{
			
			assert( field.has(sub.lower) );
			assert( field.has(sub.upper) );
			ios::ocstream fp( filename, false );
			fp("# vtk DataFile Version 3.0\n");
			fp("%s\n", title.c_str());
			fp("ASCII\n");
			fp("DATASET STRUCTURED_POINTS\n");
			fp("DIMENSIONS %u %u 1\n", unsigned(sub.width.x), unsigned(sub.width.y) );
			fp("ORIGIN %g %g 0\n", origin.x, origin.y);
			fp("SPACING %g %g 0\n", spacing.x, spacing.y);
			fp("POINT_DATA %u\n", unsigned(sub.items) );
			fp("SCALARS %s float\n", name.c_str());
			fp("LOOKUP_TABLE default\n");
			
			
			for( unit_t y=sub.lower.y; y <= sub.upper.y; ++y )
			{
				for( unit_t x=sub.lower.x; x <= sub.upper.x; ++x )
				{
					fp("%g\n", field[y][x] );
				}
			}
		}
		
		
		template <>
		void rwops<real_t> ::save_vtk( const string &filename, const string &title, const array<triangle3D_t> &triangles)
		{
			const unsigned nt = triangles.size();
			ios::ocstream fp( filename, false );
			fp("# vtk DataFile Version 3.0\n");
			fp("%s\n", title.c_str());
			fp("ASCII\n");
			fp("DATASET POLYDATA\n");
			fp("POINTS %u float\n", nt * 3 );
			for(size_t i=1; i <= nt; ++i )
			{
				const triangle3D_t &t = triangles[i];
				for( size_t j=0; j <3; ++j )
				{
					const vertex3D_t &v = t[j];
					fp("%g %g %g\n", v.x, v.y, v.z);
				}
			}
			fp("POLYGONS %u %u\n", nt, 4 * nt );
			unsigned count = 0;
			for(size_t i=1; i <= nt; ++i, count += 3)
			{
				fp("3 %u %u %u\n", count, count+1, count+2 );
			}
			
		}
		
	}
}
