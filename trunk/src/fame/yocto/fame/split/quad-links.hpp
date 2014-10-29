#ifndef YOCTO_FAME_SPLIT_QUAD_LINKS_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD_LINKS_INCLUDED 1

#include "yocto/fame/types.hpp"

namespace yocto
{
    namespace fame
    {
        //! a quad link in one direction
        class quad_link
        {
        public:
            enum position
            {
                is_prev,
                is_next
            };
            position pos;
            int      rank;
            
            quad_link() throw();
            quad_link(position p, int r) throw();
            ~quad_link() throw();
            quad_link(const quad_link &) throw();
            quad_link &operator=(const quad_link &) throw();
            
            friend std::ostream & operator<<(std::ostream &,const quad_link &);
            
            
        };
        
        //! quad links in one direction
        class quad_links
        {
        public:
            const size_t count;
            quad_links()  throw();
            ~quad_links() throw();
            
            void append( quad_link::position pos, int rank ) throw();
            
            quad_link & operator[](size_t i) throw();
            const quad_link & operator[](size_t i) const throw();
            void reset() throw();
            
            friend std::ostream & operator<<(std::ostream &,const quad_links &);

            
        private:
            quad_link link[2];
            YOCTO_DISABLE_COPY_AND_ASSIGN(quad_links);
        };
    }
}

#endif

