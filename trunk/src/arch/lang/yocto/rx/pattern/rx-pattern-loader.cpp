#include "yocto/rx/pattern.hpp"
#include "yocto/ios/istream.hpp"
#include "yocto/exception.hpp"

#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/pattern/joker.hpp"

#include "yocto/ptr/auto.hpp"

namespace yocto { 
	
	namespace regex {
		
		
		static inline void __load( p_list &ops, ios::istream &input )
		{
			const size_t n = input.read<uint16_t>();
			for( size_t i=0; i < n; ++i )
			{
				ops.push_back( pattern::load(input) );
			}
		}
		
		
		pattern *pattern:: load( ios::istream &input )
		{
			
			char C;
			while( input.query(C) )
			{
				input.store(C);
				const uint32_t id = input.read<uint32_t>();
				switch( id )
				{
					case basic::any1::id:
						return basic::any1::create();
						
					case basic::single::id:
						return basic::single::create( input.read<char>() );
						
					case basic::range::id: {
						const uint8_t lo = input.read<uint8_t>();
						const uint8_t hi = input.read<uint8_t>();
						return basic::range::create( lo, hi );
					}
						
					case basic::within::id: {
						const size_t            n = input.read<uint16_t>();
						auto_ptr<basic::within> p( basic::within::create() );
						p->reserve(n);
						for( size_t i=0; i < n; ++i )
						{
							p->insert( input.read<uint8_t>() );
						}
						return p.yield();
					}
						
						
					case logical::AND::id: {
						auto_ptr<logical::Operator> p( logical::AND::create() );
						__load( p->operands, input );
						return p.yield();
					}
						
					case logical::OR::id: {
						auto_ptr<logical::Operator> p( logical::OR::create() );
						__load( p->operands, input );
						return p.yield();
					}
						
					case logical::NONE::id: {
						auto_ptr<logical::Operator> p( logical::NONE::create() );
						__load( p->operands, input );
						return p.yield();
					}
						
					case joker::_at_least: {
						const size_t num = input.read<uint16_t>();
						return joker::at_least( pattern::load(input), num );
					}
						
					case joker::_counting: {
						const size_t nmin = input.read<uint16_t>();
						const size_t nmax = input.read<uint16_t>();
						return joker::counting( pattern::load(input), nmin, nmax );
					}
						
						
					default:
						throw exception( "unknown regex::pattern identifier" );
				}
			}
			return NULL;
		}
		
	}	
}
