#include "yocto/lingua/lexical/scanner.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        namespace lexical
        {
            
            scanner:: ~scanner() throw() {}
            
#define Y_SCANNER_CTOR()    \
name(id),                   \
line(line_ref),             \
rules(),                    \
pdict(0)
            
            scanner:: scanner( const string &id, size_t &line_ref) :
            Y_SCANNER_CTOR()
            {
                
            }
            
            
            void scanner:: make(const string &label,
                                pattern      *motif,
                                const action &cb)
            {
                assert(motif);
                rules.push_back( rule::create(label, motif, cb, false));
            }
            
            
            p_dict & scanner:: dict()
            {
                if( !pdict.is_valid())
                    pdict.reset( new p_dict() );
                assert(pdict.is_valid());
                return *pdict;
            }
            
            
            void scanner:: reset() throw()
            {
                line = 0;
                for( rule *r = rules.head; r; r=r->next )
                {
                    r->motif->reset();
                }
            }
        }
        
    }
}