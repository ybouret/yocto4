namespace yocto 
{

    namespace swamp
    {
        template <>
		void rwops<ZTYPE> ::save_vtk( const string &filename, const string &title, const array< triangle3D<ZTYPE> > &triangles)
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

    }
    
}
