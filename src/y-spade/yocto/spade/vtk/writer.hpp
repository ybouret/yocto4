#ifndef YOCTO_SPADE_VTK_WRITER_INCLUDED
#define YOCTO_SPADE_VTK_WRITER_INCLUDED 1

#include "yocto/spade/vtk/record.hpp"
#include "yocto/spade/types.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/spade/rmesh.hpp"
#include "yocto/spade/array3d.hpp"
#include "yocto/spade/workspace.hpp"
#include "yocto/spade/triangle.hpp"

namespace yocto
{
    namespace spade
    {
        
        class vtk_writer
        {
        public:
            virtual ~vtk_writer() throw();
            explicit vtk_writer();
            
            vtk_format format;
            
            //! get the record for the type
            const vtk_record & operator[]( const type_spec & ) const;
            
            //! get the record for the type
            const vtk_record & operator[]( const std::type_info & ) const;
            
            //! write the vtk header
            void header( ios::ostream &fp, const string &title ) const;
            
            //! write a 2D rmesh
            template <typename T> inline
            void write_mesh( ios::ostream &fp, const rmesh<layout2D,T> &mesh , const layout2D &sub ) const
            {
                const vtk_record &r = (*this)[ typeid(T) ];
                const array1D<T> &X = mesh.X();
                const array1D<T> &Y = mesh.Y();
                write_rmesh_sub(fp,sub);
                write_axis_sub(fp, &X[sub.lower.x], sub.width.x, sizeof(T), r, 'X');
                write_axis_sub(fp, &Y[sub.lower.y], sub.width.y, sizeof(T), r, 'Y');
                const T Z = 0;
                write_axis_sub(fp, &Z, 1, sizeof(T), r, 'Z' );
            }
            
            //! write a 3D rmesh
            template <typename T> inline
            void write_mesh( ios::ostream &fp, const rmesh<layout3D,T> &mesh , const layout3D &sub ) const
            {
                const vtk_record &r = (*this)[ typeid(T) ];
                const array1D<T> &X = mesh.X();
                const array1D<T> &Y = mesh.Y();
                const array1D<T> &Z = mesh.Z();
                
                write_rmesh_sub(fp,sub);
                write_axis_sub(fp, &X[sub.lower.x], sub.width.x, sizeof(T), r, 'X');
                write_axis_sub(fp, &Y[sub.lower.y], sub.width.y, sizeof(T), r, 'Y');
                write_axis_sub(fp, &Z[sub.lower.z], sub.width.z, sizeof(T), r, 'Z' );
            }
            
            //! write kind
            void prolog( ios::ostream &fp, const string &name, const vtk_record &r) const;
            
            //! write one item
            void write1( ios::ostream &fp, const void *data, const vtk_record &r) const;
            
            //! write a 2D array (after its mesh)
            void write_array( ios::ostream &fp, const string &name, const varray &arr, const layout2D &full, const layout2D &sub) const;
            
            //! write a 3D array (after its mesh)
            void write_array( ios::ostream &fp, const string &name, const varray &arr, const layout3D &full, const layout3D &sub) const;
            
            
            //! unique vtk save routine
            template <typename LAYOUT,typename U>
            void save( const string &filename, const string &title, const workspace<LAYOUT,rmesh,U> &wksp, const array<string> &var, const LAYOUT &sub ) const
            {
                ios::ocstream fp(filename,false);
                header(fp, title);
                write_mesh<U>(fp,wksp.mesh,sub);
                fp("POINT_DATA %u\n", unsigned(sub.items));
                for( size_t q=1; q <= var.size(); ++q )
                {
                    const string &name = var[q];
                    const varray &arr  = wksp[name];
                    write_array(fp, name, arr, wksp.outline, sub);
                }
            }
            
            //! unique vtk save routine, wrapper
            template <typename LAYOUT,typename U>
            void save( const char *the_filename, const char *the_title, const workspace<LAYOUT,rmesh,U> &wksp, const array<string> &var, const LAYOUT &sub ) const
            {
				const string filename(the_filename);
				const string title(the_title);
				this->save<LAYOUT,U>(filename,title,wksp,var,sub);
			}

            //! save triangle3D, as computed by iso3D
            template <typename T>
            inline void save( const string &filename, const string &title, const array< triangle3D<T> > &triangles ) const
            {
                const unsigned nt = triangles.size();
                ios::ocstream fp( filename, false );
                header(fp,title);
                fp("DATASET POLYDATA\n");
                fp("POINTS %u float\n", nt * 3 );
                const vtk_record &r = (*this)[ typeid( typename vertex3D<T>::type ) ];
                for(size_t i=1; i <= nt; ++i )
                {
                    const triangle3D<T> &t = triangles[i];
                    for( size_t j=0; j <3; ++j )
                    {
                        write1(fp, &t[j], r);
                    }
                }
                fp("POLYGONS %u %u\n", nt, 4 * nt );
                unsigned count = 0;
                for(size_t i=1; i <= nt; ++i, count += 3)
                {
                    fp("3 %u %u %u\n", count, count+1, count+2 );
                }
            }

            
        private:
            set<type_spec,vtk_record> records;
            YOCTO_DISABLE_COPY_AND_ASSIGN(vtk_writer);
            
            void  write_rmesh_sub( ios::ostream &fp, const layout2D &sub ) const;
            void  write_rmesh_sub( ios::ostream &fp, const layout3D &sub ) const;
            void  write_axis_sub( ios::ostream &fp, const void *data, size_t num, size_t item_size, const vtk_record &r, char axis_id ) const;
        };
        
    }
    
}

#endif
