#ifndef YOCTO_SWAMP_MPI_INCLUDED
#define YOCTO_SWAMP_MPI_INCLUDED 1


#include "yocto/swamp/dataspace.hpp"
#include "yocto/swamp/array3d.hpp"
#include "yocto/mpi/mpi.hpp"

namespace yocto 
{
    
    namespace swamp
    {
        
        //! MPI related operations
        struct _mpi
        {
            
            //! initialize the exchanges
            template <typename LAYOUT> inline static 
            void init_exchange( mpi &MPI, dataspace<LAYOUT> &D, mpi::Requests &requests )
            {
                static const int tag = 0xCALL;
                assert( requests.count == D.num_requests() );
                const linear_handles &handles = D.handles();
                
                //==============================================================
                // exchange local ghosts
                //==============================================================
                for( size_t i= D.local_ghosts_count(); i>0; --i )
                {
                    D.__local_ghosts(i).transfer( handles );
                }
                
                //==============================================================
                // start exchange of async ghost
                //==============================================================
                size_t iRequest = 0;
                for( size_t i = D.async_ghosts_count(); i>0; --i)
                {
                    async_ghosts &g = D.__async_ghosts(i);
                    assert(g.peer>=0);
                    assert(g.peer<MPI.CommWorldSize);
                    assert(g.length>0);
                    assert(g.inner_buf!=NULL);
                    assert(g.outer_buf!=NULL);
                    
                    //----------------------------------------------------------
                    //  Non blocking recv from peer
                    //----------------------------------------------------------
                    MPI.Irecv(g.outer_buf, g.length, MPI_BYTE , g.peer, tag, MPI_COMM_WORLD, requests[iRequest++]);
                    assert(iRequest<requests.count);
                    //----------------------------------------------------------
                    //  Fill inner ghost with data
                    //----------------------------------------------------------
                    g.store_inner( handles );
                    
                    //----------------------------------------------------------
                    //  Non blocking send to peer
                    //----------------------------------------------------------
                    MPI.Isend(g.inner_buf, g.length, MPI_BYTE, g.peer, tag, MPI_COMM_WORLD, requests[iRequest++]);
                    assert( iRequest<=requests.count );
                }
                assert( requests.count == iRequest );
            }
            
            //! wait for all exchanges to be done
            template <typename LAYOUT> inline static 
            void wait_exchange( mpi &MPI, dataspace<LAYOUT> &D, mpi::Requests &requests)
            {
                assert( requests.count == D.num_requests() );
                const linear_handles &handles = D.handles();
                MPI.Waitall(requests);
                for( size_t i = D.async_ghosts_count(); i>0; --i)
                {
                    async_ghosts &g = D.__async_ghosts(i);
                    assert(g.peer>=0);
                    assert(g.peer<MPI.CommWorldSize);
                    assert(g.length>0);
                    assert(g.inner_buf!=NULL);
                    assert(g.outer_buf!=NULL);
                    
                    //----------------------------------------------------------
                    //  Fill dataspace with outer ghost data
                    //----------------------------------------------------------
                    g.query_outer(handles);
                }
            }
            
            //! 2D collect a global array in rank 0
			template <typename T>
			static inline
			void collect0( const mpi &MPI, array2D<T> *pA, const array2D<T> &B, const layout2D &full, size_t dim = 1)
			{
				static const int   tag = 0xC012;
				if( 0 == MPI.CommWorldRank )
				{
					assert( NULL != pA );
					array2D<T> &A = *pA;
					//-- direct copy of B in A
					{
						const layout2D sub = full.split(0, MPI.CommWorldSize,dim );
						A.set( B, sub );
					}
                    
					//-- fetch sub data
					MPI_Status status;
					for( int r=1; r < MPI.CommWorldSize; ++r )
					{
						const layout2D sub   = full.split(r, MPI.CommWorldSize,dim);
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
					const layout2D sub   = full.split(MPI.CommWorldRank,MPI.CommWorldSize,dim);
					const size_t   bytes = sub.width.x * sizeof(T);
					
					for( unit_t y=sub.upper.y; y>=sub.lower.y;--y)
					{
						MPI.Send( &B[y][sub.lower.x], bytes, MPI_BYTE, 0, tag, MPI_COMM_WORLD );
					}
					
					
				}
			}

            
            //! 3D collect a global array in rank 0
			template <typename T>
			static inline
			void collect0( const mpi & MPI, array3D<T> *pA, const array3D<T> &B, const layout3D &full, size_t dim = 2 )
			{
				static const int   tag = 0xC013;
				if( 0 == MPI.CommWorldRank )
				{
					assert( NULL != pA );
					array3D<T> &A = *pA;
					//-- direct copy of B in A
					{
						const layout3D sub = full.split(0, MPI.CommWorldSize, dim );
						A.set( B, sub );
					}
					//-- fetch sub data
					MPI_Status status;
					for( int r=1; r < MPI.CommWorldSize; ++r )
					{
						const layout3D sub   = full.split(r, MPI.CommWorldSize,dim);
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
					const layout3D sub   = full.split(MPI.CommWorldRank,MPI.CommWorldSize,dim);
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
			
			
			
            
        };
        
    }
}
#endif
