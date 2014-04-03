#ifndef YOCTO_FOVEA_CELL_INCLUDED
#define YOCTO_FOVEA_CELL_INCLUDED 1

#include "yocto/fovea/shape/line.hpp"
#include "yocto/fovea/shape/triangle.hpp"
#include "yocto/fovea/shape/tetra.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace fovea
    {
        
        //! automatice cell type computation
        template <size_t DIM,typename T>
        struct CellBase;
        
        template <typename T>
        struct CellBase<1,T> { typedef Line<1,T> Type;     };
        
        template <typename T>
        struct CellBase<2,T> { typedef Triangle<2,T> Type; };
        
        template <typename T>
        struct CellBase<3,T> { typedef Tetra<T> Type; };
        
        template <size_t DIM,typename T>
        class Cell : public CellBase<DIM,T>::Type
        {
        public:
            typedef typename CellBase<DIM,T>::Type CellType;
            typedef Vertex<DIM,T> VERTEX;
            Cell *next;
            Cell *prev;
            
            typedef core::list_of_cpp<Cell> List;
            
            virtual ~Cell() throw() {}
            
            explicit Cell( const VERTEX &a, const VERTEX &b ) :
            CellType(a,b),
            next(0),
            prev(0)
            {
            }
            
            
            explicit Cell( const VERTEX &a, const VERTEX &b, const VERTEX &c) :
            CellType(a,b,c)
            {
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Cell);
        };
        
        
    }
    
}

#endif
