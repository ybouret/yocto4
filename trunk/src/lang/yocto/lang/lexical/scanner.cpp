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
                no_dict();
            }
            
#define Y_SCANNER_CTOR() \
name(id), line(line_ref), rules(), dict_(0), echo(false)
            
            scanner:: scanner( const string &id, int &line_ref ) :
            Y_SCANNER_CTOR()
            {
            }
            
            scanner:: scanner( const char *id, int &line_ref ) :
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
                for(const rule *i=rules.head;i;i=i->next)
                {
                    if(i->label ==  r->label )
                    {
                        delete r;
                        throw exception("lexical.scanner<%s>.append(multiple rule '%s')", name.c_str(), i->label.c_str());
                    }
                }
                rules.push_back(r);
            }

            void scanner:: reset() throw()
            {
                line = 1;
                for(rule *r=rules.head;r;r=r->next)
                {
                    r->motif->reset();
                }
            }
            
            p_dict & scanner:: dict()
            {
                if(!dict_) dict_ = new p_dict();
                return *dict_;
            }

            void scanner:: no_dict() throw()
            {
                if(dict_)
                {
                    delete dict_;
                    dict_ = 0;
                }
            }
            
            
        }
    }
}

