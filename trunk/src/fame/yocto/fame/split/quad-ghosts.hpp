#ifndef YOCTO_FAME_SPLIT_QUAD_GHOSTS_INCLUDED
#define YOCTO_FAME_SPLIT_QUAD_GHOSTS_INCLUDED 1

#include "yocto/fame/ghost.hpp"
#include "yocto/fame/layout.hpp"
#include "yocto/core/list.hpp"

namespace yocto
{
    namespace fame
    {
        
        template <typename Layout>
        class quad_ghost : public ghost
        {
        public:
            const Layout zone;
            inline quad_ghost(const Layout &outline,
                              const Layout &gzone  ) :
            ghost(gzone.items),
            zone(gzone)
            {
                outline.store_offsets_of(zone,*this);
            }
            
            virtual ~quad_ghost() throw()
            {
                
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(quad_ghost);
        };
        
        
        class quad_ghosts_io : public object
        {
        public:
            explicit quad_ghosts_io() throw();
            virtual ~quad_ghosts_io() throw();
            
            void release() throw();
            void ensure(size_t interleaved_bytes);
            
            
        protected:
            void          *sbuffer; //!< send buffer
            void          *rbuffer; //!< recv buffer
            const size_t   io_size;
            
        private:
            size_t   iobytes;
            void io_update() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(quad_ghosts_io);
        };
        
        
        template <typename Layout>
        class quad_ghosts : public quad_ghosts_io
        {
        public:
            typedef quad_ghosts        Ghosts;
            typedef quad_ghost<Layout> Ghost;
            
            const   int    peer;    //!< whom am I talking to ?
            const   int    dim;     //!< 0,1,2
            const   Ghost  source;  //!< data from rank
            const   Ghost  target;  //!< data from peer
            const   size_t size;    //!< common data size
            
            Ghosts *next;
            Ghosts *prev;
            
            typedef core::list_of_cpp< Ghosts > list;
            
            explicit quad_ghosts(const Layout outline,
                                 const int    peer_rank,
                                 const int    which_dim,
                                 const Layout source_layout,
                                 const Layout target_layout):
            peer(peer_rank),
            dim(which_dim),
            source(outline,source_layout),
            target(outline,target_layout),
            size(source.size()),
            next(0),
            prev(0)
            {
                assert( source.size() == target.size() );
            }
            
            virtual ~quad_ghosts() throw()
            {
            }
            
            
            //! put source zone into target zone for all handles
            inline void local_update( linear_handles &handles ) const throw()
            {
                for(size_t i=handles.size();i>0;--i)
                {
                    linear_space &l = handles[i];
                    ghost::copy(l,target,l,source);
                }
            }
            
            //! memory of I/O
            inline void allocate_for( const size_t chunk_size  )
            {
                ensure( size * chunk_size );
            }
            
            inline void recv_dispatch( linear_handles &handles ) throw()
            {
                assert(io_size>=size*handles.chunk_size);
                target.load(handles,rbuffer);
            }
            
            inline void send_assemble( const linear_handles &handles ) throw()
            {
                assert(io_size>=size*handles.chunk_size);
                source.save(sbuffer,handles);
            }
            
           
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(quad_ghosts);
        };
        
        
        
        
        
    }
}

#endif
