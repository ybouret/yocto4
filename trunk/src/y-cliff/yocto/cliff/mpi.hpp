#ifndef YOCTO_CLIFF_MPI_INCLUDED
#define YOCTO_CLIFF_MPI_INCLUDED 1


#include "yocto/cliff/workspace.hpp"
#include "yocto/mpi/mpi.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		
		struct _mpi
		{
			//! exchanges variables: start
			/**
			 \param w the workspace
			 \param cid the list of components to exchange
			 \param MPI the current MPI instance
			 
			 async ghosts are send/recv while the plain ghosts are directly copied
			 */
			template <typename WORKSPACE>
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
			
			//! exchanges variables: finish
			template <typename WORKSPACE>
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
			
			
			//! collect a global array in rank 0
			template <typename T>
			static inline
			void collect0( array3D<T> *pA, const array3D<T> &B, const layout3D &full )
			{
				static const mpi & MPI = mpi::instance();
				static const int   tag = 0x0C01;
				if( 0 == MPI.CommWorldRank )
				{
					assert( NULL != pA );
					array3D<T> &A = *pA;
					//-- direct copy of B in A
					{
						const layout3D sub = full.split(0, MPI.CommWorldSize );
						A.set( B, sub );
					}
					//-- fetch sub data
					MPI_Status status;
					for( int r=1; r < MPI.CommWorldSize; ++r )
					{
						const layout3D sub   = full.split(r, MPI.CommWorldSize);
						const size_t   bytes = sub.width.x * sizeof(T);
						for( unit_t z=sub.upper.z;z>=sub.lower.z;--z)
						{
							for(unit_t y=sub.upper.y;y>=sub.lower.y;--y)
							{
								MPI.Recv( &A[z][y][sub.lower.x], bytes, MPI_BYTE, r, tag, MPI_COMM_WORLD, status);
							}
						}
					}
				}
				else
				{
					assert( NULL == pA );
					//-- send sub data
					const layout3D sub   = full.split(MPI.CommWorldRank,MPI.CommWorldSize);
					const size_t   bytes = sub.width.x * sizeof(T);
					for(unit_t z=sub.upper.z;z>=sub.lower.z;--z)
					{
						for( unit_t y=sub.upper.y; y>=sub.lower.y;--y)
						{
							MPI.Send( &B[z][y][sub.lower.x], bytes, MPI_BYTE, 0, tag, MPI_COMM_WORLD );
						}
					}
					
				}
			}
			
			
			template <typename T>
			static inline
			void collect0( array2D<T> *pA, const array2D<T> &B, const layout2D &full )
			{
				static const mpi & MPI = mpi::instance();
				static const int   tag = 0x0C01;
				if( 0 == MPI.CommWorldRank )
				{
					assert( NULL != pA );
					array2D<T> &A = *pA;
					//-- direct copy of B in A
					{
						const layout2D sub = full.split(0, MPI.CommWorldSize );
						A.set( B, sub );
					}
					//-- fetch sub data
					MPI_Status status;
					for( int r=1; r < MPI.CommWorldSize; ++r )
					{
						const layout2D sub   = full.split(r, MPI.CommWorldSize);
						const size_t   bytes = sub.width.x * sizeof(T);
						
						for(unit_t y=sub.upper.y;y>=sub.lower.y;--y)
						{
							MPI.Recv( &A[y][sub.lower.x], bytes, MPI_BYTE, r, tag, MPI_COMM_WORLD, status);
						}
					}
				}
				else
				{
					assert( NULL == pA );
					//-- send sub data
					const layout2D sub   = full.split(MPI.CommWorldRank,MPI.CommWorldSize);
					const size_t   bytes = sub.width.x * sizeof(T);
					
					for( unit_t y=sub.upper.y; y>=sub.lower.y;--y)
					{
						MPI.Send( &B[y][sub.lower.x], bytes, MPI_BYTE, 0, tag, MPI_COMM_WORLD );
					}
					
					
				}
			}
			
		};
		
	}
	
}



#endif
