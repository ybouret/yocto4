#include "yocto/cliff/rwops.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
	namespace cliff
	{
		using math::real_t;
		
		template <>
		void rwops<real_t> :: save_vtk(const string        &filename,
									   const string        &title,
									   const wksp3_type    &wksp,
									   const array<size_t> &cid,
									   const layout3D      &sub)
		{
			
			assert( wksp.has(sub.lower) );
			assert( wksp.has(sub.upper) );
			assert( cid.size() > 0 );
			
			ios::ocstream fp( filename, false );
			
			const vertex3D_t              spacing = wksp.delta;
			const layout3D               &inilay  = wksp;
			const wksp3_type::region_type subreg  = wksp3_type::region_type::extract( wksp.region, inilay, sub );
			const vertex3D_t              origin  = subreg.min;
			
			fp("# vtk DataFile Version 3.0\n");
			fp("%s\n", title.c_str());
			fp("ASCII\n");
			fp("DATASET STRUCTURED_POINTS\n");
			fp("DIMENSIONS %u %u %u\n", unsigned(sub.width.x), unsigned(sub.width.y), unsigned(sub.width.z) );
			fp("ORIGIN %g %g %g\n", origin.x, origin.y, origin.z );
			fp("SPACING %g %g %g\n", spacing.x, spacing.y, spacing.z );
			fp("POINT_DATA %u\n", unsigned(sub.items) );
			
			for( size_t iv = 1; iv <= cid.size(); ++iv )
			{
				const size_t           jv    = cid[iv];
				const string          &name  = wksp.name( jv );
				const array3D<real_t> &field = wksp[jv]; 
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
		}
		
		template <>
		void rwops<real_t> :: save_vtk(const string        &filename,
									   const string        &title,
									   const wksp2_type    &wksp,
									   const array<size_t> &cid,
									   const layout2D      &sub)
		{
			
			assert( wksp.has(sub.lower) );
			assert( wksp.has(sub.upper) );
			assert( cid.size() > 0 );
			const vertex2D_t              spacing = wksp.delta;
			const layout2D               &inilay  = wksp;
			const wksp2_type::region_type subreg  = wksp2_type::region_type::extract( wksp.region, inilay, sub );
			const vertex2D_t              origin  = subreg.min;
			
			
			ios::ocstream fp( filename, false );
			fp("# vtk DataFile Version 3.0\n");
			fp("%s\n", title.c_str());
			fp("ASCII\n");
			fp("DATASET STRUCTURED_POINTS\n");
			fp("DIMENSIONS %u %u 1\n", unsigned(sub.width.x), unsigned(sub.width.y) );
			fp("ORIGIN %g %g 0\n", origin.x, origin.y);
			fp("SPACING %g %g 0\n", spacing.x, spacing.y);
			fp("POINT_DATA %u\n", unsigned(sub.items) );
			
			for( size_t iv=1; iv <= cid.size(); ++iv )
			{
				const size_t           jv    = cid[iv];
				const string          &name  = wksp.name(jv);
				const array2D<real_t> &field = wksp[jv];
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
