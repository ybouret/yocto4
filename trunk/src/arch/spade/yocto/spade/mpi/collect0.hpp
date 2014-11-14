#ifndef YOCTO_SPADE_MPI_COLLECT0_INCLUDED
#define YOCTO_SPADE_MPI_COLLECT0_INCLUDED 1

#include "yocto/spade/array3d.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        struct mpi_collect0
        {
            
            //! process
            /**
             \param pA   on master: an array with 'full' layout, NULL otherwise
             \param B    an array with a splitted layout.
             \param full the original full layout
             */
            template <typename T> static inline
            void get(const mpi        &MPI,
                     array1D<T>       *pA,
                     const array1D<T> &B,
                     const layout1D   &full )
            {
                static const int   tag = 0xC011;
                const int rank =  MPI.CommWorldRank;
                const int size =  MPI.CommWorldSize;
				if( 0 == rank )
				{
					assert( NULL != pA );
					array1D<T> &A = *pA;
                    //----------------------------------------------------------
					//-- direct copy of B in A
                    //----------------------------------------------------------
					{
						const layout1D sub = full.split(0,size,0);
						A.set( B, sub );
					}
                    
                    //----------------------------------------------------------
					//-- fetch sub data
                    //----------------------------------------------------------
					MPI_Status status;
					for( int r=1; r < MPI.CommWorldSize; ++r )
					{
						const layout1D sub   = full.split(r,size,0);
						const size_t   bytes = sub.width * sizeof(T);
						
                        MPI.Recv( &A[sub.lower], bytes, MPI_BYTE, r, tag, MPI_COMM_WORLD, status);
					}
				}
				else
				{
					assert( NULL == pA );
                    //----------------------------------------------------------
					//-- send sub data
                    //----------------------------------------------------------
					const layout1D sub   = full.split(rank,size,0);
					const size_t   bytes = sub.width * sizeof(T);
					
                    MPI.Send( &B[sub.lower], bytes, MPI_BYTE, 0, tag, MPI_COMM_WORLD );
				}
            }
            
            
            //! 2D collect a global array in rank 0: SPLITTED ON Y
			template <typename T> static inline
			void get( const mpi &MPI, array2D<T> *pA, const array2D<T> &B, const layout2D &full)
			{
				static const int   tag = 0xC012;
                const int rank =  MPI.CommWorldRank;
                const int size =  MPI.CommWorldSize;
				if( 0 == rank )
				{
					assert( NULL != pA );
					array2D<T> &A = *pA;
                    //----------------------------------------------------------
					//-- direct copy of B in A for rank 0
                    //----------------------------------------------------------
					{
						const layout2D sub = full.split(0, MPI.CommWorldSize,on_y);
						A.set( B, sub );
					}
                    
                    //----------------------------------------------------------
					//-- fetch sub data
                    //----------------------------------------------------------
					MPI_Status status;
					for( int r=1; r < MPI.CommWorldSize; ++r )
					{
						const layout2D sub   = full.split(r,size,on_y);
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
                    //----------------------------------------------------------
					//-- send sub data
                    //----------------------------------------------------------
					const layout2D sub   = full.split(rank,size,on_y);
					const size_t   bytes = sub.width.x * sizeof(T);
					
					for( unit_t y=sub.upper.y; y>=sub.lower.y;--y)
					{
						MPI.Send( &B[y][sub.lower.x], bytes, MPI_BYTE, 0, tag, MPI_COMM_WORLD );
					}
					
				}
			}
            
            //! 3D collect a global array in rank 0
			template <typename T> static inline
			void collect0( const mpi & MPI, array3D<T> *pA, const array3D<T> &B, const layout3D &full )
			{
				static const int   tag = 0xC013;
                const int rank =  MPI.CommWorldRank;
                const int size =  MPI.CommWorldSize;
                const dimension_t dim = on_z;
				if( 0 == rank )
				{
					assert( NULL != pA );
					array3D<T> &A = *pA;
					//-- direct copy of B in A
					{
						const layout3D sub = full.split(0, size, dim );
						A.set( B, sub );
					}
					//-- fetch sub data
					MPI_Status status;
					for( int r=1; r < MPI.CommWorldSize; ++r )
					{
						const layout3D sub   = full.split(r, size,dim);
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
					const layout3D sub   = full.split(rank,size,dim);
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
