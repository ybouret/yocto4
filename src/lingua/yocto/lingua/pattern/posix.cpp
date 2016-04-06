#include "yocto/lingua/pattern/posix.hpp"

#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/joker.hpp"

#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        pattern * posix::upper()
        {
            return range::create('A','Z');
        }

        pattern * posix::lower()
        {
            return range::create('a','z');
        }

        pattern * posix::digit()
        {
            return range::create('0','9');
        }

        pattern * posix:: alpha()
        {
            auto_ptr<logical> p( OR::create() );
            p->add( lower() );
            p->add( upper() );
            return p.yield();
        }

        pattern *posix:: alnum()
        {
            auto_ptr<logical> p( OR::create() );
            p->add( digit() );
            p->add( lower() );
            p->add( upper() );
            return p.yield();
        }

        pattern *posix:: xdigit()
        {
            auto_ptr<logical> p( OR::create() );
            p->add( digit() );
            p->add( range::create('a','f' ) );
            p->add( range::create('A','F' ) );
            return p.yield();
        }

        pattern *posix:: blank()
        {
            return logical::among(" \t");
        }

        pattern *posix::space()
        {
            return logical::among(" \t\r\n\v\f");
        }


        pattern *posix::punct()
        {
            return logical::among("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~");
        }

        pattern *posix:: word()
        {
            auto_ptr<logical> p( OR::create() );
            p->add( lower() );
            p->add( upper() );
            p->add( single::create('_') );
            return p.yield();
        }

        pattern *posix:: endl()
        {
            auto_ptr<logical> p( OR::create() );
            p->add( logical::equal("\r\n") );
            p->add( single::create('\n')   );
            p->add( single::create('\r')   );

            return p.yield();
        }

        pattern *posix:: dot()
        {
            auto_ptr<logical> p( NONE::create() );
            p->add( single::create('\n') );
            p->add( single::create('\r') );
            return p.yield();
        }


        pattern *posix:: cstring()
        {
            auto_ptr<logical> p( AND::create() );

            p->add( single::create('"'));
            {
                auto_ptr<logical> q( OR::create() );

                q->add(range::create(32,33));
                q->add(range::create(35,91));
                q->add(range::create(93,126));

                {
                    auto_ptr<logical> esc( AND::create() );
                    esc->add(single::create( 92 ));
                    esc->add(logical::among( "\\\"'nrbtfav" ));
                    q->add(esc.yield());
                }

                {
                    auto_ptr<logical> hex( AND::create() );
                    hex->add(single::create( 92 ));
                    hex->add(single::create( 'x'));
                    hex->add( xdigit() );
                    hex->add( xdigit() );
                    q->add(hex.yield());
                }

                p->add( zero_or_more(q.yield()) );
            }

            p->add( single::create('"'));
            
            return p.yield();

        }
    }

}
