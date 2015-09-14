#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {

        pattern * pattern::load( ios::istream &fp )
        {
            const uint32_t tag = fp.read<uint32_t>("pattern tag");
            switch(tag)
            {
                    //__________________________________________________________
                    //
                    // Basic
                    //__________________________________________________________
                case any1  ::UUID:
                case single::UUID:
                case range ::UUID:

                    //__________________________________________________________
                    //
                    // Joker
                    //__________________________________________________________
                case optional::UUID:
                case at_least::UUID:
                case counting::UUID:

                    //__________________________________________________________
                    //
                    // logic
                    //__________________________________________________________
                    

                default:
                    break;
            }
            throw exception("unexpected pattern tag=%0x08x", unsigned(tag));
        }

    }

}


