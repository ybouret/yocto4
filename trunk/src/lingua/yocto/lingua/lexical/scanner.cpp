#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/exception.hpp"

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
mylex(0),                   \
pdict(0),                   \
opsID(0)
            
            scanner:: scanner( const string &id, size_t &line_ref) :
            Y_SCANNER_CTOR()
            {
                
            }
            
            scanner:: scanner( const char *id, size_t &line_ref) :
            Y_SCANNER_CTOR()
            {
                
            }            
            
            const string & scanner:: key() const throw() { return name; }
            
            bool scanner:: has( const string &id ) const throw()
            {
                for( const rule *r = rules.head;r;r=r->next)
                {
                    if( id == r->label)
                        return true;
                }
                return false;
            }
            
            p_dict & scanner:: dict()
            {
                if( !pdict.is_valid())
                    pdict.reset( new p_dict() );
                assert(pdict.is_valid());
                return *pdict;
            }
            
            void scanner:: no_dict() throw()
            {
                pdict.reset(0);
            }
            
            void scanner:: reset() throw()
            {
                for( rule *r = rules.head; r; r=r->next )
                {
                    r->motif->reset();
                }
            }
            ////////////////////////////////////////////////////////////////////
            //
            // simple actions
            //
            ////////////////////////////////////////////////////////////////////
            bool scanner:: __discard(const token &) throw() {return false;}
            bool scanner:: __forward(const token &) throw() {return true;}
            bool scanner:: __newline(const token &) throw()       {++line;return true; }
            bool scanner:: __no_endl(const token &) throw()       {++line;return false;}
            
            ////////////////////////////////////////////////////////////////////
            //
            // make
            //
            ////////////////////////////////////////////////////////////////////
            void scanner:: make(const string &label,
                                pattern      *motif,
                                const action &cb)
            {
                assert(motif);
                if( has(label) )
                {
                    delete motif;
                    throw exception("<%s>.make(mutliple '%s;)", name.c_str(), label.c_str());
                }
                rules.push_back( rule::create(label, motif, cb, false));
            }
            
        }
        
    }
}

