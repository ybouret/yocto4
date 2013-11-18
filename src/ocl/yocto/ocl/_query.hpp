//! \file _query.hpp
/**
	wrappers around clGet[something]...
 */

#ifndef YOCTO_OPENCL_QUERY_INCLUDED
#define YOCTO_OPENCL_QUERY_INCLUDED 1

#include "yocto/code/round.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ocl/exception.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		namespace Core
		{
			//======================================================================
			//
			// clGetXXXInfo wrappers
			//
			//======================================================================
			//! read a value
			/**
			 \param func clGet[Platform|Device|....]Info
			 \param id   cl_[platform|device|...]_id
			 \param info cl_[platofrm|device|...]_info
			 \param buf  user's buffer
			 \param len  user's buffer length
			 \param tag  associated tag
			 */
			template <typename FUNC, typename TYPE_ID, typename TYPE_INFO >
			inline void ReadInfoValue( FUNC func, TYPE_ID & id, TYPE_INFO info, void *buf, const size_t len, const char *tag )
			{
				assert( func != NULL );
				assert( tag  != NULL );
				const cl_int err = func( id, info, len, buf, NULL );
				if( CL_SUCCESS != err ) throw Exception( err, "%s", tag );
			}
			
			//! read and convert a value
			/**
			 type2type is used to force hint the template instance.s
			 \param func clGet[Platform|Device|....]Info
			 \param id   cl_[platform|device|...]_id
			 \param info cl_[platofrm|device|...]_info
			 \param tag  associated tag
			 */
			template <typename T, typename FUNC, typename TYPE_ID, typename TYPE_INFO >
			inline T GetInfoValue( type2type<T>, FUNC fn, TYPE_ID & id, TYPE_INFO info, const char *tag )
			{
				T ans;
				ReadInfoValue<FUNC,TYPE_ID,TYPE_INFO>( fn, id, info, & ans, sizeof(T), tag );
				return ans;
			}
			
			//! read a string in two stages
			/**
			 \param func clGet[Platform|Device|....]Info
			 \param id   cl_[platform|device|...]_id
			 \param info cl_[platform|device|...]_info
			 \param tag  associated tag
			 */
			template <typename FUNC, typename TYPE_ID, typename TYPE_INFO>
			inline string GetInfoString( FUNC fn, TYPE_ID & id, TYPE_INFO info, const char *tag )
			{
				size_t        len = 0;
				{
					const cl_int err = fn( id, info, 0, NULL, & len );
					if( CL_SUCCESS != err )
						throw Exception( err, "%s: reading length", tag);
				}
				
				ArrayOf<char>::Type tmp( YOCTO_MEMALIGN( len+1 ) );
				size_t              chk = 0;
				{
					const cl_int err = fn( id, info, len, tmp.rw(), & chk );
					if( CL_SUCCESS != err )
						throw Exception( err, "%s (%u->%u)", tag, unsigned(len), unsigned(chk) );
				}
				const string ans( tmp(), max_of<size_t>(len,1) - 1 );
				return ans;
			}
			
			size_t  CountExtensions( const string & EXTENSIONS ) throw();
			void    ParseExtensions( const string & EXTENSIONS, const Extensions &ext ) throw();

			
		}
		
	}
	
}

#endif
