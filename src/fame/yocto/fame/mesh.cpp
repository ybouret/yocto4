#include "yocto/fame/mesh.hpp"

namespace yocto
{
    namespace fame
    {
        
        static inline
        void __init_label( void *ptr, void *args)
        {
            assert(args);
            new (ptr) string( get_axis_name( *(size_t*)args) );
        }
        
        static inline
        void __init_units( void *ptr, void * )
        {
            new (ptr) string();
        }
        
        const string &MeshInfo:: get_label(size_t dim) const throw()
        {
            assert(dim<dimensions);
            return label[dim];
        }
        
        const string &MeshInfo:: get_units(size_t dim) const throw()
        {
            assert(dim<dimensions);
            return units[dim];
        }
        
        
        MeshInfo:: MeshInfo(size_t DIM,const string &id) :
        dimensions(DIM),
        name(id),
        wksp(),
        label(  (string *)wksp,   3),
        units( ((string *)wksp)+3,3)
        {
            for(size_t i=0;i<dimensions;++i)
            {
                label(__init_label,&i  );
                units(__init_units,NULL);
            }
            
        }
        
        MeshInfo:: ~MeshInfo() throw()
        {
        }
        
        
        
        string       & MeshInfo:: XLabel() throw()
        {
            assert(dimensions>=1);
            return label[0];
        }
        
        const string & MeshInfo:: XLabel() const throw()
        {
            assert(dimensions>=1);
            return label[0];
        }
        
        string       & MeshInfo:: XUnits() throw()
        {
            assert(dimensions>=1);
            return units[0];
        }
        
        const string & MeshInfo:: XUnits() const throw()
        {
            assert(dimensions>=1);
            return units[0];
        }
        
        
        string       & MeshInfo:: YLabel() throw()
        {
            assert(dimensions>=2);
            return label[1];
        }
        
        const string & MeshInfo:: YLabel() const throw()
        {
            assert(dimensions>=2);
            return label[1];
        }
        
        string       & MeshInfo:: YUnits() throw()
        {
            assert(dimensions>=2);
            return units[1];
        }
        
        const string & MeshInfo:: YUnits() const throw()
        {
            assert(dimensions>=2);
            return units[1];
        }
        
        string       & MeshInfo:: ZLabel() throw()
        {
            assert(dimensions>=3);
            return label[2];
        }
        
        const string & MeshInfo:: ZLabel() const throw()
        {
            assert(dimensions>=3);
            return label[2];
        }
        
        string       & MeshInfo:: ZUnits() throw()
        {
            assert(dimensions>=3);
            return units[2];
        }
        
        const string & MeshInfo:: ZUnits() const throw()
        {
            assert(dimensions>=3);
            return units[2];
        }




    }
}
