#include "yocto/lang/pattern/posix.hpp"

#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/pattern/joker.hpp"

#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
    {

        pattern * posix:: upper()
        {
            return range::create( 'A', 'Z' );
        }

        pattern * posix:: lower()
        {
            return range::create( 'a', 'z' );
        }

        pattern * posix:: digit()
        {
            return range::create( '0', '9' );
        }

        pattern * posix:: alpha()
        {
            auto_ptr<logical> p( OR::create() );

            p->append(lower());
            p->append(upper());

            return p.yield();
        }

        pattern * posix:: alnum()
        {
            logical *p = OR::create();
            try
            {
                p->append(lower());
                p->append(upper());
                p->append(digit());
                return p;
            }
            catch (...) {
                delete p;
                throw;
            }
        }

        pattern * posix:: xdigit()
        {
            logical *p = OR::create();
            try {
                p->append( digit() );
                p->append( range::create( 'A','F') );
                p->append( range::create( 'a','f') );
                return p;
            }
            catch (...) {
                delete p;
                throw;
            }
        }

        pattern * posix:: word()
        {
            logical  *p = OR::create();
            try {
                p->append( lower() );
                p->append( upper() );
                p->append( digit() );
                p->append( single::create( '_') );
                return p;
            }
            catch (...) {
                delete p;
                throw;
            }

        }


        pattern * posix:: blank()
        {
            return logical::among( " \t" );
        }

        pattern * posix:: space()
        {
            return logical::among( " \t\r\n\v\f" );
        }

        pattern * posix:: endl()
        {
            logical *p = OR::create();
            try {
                p->append( single::create('\n')   );
                p->append( single::create('\r')   );
                p->append( logical::equal("\r\n") );
                return p;
            }
            catch (...) {
                delete p;
                throw;
            }
        }


        pattern *posix:: punct()
        {
            return logical::among("!\"#$%&'()*+,\\-./:;<=>?@[]^_`{|}~");
#if 0
            multi *p = multi::create();
            try
            {
                p->codes.reserve( 32 );
                p->append(  33,  47 );
                p->append(  58,  64 );
                p->append(  91,  96 );
                p->append( 123, 126 );
            }
            catch(...)
            {
                delete p;
                throw;
            }
            return p;
#endif
        }

        pattern * posix::dot()
        {
            auto_ptr<logical> p( NOT::create() );

            p->append( single::create('\n') );
            p->append( single::create('\r') );


            return p.yield();
        }


        pattern * posix:: cstring()
        {

            auto_ptr<logical> p( AND::create() );

            p->append( single::create('"'));
            {
                auto_ptr<logical> q( OR::create() );

                q->append(range::create(32,33));
                q->append(range::create(35,91));
                q->append(range::create(93,126));

                {
                    auto_ptr<logical> esc( AND::create() );
                    esc->append(single::create( 92 ));
                    esc->append(logical::among( "\\\"'nrbtfav" ));
                    q->append(esc.yield());
                }

                {
                    auto_ptr<logical> hex( AND::create() );
                    hex->append(single::create( 92 ));
                    hex->append(single::create( 'x'));
                    hex->append( xdigit() );
                    hex->append( xdigit() );
                    q->append(hex.yield());
                }

                p->append( zero_or_more(q.yield()) );
            }

            p->append( single::create('"'));

            return p.yield();

        }

        pattern *posix::base64()
        {
            auto_ptr<logical> p( AND::create() );

            {
                logical *q = OR::create();
                p->append(q);
                q->append( range::create('A','B') );
                q->append( range::create('a','b') );
                q->append( range::create('0','9') );
                q->append( single::create('+') );
                q->append( single::create('/') );
            }

            {
                pattern *q = counting::create(single::create('='),0,2);
                p->append(q);
            }
            
            
            return p.yield();
        }
        
        
    }
}

