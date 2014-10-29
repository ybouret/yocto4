#ifndef YOCTO_FAME_SPLIT_INCLUDED
#define YOCTO_FAME_SPLIT_INCLUDED 1

#include "yocto/fame/layout.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        //! information for one link
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
            
            Link() throw() : rank(-1), pos(Prev) {}
            Link( int r, Position p) throw() : rank(r), pos(p) {}
            ~Link() throw() {}
            Link(const Link &link) throw() : rank(link.rank), pos(link.pos) {}
            
            friend std::ostream & operator<<(std::ostream &os, const Link &l )
            {
                os << l.rank << "@" << ((l.pos==Prev) ? "Prev" : "Next");
                return os;
            }
            
            void set_rank(int r) throw()
            {
                assert(r>=0);
                (int&)rank = r;
            }
            
        private:
            YOCTO_DISABLE_ASSIGN(Link);
        };
        
        //! links in one direction
        class Links
        {
        public:
            const size_t count;
            inline Links() throw() : count(0), link() {}
            inline ~Links() throw() {}
            
            void reset() throw() { (int&)count = 0; }
            
            void append(const int rank, const Link::Position pos) throw()
            {
                assert(count<2);
                (int            &)(link[count].rank) = rank;
                (Link::Position &)(link[count].pos)  = pos;
                ++(size_t &)count;
            }
            
            friend std::ostream & operator<<(std::ostream &os, const Links &l )
            {
                os << "[[";
                for(size_t i=0;i<l.count;++i)
                {
                    os << ' ' << l.link[i];
                }
                os << " ]]";
                return os;
            }
            
            Link       & operator[](size_t i) throw()       { assert(i<count); return link[i]; }
            const Link & operator[](size_t i) const throw() { assert(i<count); return link[i]; }
            
            
        private:
            Link link[2];
            YOCTO_DISABLE_COPY_AND_ASSIGN(Links);
        };
        
        
        
        //----------------------------------------------------------------------
        //
        // 1D
        //
        //----------------------------------------------------------------------

        //! create a 1D sub layout
        layout1D Split(const layout1D &full_layout,
                       int             rank,
                       int             size,
                       bool            pbc,
                       Links          &links,
                       int            *ranks);
        
        //! create the outline for num ghosts and links
        layout1D Outline(const layout1D &inside,
                         const Links    &links,
                         const size_t    ng);
        
        
        //! create a 2D sub layout
        layout2D Split(const layout2D &full_layout,
                       int             rank,
                       int             size,
                       bool            xpbc,
                       Links          &xlinks,
                       bool            ypbc,
                       Links          &ylinks,
                       int            *ranks);
        
    }
    
}

#endif
