#include "yocto/lingua/pattern.hpp"

#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/joker.hpp"

#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{

    namespace lingua
    {
        static inline
        void __load_logical( p_list &ops, ios::istream &fp )
        {
            const size_t n = fp.read<uint32_t>();
            for(size_t i=1;i<=n;++i)
            {
                ops.push_back( pattern::load(fp) );
            }
        }

        pattern * pattern::load(ios::istream &fp)
        {
            const uint32_t uuid = fp.read<uint32_t>();
            switch(uuid)
            {
                    //__________________________________________________________
                    //
                    // basic
                    //__________________________________________________________
                case any1::UUID:
                    return any1::create();

                case single::UUID:
                    return single::create( fp.read<uint8_t>() );

                case range:: UUID: {
                    const size_t lo = fp.read<uint32_t>();
                    const size_t up = fp.read<uint32_t>();
                    return range::create(lo,up);
                }
                    
                    //__________________________________________________________
                    //
                    // joker
                    //__________________________________________________________
                case optional::UUID:
                    return optional::create( pattern::load(fp) );

                case at_least::UUID: {
                    const size_t nmin = fp.read<uint32_t>();
                    return at_least::create( pattern::load(fp), nmin);
                }

                case counting::UUID: {
                    const size_t nmin = fp.read<uint32_t>();
                    const size_t nmax = fp.read<uint32_t>();
                    return counting::create( pattern::load(fp), nmin, nmax);
                }

                    //__________________________________________________________
                    //
                    // logic
                    //__________________________________________________________
                case AND::UUID:
                {
                    auto_ptr<logical> p( AND::create() );
                    __load_logical(p->operands,fp);
                    return p.yield();
                }

                case OR::UUID:
                {
                    auto_ptr<logical> p( OR::create() );
                    __load_logical(p->operands,fp);
                    return p.yield();
                }

                case NONE::UUID:
                {
                    auto_ptr<logical> p( OR::create() );
                    __load_logical(p->operands,fp);
                    return p.yield();
                }

                default:
                    break;
            }
            throw exception("pattern::load(unknown ID)");
        }
    }
}