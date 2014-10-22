#ifndef YOCTO_FAME_MESH_INCLUDED
#define YOCTO_FAME_MESH_INCLUDED 1

#include "yocto/fame/vertices.hpp"
#include "yocto/fame/arrays.hpp"
#include "yocto/memory/records.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        class MeshInfo
        {
        public:
            explicit MeshInfo(size_t DIM);
            virtual ~MeshInfo() throw();
            
            const size_t dimensions;
            
            string       & XLabel() throw();
            const string & XLabel() const throw();
            string       & XUnits() throw();
            const string & XUnits() const throw();
            
            string       & YLabel() throw();
            const string & YLabel() const throw();
            string       & YUnits() throw();
            const string & YUnits() const throw();
            
            string       & ZLabel() throw();
            const string & ZLabel() const throw();
            string       & ZUnits() throw();
            const string & ZUnits() const throw();

            
            const string &get_label(size_t dim) const throw();
            const string &get_units(size_t dim) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(MeshInfo);
            char wksp[ 6*sizeof(string) ];
            memory::records_of<string> label;
            memory::records_of<string> units;
            
        };
        
        
        template <size_t DIM,typename T>
        class Mesh : public MeshInfo
        {
        public:
            static const size_t DIMENSIONS=DIM;
            
            typedef Vertex<DIM,T>      VTX;
            typedef typename VTX::List Vertices;
            typedef array1D<T>         Axis;
            
            Vertices vertices;
            
            inline   Mesh( arrays &adb ) :
            MeshInfo(DIM),
            vertices()
            {
                
            }
            virtual ~Mesh() throw() {}
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Mesh);
            
            
        };
    }
    
}

#endif
