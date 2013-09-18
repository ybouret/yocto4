#include "yocto/rx/pattern/posix.hpp"

#include "yocto/rx/pattern/basic.hpp"
#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/pattern/joker.hpp"

#include "yocto/ptr/auto.hpp"

namespace yocto
{
	namespace regex
	{

		pattern * posix:: upper()
		{
			return basic::range::create( 'A', 'Z' );
		}

		pattern * posix:: lower()
		{
			return basic::range::create( 'a', 'z' );
		}

		pattern * posix:: digit()
		{
			return basic::range::create( '0', '9' );
		}

		pattern * posix:: alpha()
		{
			logical::Operator *p = logical::OR::create();
			try {
				*p << lower() << upper();
				return p;
			}
			catch (...) {
				delete p;
				throw;
			}
		}

		pattern * posix:: alnum()
		{
			logical::Operator *p = logical::OR::create();
			try {
				*p << lower() << upper() << digit();
				return p;
			}
			catch (...) {
				delete p;
				throw;
			}
		}

		pattern * posix:: xdigit()
		{
			logical::Operator *p = logical::OR::create();
			try {
				*p << digit() << basic::range::create('A','F') << basic::range::create('a','f');
				return p;
			}
			catch (...) {
				delete p;
				throw;
			}
		}

		pattern * posix:: word()
		{
			logical::Operator *p = logical::OR::create();
			try {
				*p << lower() << upper() << digit() << basic::single::create( '_' );
				return p;
			}
			catch (...) {
				delete p;
				throw;
			}

		}


		pattern * posix:: blank()
		{
			return logical::AMONG( " \t" );
		}

		pattern * posix:: space()
		{
			return logical::AMONG( " \t\r\n\v\f" );
		}

		pattern * posix:: endl()
		{
			logical::Operator *p = logical::OR::create();
			try {
				*p << basic::single::create( '\n' ) << basic::single::create( '\r' ) << logical::EQUAL( "\r\n" );
				return p;
			}
			catch (...) {
				delete p;
				throw;
			}
		}


		pattern *posix:: punct()
		{
			basic::within *p = basic::within::create();
			try
			{
				p->reserve( 32 );
				p->insert(  33,  47 );
				p->insert(  58,  64 );
				p->insert(  91,  96 );
				p->insert( 123, 126 );
			}
			catch(...)
			{
				delete p;
				throw;
			}
			return p;
		}

		pattern * posix::dot()
		{
			auto_ptr<logical::Operator> p( logical::NONE::create() );

			*p << basic::single::create( '\n' );
			*p << basic::single::create( '\r' );

			return p.yield();
		}


		pattern * posix:: cstring()
		{

			auto_ptr<logical::Operator> p( logical::AND::create() );

			*p << basic::single::create( '"' );

			{
				auto_ptr<logical::Operator> q( logical:: OR::create() );

				*q << basic::range::create(32,33);
				//*q << basic::range::create(35,38);
				//*q << basic::range::create(40,91);
                *q << basic::range::create(35,91);
				*q << basic::range::create(93,126);

				{
					auto_ptr<logical:: Operator> esc( logical::AND::create() );
					*esc << basic::single::create( 92 );
					*esc << logical::AMONG( "\\\"'nrbtfav" );
					*q   << esc.yield();
				}

				*p << joker::at_least(q.yield(),0);
			}

			*p << basic::single::create( '"' );

			return p.yield();

		}



	}

}
