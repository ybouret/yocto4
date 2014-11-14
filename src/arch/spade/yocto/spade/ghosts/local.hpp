#ifndef YOCTO_SPADE_LOCAL_GHOSTS_INCLUDED
#define YOCTO_SPADE_LOCAL_GHOSTS_INCLUDED 1

#include "yocto/spade/ghosts/ghost.hpp"
#include "yocto/spade/linear-handles.hpp"
#include "yocto/ptr/shared.hpp"

namespace yocto
{
    namespace spade
    {
        
        //! for PBC without parallelism
        class local_ghosts_pair
        {
        public:
            ghost inside; //!< inside, in layout
            ghost mirror; //!< mirror, in outline
            
            local_ghosts_pair( ghost::position pos ) throw();
            ~local_ghosts_pair() throw();
            
            template <typename LAYOUT>
            inline void setup( size_t num_ghosts, const LAYOUT &outline)
            {
                //! create corresponding inner layout
                {
                    const LAYOUT inside_sub = inside.inner_sublayout(outline, num_ghosts);
                    //std::cerr << "\tinside_sub: " << inside_sub << std::endl;
                    inside.load_from(outline, inside_sub);
                }
                
                //! create corresponding outer layout
                {
                    const LAYOUT mirror_sub = mirror.outer_sublayout(outline, num_ghosts);
                    //std::cerr << "\tmirror_sub: " << mirror_sub << std::endl;
                    mirror.load_from(outline,mirror_sub);
                }
                
                assert( inside.size() == mirror.size() );
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(local_ghosts_pair);
        };
        
        
        //! lower and upper pair
        class local_ghosts : public ghosts
        {
        public:
            typedef shared_ptr<local_ghosts> ptr;

            
            explicit local_ghosts( dimension_t dim ) throw();
            virtual ~local_ghosts() throw();
            
            //! transfert one array
            void transfer( linear &handle ) throw();
            
            //! transfert a set of handles
            void transfer( array<linear_ptr> &handles ) throw();
            
            
            
            //! compute all offets
            template <typename LAYOUT>
            inline void setup( size_t num_ghosts, const LAYOUT &outline)
            {
                //std::cerr << "local lower:" << std::endl;
                lower.setup<LAYOUT>( num_ghosts, outline);
                
                //std::cerr << "local upper:" << std::endl;
                upper.setup<LAYOUT>( num_ghosts, outline);
                assert(lower.inside.size() == upper.inside.size());
                (size_t&)num_offsets = lower.inside.size();
            }
            
            local_ghosts_pair lower;
            local_ghosts_pair upper;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(local_ghosts);
        };
    }
    
}

#endif
