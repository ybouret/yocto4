#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/joker.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
     
        
        static inline pattern *__load_choice( choice *p, ios::istream &fp )
        {
            auto_ptr<pattern> q(p);
            const size_t n = fp.read<uint32_t>();
            p->chars.reserve(n);
            for(size_t i=1; i <=n; ++i)
            {
                p->append( fp.read<char>() );
            }
            return q.yield();
        }
        
        static inline pattern * __load_logical( logical *p, ios::istream &fp )
        {
            auto_ptr<pattern> q(p);
            const size_t n = fp.read<uint32_t>();
            for(size_t i=1; i <= n; ++i)
            {
                p->append( pattern::load(fp) );
            }
            return q.yield();
        }
        
        pattern * pattern::load( ios::istream &fp )
        {
            //------------------------------------------------------------------
            // read type
            //------------------------------------------------------------------
            const uint32_t t = fp.read<uint32_t>();
            
            //------------------------------------------------------------------
            // selective loading
            //------------------------------------------------------------------
            switch(t)
            {
                    
                    //----------------------------------------------------------
                    //
                    // BASIC
                    //
                    //----------------------------------------------------------
                case any1::tag:
                    return any1::create();
                    
                case single::tag:
                    return single::create( fp.read<char>() );
                    
                case range::tag:
                {
                    const int a = fp.read<int32_t>();
                    const int b = fp.read<int32_t>();
                    return range::create(a, b);
                }
                  
                case within::tag:
                    return __load_choice( within::create(),fp );
                    
                case none::tag:
                    return __load_choice( none::create(), fp );
                    
                    //----------------------------------------------------------
                    //
                    // LOGIC
                    //
                    //----------------------------------------------------------
                case AND::tag:
                    return __load_logical( AND::create(), fp);
                
                case OR::tag:
                    return __load_logical( OR::create(), fp);
                
                case NOT::tag:
                    return __load_logical( NOT::create(), fp);
                    
                    
                    //----------------------------------------------------------
                    //
                    // JOKER
                    //
                    //----------------------------------------------------------
                case optional::tag:
                    return optional::create( pattern::load(fp) );
                    
                case at_least::tag: {
                    const size_t n = fp.read<uint32_t>();
                    return at_least::create( pattern::load(fp), n);
                }
                    
                case counting::tag: {
                    const size_t a = fp.read<uint32_t>();
                    const size_t b = fp.read<uint32_t>();
                    return counting::create( pattern::load(fp), a, b);
                }
                    
                    
                    //----------------------------------------------------------
                    //
                    // ERROR !
                    //
                    //----------------------------------------------------------
                default:
                    break;
            }
            
            throw exception("lingua::load(unknown tag %08x)", unsigned(t));
        }
        
        
    }
}
