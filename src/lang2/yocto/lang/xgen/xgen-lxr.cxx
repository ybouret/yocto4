#include "yocto/lang/xgen/xgen.hxx"
#include "yocto/exception.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"
#include "yocto/lang/lexical/plugin/rstring.hpp"

#include <iostream>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {


            void xgen:: process_lxr__level1(const xnode *node)
            {
                assert(node);
                assert("LXR" == node->label);
                assert(!node->terminal);
                assert(2==node->children().size);
                const string code = node->children().head->content();
                const xnode *data = node->children().tail;
                std::cerr << "\t\tcode='" << code << "'" << std::endl;

                //______________________________________________________________
                //
                // check reserved words
                //______________________________________________________________
                const size_t nsub = node->children().size-1;
                switch( cmph(code) )
                {

                    case 0: assert("@drop"==code);
                    {
                        const string          l = code + vformat("/%u",++jndx);
                        const lexical::action a(& xprs->scanner, & lexical::scanner::discard);
                        xprs->scanner.make(l, compile_lexical(data,nsub), a);
                    } return;

                    case 1: assert("@endl"==code);
                    {
                        const string          l = code + vformat("/%u",++jndx);
                        const lexical::action a(& xprs->scanner, & lexical::scanner::newline);
                        xprs->scanner.make(l, compile_lexical(data,nsub), a);
                    } return;

                    case 2: assert("@comment"==code);
                    {
                        const string l      = code + vformat("/%u",++jndx);
                        const string marker = data->content();
                        xprs->load<lexical::comment>(code.c_str(), marker.c_str());
                    }  return;


                    default: assert(-1==cmph(code));

                        break;
                }

                //______________________________________________________________
                //
                // try a plugin: a terminal will be created
                //______________________________________________________________
                const string plug_name = data->content();
                std::cerr << "\t\tplugin: " << code << " ==> " << plug_name << std::endl;
                const string term_name = code;
                ((string&)term_name).skip(1);
                
                const char *termID = term_name.c_str();
                if( "cstring" == plug_name )
                {
                    terminal &r = xprs->term<lexical::cstring>(termID);
                    if(!rxp.insert(&r))
                    {
                        throw exception("%s: unexpected failure for plugin '%s' insertion", name, termID);
                    }
                    return;
                }

                throw exception("%s: no plugin '%s'", name, plug_name.c_str());

            }

            pattern *xgen:: compile_lexical(const xnode *node,const size_t nsub)
            {
                assert(node);
                assert(nsub>0);
                if(1==nsub)
                {
                    return compile_lexical(node);
                }
                else
                {
                    auto_ptr<lang::logical> p( AND::create() );
                    while(node)
                    {
                        p->append(compile_lexical(node));
                        node=node->next;
                    }
                    return p.yield();
                }
            }
            
            pattern *xgen:: compile_lexical(const xnode *node)
            {
                
                assert(node);
                switch( mmph(node->label) )
                {
                    case 0: assert("RXP"==node->label);
                    {
                        const string meta = node->content();
                        return regexp(meta,& xprs->dict );
                    }

                    case 1: assert("RAW"==node->label);
                    {
                        const  string meta = node->content();
                        return lang::logical::equal(meta);
                    }
                    default:
                        break;
                }
                throw exception("%s: unexpected lexical metadata '%s'", name, node->label.c_str());
            }
            
        }
        
    }
    
}
