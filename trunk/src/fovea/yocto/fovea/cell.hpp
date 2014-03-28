#ifndef YOCTO_FOVEA_CELL_INCLUDED
#define YOCTO_FOVEA_CELL_INCLUDED 1

#include "yocto/fovea/vertex.hpp"
#include "yocto/container/vslot.hpp"

namespace yocto
{
    namespace fovea
    {
        
        template <size_t DIM,typename T>
        class Cell : public vslot
        {
        public:
            typedef typename vertex_for<DIM,T>::type vertex;
            typedef Vertex<DIM,T>                    VERTEX;
            
            virtual ~Cell() throw()
            {
            }
            
            const size_t vertices;
            
            virtual VERTEX **vtx() const throw() = 0;

            //! once physical vertices have been set
            virtual void compile() = 0;
            
            
        protected:
            inline Cell( size_t nv ) throw() :
            vertices(nv)
            {
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Cell);
        };
        
    }
}

#endif
