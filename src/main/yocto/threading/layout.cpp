#include "yocto/threading/layout.hpp"

#include "yocto/string/env.hpp"
#include "yocto/exception.hpp"
#include "yocto/sys/hw.hpp"
#include "yocto/code/utils.hpp"

#include <iostream>
#include <cstdlib>

namespace yocto
{
    
	namespace threading
	{
		//======================================================================
		//
		// layout
		//
		//======================================================================
		static inline bool __next_field( char * &field )
		{
			assert(field!=NULL);
			while( field[0] != '\0' )
			{
				if( !isspace( field[0] ) )
				{
					if( ',' == field[0] )
					{
						++field;
						return true;
					}
					else
					{
						throw exception("Invalid char '%c' in value", field[0]);
					}
					
				}
				++field;
			}
			return false;
		}
		
		layout:: layout( ) :
		size( hardware::nprocs() ),
		root(0),
        ncpu(size),
        scan(ncpu)
		{
			const string name = "YOCTO_THREADING";
			string       value;
			if( environment::get( value, name ) )
			{
				//--------------------------------------------------------------
				// #nproc
				//--------------------------------------------------------------
				const char *text  = value.c_str();
				char       *field = NULL;
				const long  nproc = strtol(text, &field, 10 );
                
				if( nproc <= 0 )
					throw exception("Invalid #CPU");
				
                (size_t&)size = size_t(nproc);
				
                //--------------------------------------------------------------
				// offset, optional
				//--------------------------------------------------------------
				if( __next_field(field) )
				{
					text = field;
					const long ibase = strtol(text, &field, 10 );
					if( ibase < 0 )
						throw exception("Invalid base CPU");
					(size_t&)root = size_t(ibase);
				}
                
                if(root>=ncpu)
                    throw exception("Invalid Root=%u", unsigned(root));
                
                (size_t &)scan = min_of(size,ncpu-root);

                
			}
			std::cerr << "[threading::layout=" << size << "," << root << ",scan=" << scan << "]" << std::endl;
		}
		
        layout:: layout( size_t num_threads, size_t thread_offset ) :
        size( num_threads   ),
        root( thread_offset ),
        ncpu( hardware::nprocs() ),
        scan( min_of(size,ncpu-root) )
        {
            if(num_threads<=0)
                throw exception("Invalid #CPU");
            if(root>=ncpu)
                throw exception("Invalid Root=%u", unsigned(root));
        }
        
		layout:: layout( const layout &other ) throw() :
		size( other.size ),
		root( other.root ),
        ncpu( other.ncpu ),
        scan( other.scan )
		{
		}
		
		layout:: ~layout() throw()
		{
		}
		
        size_t layout:: cpu_index_of(size_t iThread) const throw()
        {
            
            assert(size>0);
            assert(scan>0);
            assert(root<size);
            const size_t j = root + ( iThread % scan );
            std::cerr << "              |_ assign " << iThread << " on CPU #" << j << std::endl;
            return j;
        }
		
	}
	
}
