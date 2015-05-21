#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/lang/pattern/logic.hpp"

#include "yocto/exception.hpp"
#include "yocto/ios/imstream.hpp"

namespace yocto
{
    namespace lang
    {
        
        static inline pattern *load_choice( ios::istream &fp, choice *p )
        {
            try
            {
                const uint32_t n = fp.read<uint32_t>("choice.chars");
                for(uint32_t i=1;i<=n;++i)
                {
                    p->chars.insert( fp.read<code_type>("choice.char") );
                }
                return p;
            }
            catch(...)
            {
                delete p;
                throw;
            }
        }
        
        static inline pattern *load_logical( ios::istream &fp, logical *p )
        {
            try
            {
                const uint32_t n = fp.read<uint32_t>("logical.operands");
                for(uint32_t i=1;i<=n;++i)
                {
                    p->append( pattern::load(fp) );
                }
                return p;
            }
            catch(...)
            {
                delete p;
                throw;
            }
        }
        
        
        pattern *pattern::load(ios::istream &fp)
        {
            const uint32_t tag = fp.read<uint32_t>();
            switch(tag)
            {
                    //----------------------------------------------------------
                    //
                    // BASIC
                    //
                    //----------------------------------------------------------
                case any1::tag:   return any1::create();
                case single::tag: return single::create(fp.read<code_type>("single value"));
                case range::tag:{
                    const code_type lo = fp.read<code_type>("range.lower");
                    const code_type up = fp.read<code_type>("range.upper");
                    return range::create(lo,up);
                };
                    
                case within::tag: return load_choice( fp, within::create() );
                case none::tag:   return load_choice( fp, none::create()   );
                    
                    //----------------------------------------------------------
                    //
                    // JOKER
                    //
                    //----------------------------------------------------------
                case optional::tag: return optional::create( pattern::load(fp) );
                case at_least::tag: {
                    const uint32_t value = fp.read<uint32_t>("at_least.value");
                    return at_least::create( pattern::load(fp), value);
                };
                case counting::tag: {
                    const uint32_t n = fp.read<uint32_t>("counting.n");
                    const uint32_t m = fp.read<uint32_t>("counting.m");
                    return counting::create(pattern::load(fp),n, m);
                };
                    //----------------------------------------------------------
                    //
                    // LOGICAL
                    //
                    //----------------------------------------------------------
                case AND::tag: return load_logical(fp,  AND::create() );
                case OR:: tag: return load_logical(fp,  OR:: create() );
                case NOT::tag: return load_logical(fp,  NOT::create() );
                    
                default:
                    break;
            }
            throw exception("pattern.load(unknown tag %08x <=> [%s])",tag,fourcc_of(tag));
            
        }
        
        
        pattern * pattern::load( const memory::ro_buffer &buff )
        {
            ios::imstream fp(buff);
            return pattern::load(fp);
        }
    }
}
