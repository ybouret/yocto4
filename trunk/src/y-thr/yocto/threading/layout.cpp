#include "yocto/threading/layout.hpp"

#include "yocto/string/env.hpp"
#include "yocto/exception.hpp"
#include "yocto/sys/hw.hpp"

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
		root(0)
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
					throw exception("Invalid number of CPU");
				
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
				
			}
			std::cerr << "[threading::layout=" << size << "," << root << "]" << std::endl;
		}
		
        layout:: layout( size_t num_threads, size_t thread_offset ) :
        size( num_threads   ),
        root( thread_offset )
        {
            
        }
        
		layout:: layout( const layout &other ) throw() :
		size( other.size ),
		root( other.root )
		{
		}
		
		layout:: ~layout() throw()
		{
		}
		
		
	}
	
}
