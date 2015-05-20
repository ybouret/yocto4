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
                switch( cmph(code) )
                {

                    case 0: assert("@drop"==code);
                    {
                        const string          l = code + vformat("/%u",++jndx);
                        const lexical::action a(& xprs->scanner, & lexical::scanner::discard);
                        xprs->scanner.make(l, compile_lexical(data), a);
                    } return;

                    case 1: assert("@endl"==code);
                    {
                        const string          l = code + vformat("/%u",++jndx);
                        const lexical::action a(& xprs->scanner, & lexical::scanner::newline);
                        xprs->scanner.make(l, compile_lexical(data), a);
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
                // try a plugin
                //______________________________________________________________
                const string plug_name = data->content();
                std::cerr << "\t\tplugin: " << code << " ==> " << plug_name << std::endl;
                const string rule_name = code;
                ((string&)rule_name).skip(1);
                std::cerr << "\t\tplug into rule '" << rule_name << "'" << std::endl;
                agg_ptr *ppA = agg.search(rule_name);
                if(!ppA)
                {
                    throw exception("%s: rule '%s' must be declared before use of plugin",name,rule_name.c_str());
                }

                const char *plugID = plug_name.c_str();
                if( "cstring" == plug_name )
                {
                    rule &r = xprs->term<lexical::cstring>(plugID);
                    (*ppA)->append(&r);
                }

                throw exception("%s: no plugin '%s'", name, plugID);

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
