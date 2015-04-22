#include "yocto/lang/pattern/joker.hpp"

namespace yocto
{
    namespace lang
    {

        joker:: ~joker() throw()
        {
            assert(jk);
            delete jk;
            jk = 0;
        }


        joker:: joker( uint32_t ID, pattern *p) throw() : pattern(ID), jk(p) {}

       
      

    }
}