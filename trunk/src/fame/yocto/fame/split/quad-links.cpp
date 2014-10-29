#include "yocto/fame/split/quad-links.hpp"

namespace yocto
{
    namespace fame
    {
        quad_link:: ~quad_link() throw()
        {
        }
        
        quad_link:: quad_link() throw() : pos(is_prev), rank(-1) {}
        
        quad_link:: quad_link( position p, int r) throw() : pos(p), rank(r) {}
        
        quad_link:: quad_link( const quad_link &l ) throw() : pos(l.pos), rank(l.rank) {}
        
        quad_link & quad_link:: operator=(const quad_link &l) throw()
        {
            pos  = l.pos;
            rank = l.rank;
            return *this;
        }
       
        std::ostream & operator<<(std::ostream &os, const quad_link &l)
        {
            os << l.rank << "@" << ((l.pos==quad_link::is_prev) ? "prev" : "next");
            return os;
        }
        
        
    }
    
}

namespace yocto
{
    namespace fame
    {
        
        quad_links:: ~quad_links() throw()
        {
            
        }
        
        quad_links:: quad_links() throw() : count(0), link()
        {
        }
        
        void quad_links:: append( quad_link::position pos, int rank ) throw()
        {
            assert(count<2);
            quad_link &l = link[count];
            l.pos  = pos;
            l.rank = rank;
            
            ++(size_t&)count;
        }

        quad_link & quad_links:: operator[](size_t i) throw()
        {
            assert(i<count);
            return link[i];
        }
        
        
        const quad_link & quad_links:: operator[](size_t i) const throw()
        {
            assert(i<count);
            return link[i];
        }
        
        void quad_links::reset() throw() { (size_t&)count=0; }
        
        
        std::ostream & operator<<(std::ostream &os, const quad_links &l)
        {
            os << "[[";
            for(size_t i=0;i<l.count;++i)
            {
                os << ' ' << l.link[i];
            }
            os << " ]]";
            return os;
        }

        
    }
}
