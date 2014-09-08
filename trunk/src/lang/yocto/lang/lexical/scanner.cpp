#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            scanner:: ~scanner() throw()
            {
            }
            
#define Y_SCANNER_CTOR() \
name(id), line(line_ref), rules(), mylex(0), dict( &dict_ref), ibck(0)
            
            scanner:: scanner( const string &id, int &line_ref, const p_dict &dict_ref ) :
            Y_SCANNER_CTOR()
            {
            }
            
            scanner:: scanner( const char *id, int &line_ref, const p_dict &dict_ref  ) :
            Y_SCANNER_CTOR()
            {
            }
            
            const string & scanner:: key() const throw()
            {
                return name;
            }
            
            void scanner:: append( rule *r )
            {
                assert(r);
                // check no empty
                first_chars fc;
                try
                {
                    assert(r->motif);
                    r->motif->detect(fc);
                }
                catch(...)
                {
                    delete r;
                    throw;
                }
                if(fc.accept_empty)
                {
                    throw exception("<%s>.append('%s' accept empty expression!)", name.c_str(), r->label.c_str());
                }
            
                // check no multiple
                for(const rule *i=rules.head;i;i=i->next)
                {
                    if(i->label ==  r->label )
                    {
                        delete r;
                        throw exception("<%s>.append(multiple rule '%s')", name.c_str(), i->label.c_str());
                    }
                }
                
                // ok
                rules.push_back(r);
            }
            
            void scanner:: append(const string    &label,
                                  pattern         *motif,
                                  const action    &which,
                                  const rule::kind flag)
            {
                assert(motif);
                append( rule::create(label,motif,which,flag) );
            }
            
            void scanner:: append(const char      *label,
                                  pattern         *motif,
                                  const action    &which,
                                  const rule::kind flag)
            {
                auto_ptr<pattern> q( motif );
                const string Label(label);
                append( Label, q.yield(), which, flag);
            }
            
            void scanner:: reset() throw()
            {
                for(rule *r=rules.head;r;r=r->next)
                {
                    r->motif->reset();
                }
            }
            
            
        }
    }
}

