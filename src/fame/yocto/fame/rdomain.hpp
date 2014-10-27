#ifndef YOCTO_RDOMAIN_INCLUDED
#define YOCTO_RDOMAIN_INCLUDED 1

#include "yocto/fame/layout.hpp"

namespace yocto
{
    namespace fame
    {
        
        class RDomain
        {
        public:
            const int rank; //!< self rank
            
            enum Position
            {
                Prev,
                Next
            };
            
            // one link in one directions
            class Link
            {
            public:
                const Position position;
                const int      rank;
                
                Link() throw();
                ~Link() throw();
                void set( const Position p, const int r) throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Link);
            };
            
            
            // links in one direction
            class Links
            {
            public:
                const size_t count;
                Links() throw();
                ~Links() throw();
                
                void append(const Position p, const int r) throw();
                
            private:
                Link link[2];
            };
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(RDomain);
            
        };
        
        template <size_t DIM>
        class RDomains : public RDomain
        {
        public:
            
            
            const Links links[DIM];
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(RDomains);
            
        };
        
        
    }
}

#endif
