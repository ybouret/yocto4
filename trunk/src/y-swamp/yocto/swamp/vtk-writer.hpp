#ifndef YOCTO_SWAMP_VTK_WRITER_INCLUDED
#define YOCTO_SWAMP_VTK_WRITER_INCLUDED 1

#include "yocto/swamp/workspace.hpp"
#include "yocto/swamp/rmesh.hpp"
#include "yocto/swamp/triangle.hpp"

#include "yocto/string.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/type-spec.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        //! format for basic types
        class vtk_format
        {
        public:
            explicit vtk_format();
            virtual ~vtk_format() throw();
            
            string real_fmt; //!< format for real, default is %g
            string unit_fmt; //!< format for unit, default is %ld
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(vtk_format);
        };
        
        
        
        //! write all arrays
        class vtk_writer
        {
        public:
            typedef void (*callback)( ios::ostream &fp, const void *data, const vtk_format &fmt );
            
            class record
            {
            public:
                const type_spec spec; //!< identifier
                const string    kind; //!< SCALARS | VECTORS
                const callback  proc; //!< print procedure
                
                const type_spec & key() const throw();
                record( const record & );
                record( const std::type_info &id, const char *k, callback cb );
                
            private:
                YOCTO_DISABLE_ASSIGN(record);
            };
            
            
            explicit vtk_writer();
            virtual ~vtk_writer() throw();
            
            //! get the record for the type
            const record & operator[]( const type_spec & ) const;
            
            //! get the record for the type
            const record & operator[]( const std::type_info & ) const;
            
            
            //! write VTK header file
            void header( ios::ostream &fp, const string &title ) const;
            
            //! write a 2D rmesh
            template <typename T>
            void write_mesh( ios::ostream &fp, const rmesh<T,layout2D> &mesh , const layout2D &sub )
            {
                const record     &r = (*this)[ typeid(T) ];
                const array1D<T> &X = mesh.X(); assert(X.lower>=sub.lower.x); assert(X.upper<=sub.upper.x);
                const array1D<T> &Y = mesh.Y(); assert(Y.lower>=sub.lower.y); assert(Y.upper<=sub.upper.y);
                write_rmesh_sub(fp,sub);
                write_axis_sub(fp, &X[sub.lower.x], sub.width.x, sizeof(T), r, 'X');
                write_axis_sub(fp, &Y[sub.lower.y], sub.width.y, sizeof(T), r, 'Y');
                const T Z = 0;
                write_axis_sub(fp, &Z, 1, sizeof(T), r, 'Z' );
            }
            
            //! write a 3D rmesh
            template <typename T>
            void write_mesh( ios::ostream &fp, const rmesh<T,layout3D> &mesh , const layout3D &sub )
            {
                const record     &r = (*this)[ typeid(T) ];
                const array1D<T> &X = mesh.X(); assert(X.lower>=sub.lower.x); assert(X.upper<=sub.upper.x);
                const array1D<T> &Y = mesh.Y(); assert(Y.lower>=sub.lower.y); assert(Y.upper<=sub.upper.y);
                const array1D<T> &Z = mesh.Z(); assert(Z.lower>=sub.lower.z); assert(Z.upper<=sub.upper.z);
                
                write_rmesh_sub(fp,sub);
                write_axis_sub(fp, &X[sub.lower.x], sub.width.x, sizeof(T), r, 'X');
                write_axis_sub(fp, &Y[sub.lower.y], sub.width.y, sizeof(T), r, 'Y');
                write_axis_sub(fp, &Z[sub.lower.z], sub.width.z, sizeof(T), r, 'Z' );
            }
            
            
            //! write POINT_DATA
            void prolog( ios::ostream &fp, const string &name, size_t num, const record &r) const;
            
            //! write one item
            void write1( ios::ostream &fp, const void *data, const record &r) const;
            
            //! write a 2D array (after its mesh)
            void write_array( ios::ostream &fp, const string &name, const varray &arr, const layout2D &full, const layout2D &sub) const;
            
            //! write a 3D array (after its mesh)
            void write_array( ios::ostream &fp, const string &name, const varray &arr, const layout3D &full, const layout3D &sub) const;
            
            
            //! unique vtk save routine
            template <typename U,typename LAYOUT>
            void save( const string &filename, const string &title, const workspace<LAYOUT, U, rmesh> &wksp, const array<string> &var, const LAYOUT &sub )
            {
                ios::ocstream fp(filename,false);
                header(fp, title);
                write_mesh(fp,wksp.mesh,sub);
                for( size_t q=1; q <= var.size(); ++q )
                {
                    const string &name = var[q];
                    const varray &arr  = wksp[name];
                    write_array(fp, name, arr, wksp.outline, sub);
                }
            }
            
            template <typename T>
            inline void save( const string &filename, const string &title, const array< triangle3D<T> > &triangles ) const
            {
                const unsigned nt = triangles.size();
                ios::ocstream fp( filename, false );
                header(fp,title);
                fp("DATASET POLYDATA\n");
                fp("POINTS %u float\n", nt * 3 );
                const record &r = (*this)[ typeid( typename vertex3D<T>::type ) ];
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
            vtk_format              format;
            set<type_spec,record>   out_db;
            void  write_rmesh_sub( ios::ostream &fp, const layout2D &sub ) const;
            void  write_rmesh_sub( ios::ostream &fp, const layout3D &sub ) const;
            
            void  write_axis_sub( ios::ostream &fp, const void *data, size_t num, size_t item_size, const record &r, char axis_id ) const;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(vtk_writer);
        };
        
    }
    
}
#endif

