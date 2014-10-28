#ifndef YOCTO_FAME_SPLIT_INCLUDED
#define YOCTO_FAME_SPLIT_INCLUDED 1

#include "yocto/fame/layout.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        class Link
        {
        public:
            enum Position
            {
                Prev,
                Next
            };
            const int      rank;
            const Position pos;
            
            explicit Link( int r, Position p) throw() : rank(r), pos(p) {}
            ~Link() throw() {}
            Link(const Link &link) throw() : rank(link.rank), pos(link.pos) {}
            
            static
            layout1D Split(const layout1D &full_layout,
                           int rank,
                           int size );
            
        private:
            YOCTO_DISABLE_ASSIGN(Link);
        };
        
        
        
        
    }
    
}

#endif
