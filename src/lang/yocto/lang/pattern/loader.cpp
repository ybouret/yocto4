#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/string/base64.hpp"

namespace yocto
{
    namespace lang
    {

        static inline
        pattern *fill_logical( logical *motif, ios::istream &fp )
        {
            auto_ptr<logical> p(motif);
            const uint32_t n = fp.read<uint32_t>("logical #ops");
            for(size_t i=0;i<n;++i)
            {
                p->append( pattern::load(fp) );
            }
            return p.yield();
        }
        
        pattern * pattern::load( ios::istream &fp )
        {
            const uint32_t tag = fp.read<uint32_t>("pattern tag");
            switch(tag)
            {
                    //__________________________________________________________
                    //
                    // Basic
                    //__________________________________________________________
                case any1  ::UUID: return any1::create();
                case single::UUID: return single::create( fp.read<code_t>("single code") );
                case range ::UUID: {
                    const code_t lower = fp.read<code_t>("range lower");
                    const code_t upper = fp.read<code_t>("range upper");
                    return range::create(lower, upper);
                }

                    //__________________________________________________________
                    //
                    // Joker
                    //__________________________________________________________
                case optional::UUID: return optional::create( load(fp) );
                case at_least::UUID: {
                    const uint32_t n = fp.read<uint32_t>("at_least count");
                    return at_least::create( load(fp), n);
                }
                case counting::UUID: {
                    const uint32_t lower = fp.read<uint32_t>("counting lower");
                    const uint32_t upper = fp.read<uint32_t>("counting upper");
                    return counting::create(load(fp), lower, upper);
                }

                    //__________________________________________________________
                    //
                    // Logic
                    //__________________________________________________________
                case AND::UUID: return fill_logical( AND::create(), fp);
                case OR ::UUID: return fill_logical( OR:: create(), fp);
                case NOT::UUID: return fill_logical( NOT::create(), fp);
                    
                default:
                    break;
            }
            throw exception("unexpected pattern tag=%0x08x", unsigned(tag));
        }

        pattern *pattern:: load( const void *data, const size_t size )
        {
            ios::imstream fp(data,size);
            return load(fp);
        }
        
        pattern *pattern:: load( const memory::ro_buffer &buff)
        {
            return load(buff.ro(),buff.length());
        }
        
        pattern *pattern:: load64( const memory::ro_buffer &buff )
        {
            static base64 &Base64 = base64::instance();
            const string ebin = Base64.Decode(buff);
            return load(ebin);
        }

        
    }

}


