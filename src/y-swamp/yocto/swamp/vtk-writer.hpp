#ifndef YOCTO_SWAMP_VTK_WRITER_INCLUDED
#define YOCTO_SWAMP_VTK_WRITER_INCLUDED 1

#include "yocto/swamp/workspace.hpp"
#include "yocto/swamp/rmesh.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/string.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/type-spec.hpp"


namespace yocto 
{
    namespace swamp 
    {
        
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
            
            //! write a rmesh
            template <typename T>
            void write_mesh( ios::ostream &fp, const rmesh<T,layout2D> &mesh , const layout2D &sub )
            {
                const record     &r = (*this)[ typeid(T) ];
                const array1D<T> &X = mesh.X(); assert(X.lower>=sub.lower.x); assert(X.upper<=sub.upper.x);
                const array1D<T> &Y = mesh.Y(); assert(Y.lower>=sub.lower.y); assert(Y.upper<=sub.upper.y);
                write_rmesh_sub(fp,sub);
                write_axis_sub(fp, &X[sub.lower.x], sub.width.x, sizeof(T), r, 'X');
                write_axis_sub(fp, &X[sub.lower.x], sub.width.x, sizeof(T), r, 'Y');
                const T Z = 0;
                write_axis_sub(fp, &Z, 1, sizeof(T), r, 'Z' );
            }
            
            //! write POINT_DATA
            void prolog( ios::ostream &fp, const string &name, size_t num, const record &r) const;
            
            //! write one item
            void write1( ios::ostream &fp, const void *data, const record &r) const;
            
            
            void write_array( ios::ostream &fp, const string &name, const varray &arr, const layout2D &full, const layout2D &sub) const;
                       
            
        private:            
            vtk_format              format;
            set<type_spec,record>   out_db;
            void  write_rmesh_sub( ios::ostream &fp, const layout2D &sub ) const;
            void  write_axis_sub( ios::ostream &fp, const void *data, size_t num, size_t item_size, const record &r, char axis_id ) const;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(vtk_writer);
        };
        
    }
    
}
#endif

