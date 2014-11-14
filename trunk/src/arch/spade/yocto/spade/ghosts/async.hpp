#ifndef YOCTO_SPADE_ASYNC_GHOSTS_INCLUDED
#define YOCTO_SPADE_ASYNC_GHOSTS_INCLUDED 1

#include "yocto/spade/ghosts/ghost.hpp"
#include "yocto/spade/linear-handles.hpp"
#include "yocto/ptr/shared.hpp"

namespace yocto
{
    namespace spade
    {
      
        class async_ghosts : public ghosts
        {
        public:
            typedef shared_ptr<async_ghosts> ptr;
            
            //! set ghosts site
            explicit async_ghosts( ghost::position pos ) throw();
            
            //! release memory
            virtual ~async_ghosts() throw();
            
            ghost  inner;   //!< in layout
            ghost  outer;   //!< in outline
            int    peer;    //!< default is -1
            size_t content; //!< last content, in bytes
            
            //! create offsets
            template <typename LAYOUT>
            inline void setup( size_t num_ghosts, const LAYOUT &outline)
            {
                //std::cerr << "async " << inner.position_name() << std::endl;
                //! create corresponding inner layout
                {
                    const LAYOUT inner_sub = inner.inner_sublayout(outline, num_ghosts);
                    inner.load_from(outline,inner_sub);
                    //std::cerr << "\tinner_sub:" << inner_sub << std::endl;
                }
                
                //! create corresponding outer layout
                {
                    const LAYOUT outer_sub = outer.outer_sublayout(outline,num_ghosts);
                    outer.load_from(outline,outer_sub);
                    //std::cerr << "\touter_sub:" << outer_sub << std::endl;
                }
                
                assert( inner.size() == outer.size() );
                (size_t&)num_offsets = inner.size();
            }
            
            uint8_t *ibuffer; //!< buffer to store inner ghost
            uint8_t *obuffer; //!< buffer to query outer ghost
            size_t   iobytes; //!< maximum buffers capacity
            
            void allocate_for( const size_t max_interleaved_size );
            
            //! store inner handle content into ibuffer
            /**
             update content, the number of stored bytes
             */
            void inner_store( const linear &handle ) throw();
            
            //! query outer handle content from obuffer
            void outer_query( linear &handle ) throw();
            
            //! store inner handles content into ibuffer
            /**
             update content
             */
            void inner_store( const linear_handles &handles ) throw();
           
            //! query outer handle content from obuffer
            void outer_query( linear_handles &handles ) throw();

            //! check if has enough bytes for handle
            bool can_handle( const linear &handle ) const throw();
            
            //! check if has enogh bytes for handles
            bool can_handle( const linear_handles &handles) const throw();
            
            
        private:
            size_t iolen;                 //!< for memory allocation
            size_t max_interleaved_bytes; //!< for I/O control
            YOCTO_DISABLE_COPY_AND_ASSIGN(async_ghosts);
            
        };
        
    }
    
}

#endif

