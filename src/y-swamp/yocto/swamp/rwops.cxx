namespace yocto 
{
    
    namespace swamp
    {
        template <>
		void rwops<ZTYPE>:: save_vtk( const string &filename, const string &title, const array< triangle3D<ZTYPE> > &triangles)
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
				const triangle3D<ZTYPE> &t = triangles[i];
				for( size_t j=0; j <3; ++j )
				{
					const vertex3D<ZTYPE>::type &v = t[j];
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
        
        template <>
        void rwops<ZTYPE>:: save_vtk(const string                           &filename,
                                     const string                           &title,
                                     const workspace<layout2D,ZTYPE,rmesh>  &wksp,
                                     const array<string>                    &id,
                                     const layout2D                         &sub )
        {
            assert( wksp.outline.contains(sub) );
            ios::ocstream fp( filename, false );
            const array1D<ZTYPE> &X = wksp.mesh.X();
            const array1D<ZTYPE> &Y = wksp.mesh.Y();
            
			fp("# vtk DataFile Version 3.0\n");
			fp("%s\n", title.c_str());
			fp("ASCII\n");
            fp("DATASET RECTILINEAR_GRID\n");
            fp("DIMENSIONS %u %u 1\n", unsigned(sub.width.x), unsigned(sub.width.y));
            fp("X_COORDINATES %u float\n", unsigned(sub.width.x));
            for( unit_t i = sub.lower.x; i <= sub.upper.x; ++i )
                fp("%g ", X[i]);
            fp("\n");
            fp("Y_COORDINATES %u float\n", unsigned(sub.width.y));
            for( unit_t j = sub.lower.y; j <= sub.upper.y; ++j )
                fp("%g ", Y[j]);
            fp("\n");
            fp("Z_COORDINATES 1 float\n0\n");
            for( size_t i=1; i <= id.size(); ++i )
            {
                const string         &name = id[i];
                const array2D<ZTYPE> &A    = wksp[name].as< array2D<ZTYPE> >();
                fp("POINT_DATA %u\n", unsigned(sub.items));
                fp("SCALARS %s float\n", name.c_str());
                fp("LOOKUP_TABLE default\n");
                for( unit_t j = sub.lower.y; j <= sub.upper.y; ++j )
                {
                    for( unit_t i = sub.lower.x; i <= sub.upper.x; ++i )
                    {
                        fp("%g\n", A[j][i] );
                    }
                }
            }
        }
        
        template <>
        void rwops<ZTYPE>:: save_vtk(const string                           &filename,
                                     const string                           &title,
                                     const workspace<layout3D,ZTYPE,rmesh>  &wksp,
                                     const array<string>                    &id,
                                     const layout3D                         &sub )
        {
            assert( wksp.outline.contains(sub) );
            ios::ocstream fp( filename, false );
            const array1D<ZTYPE> &X = wksp.mesh.X();
            const array1D<ZTYPE> &Y = wksp.mesh.Y();
            const array1D<ZTYPE> &Z = wksp.mesh.Z();
            
			fp("# vtk DataFile Version 3.0\n");
			fp("%s\n", title.c_str());
			fp("ASCII\n");
            fp("DATASET RECTILINEAR_GRID\n");
            fp("DIMENSIONS %u %u %u\n", unsigned(sub.width.x), unsigned(sub.width.y), unsigned(sub.width.z) );
            fp("X_COORDINATES %u float\n", unsigned(sub.width.x));
            for( unit_t i = sub.lower.x; i <= sub.upper.x; ++i )
                fp("%g ", X[i]);
            fp("\n");
            fp("Y_COORDINATES %u float\n", unsigned(sub.width.y));
            for( unit_t j = sub.lower.y; j <= sub.upper.y; ++j )
                fp("%g ", Y[j]);
            fp("\n");
            fp("Z_COORDINATES %u float\n", unsigned(sub.width.z));
            for( unit_t k = sub.lower.z; k <= sub.upper.z; ++k )
                fp("%g ", Z[k]);
            fp("\n");
            
            for( size_t i=1; i <= id.size(); ++i )
            {
                const string         &name = id[i];
                const array3D<ZTYPE> &A    = wksp[name].as< array3D<ZTYPE> >();
                fp("POINT_DATA %u\n", unsigned(sub.items));
                fp("SCALARS %s float\n", name.c_str());
                fp("LOOKUP_TABLE default\n");
                for( unit_t k = sub.lower.z; k <= sub.upper.z; ++k )
                {
                    for( unit_t j = sub.lower.y; j <= sub.upper.y; ++j )
                    {
                        for( unit_t i = sub.lower.x; i <= sub.upper.x; ++i )
                        {
                            fp("%g\n", A[k][j][i] );
                        }
                    }
                }
            }
        }
        
        
    }
    
}
