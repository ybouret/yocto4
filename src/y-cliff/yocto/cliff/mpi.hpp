#ifndef YOCTO_CLIFF_MPI_INCLUDED
#define YOCTO_CLIFF_MPI_INCLUDED 1


#include "yocto/cliff/workspace.hpp"
#include "yocto/mpi/mpi.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		template <typename WORKSPACE>
		struct _mpi
		{
			//! exchanges variables
			/**
			 \param w the workspace
			 \param cid the list of components to exchange
			 \param MPI the current MPI instance
			 
			 async ghosts are send/recv while the plain ghosts are directly copied
			 */
			static inline void exchanges_start( WORKSPACE &w, const array<size_t> &cid, mpi::Requests &requests, int tag ) 
			{
				static const mpi & MPI = mpi::instance();
				assert( cid.size() > 0 );
				assert( requests.count == (w.async_ghosts *2) * cid.size() );
				
				size_t iRequest = 0;
				for( size_t g = w.async_ghosts; g>0; --g )
				{					
					typename WORKSPACE::ghost_type const &outer_ghost = w.async_outer_ghost(g); 
					typename WORKSPACE::ghost_type const &inner_ghost = w.async_inner_ghost(g); 
					
					for( size_t i=cid.size();i>0;--i)
					{
						//------------------------------------------------------
						// wait for data from outer ghost peer
						//------------------------------------------------------
						MPI.Irecv( outer_ghost[i], outer_ghost.bytes, MPI_BYTE, outer_ghost.peer, tag, MPI_COMM_WORLD, requests[iRequest++]);
						
						//------------------------------------------------------
						// load data from war #cid[i] into slot i
						//------------------------------------------------------
						inner_ghost.pull( w[ cid[i] ], i );
						
						//------------------------------------------------------
						// send it to inner ghost peer
						//------------------------------------------------------
						MPI.Isend( inner_ghost[i], inner_ghost.bytes, MPI_BYTE, inner_ghost.peer, tag, MPI_COMM_WORLD, requests[iRequest++]);
					}
				}
				
				for( size_t g = w.plain_ghosts; g>0; --g )
				{
					typename WORKSPACE::ghost_type const &outer_ghost = w.plain_outer_ghost(g); 
					typename WORKSPACE::ghost_type const &inner_ghost = w.plain_inner_ghost(g); 
					
					for( size_t i=cid.size();i>0;--i)
					{
						WORKSPACE::ghost_type::direct_copy( outer_ghost, inner_ghost, w[ cid[i] ] );
					}
				}
			}
			
			static inline void exchanges_finish( WORKSPACE &w, const array<size_t> &cid, mpi::Requests &requests ) 
			{
				static const mpi & MPI = mpi::instance();
				assert( cid.size() > 0 );
				assert( requests.count == (w.async_ghosts *2) * cid.size() );
				
				MPI.Waitall(requests);
				for( size_t g = w.async_ghosts; g>0; --g )
				{	
					typename WORKSPACE::ghost_type const &outer_ghost = w.async_outer_ghost(g); 
					for( size_t i = cid.size(); i>0; --i )
					{
						outer_ghost.push( w[ cid[i] ], i );
					}
				}
			}
			
			
		};
		
	}
	
}



#endif
