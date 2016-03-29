#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"
#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/alternate.hpp"
#include "yocto/lang/syntax/aggregate.hpp"
#include "yocto/lang/syntax/grammar.hpp"
#include "yocto/lang/syntax/walker.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/osstream.hpp"
#include "yocto/ios/net-string.hpp"

#include <iostream>

#include <cstring>

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {

            static const char xnode_name  [] = "XNODE";
            static const char leaves_name [] = "LEAVES";
            static const char walker_args [] = "const XNODE *node";



            namespace
            {
                static inline string label2method(const string &label)
                {
                    string       ans = "on_";
                    const size_t n   = label.size();
                    for(size_t i=0;i<n;++i)
                    {
                        const char C = label[i];
                        if( isalnum(C) || '_' == C)
                        {
                            ans += C;
                        }
                        else
                        {
                            const uint8_t B(C);
                            ans += '_';
                            ans += hexa_text[B];
                        }
                    }
                    return ans;
                }

                static inline bool __is_internal(const string &label) throw()
                {
                    return (0 != strchr( label.c_str(), grammar::internal_char ) );
                }

                class walker_rule
                {
                public:
                    const string   label;
                    const string   method;

                    inline walker_rule(const string &l) :
                    label(l),
                    method(label2method(label))                    {
                    }

                    inline  ~walker_rule() throw()
                    {}


                    const string &key() const throw() { return label; }

                    inline walker_rule(const walker_rule &other) :
                    label( other.label ),
                    method(other.method)
                    {
                    }


                private:
                    YOCTO_DISABLE_ASSIGN(walker_rule);
                };

                typedef set<string,walker_rule> _walker_db;

                class walker_db : public _walker_db
                {
                public:
                    hashing::mperf mph;

                    explicit walker_db(const size_t n) : _walker_db(n,as_capacity) {}
                    virtual ~walker_db() throw() {}

                    inline void add( const rule *r, ios::ostream &nsfp)
                    {
                        assert(r);
                        //-- make local perfect hash
                        mph.insert(r->label,size());


                        //-- local database of method
                        const walker_rule wr(r->label);
                        if( !insert(wr) )
                        {
                            throw exception("unexpected walker failure for '%s'", r->label.c_str());
                        }

                        //-- store info for binary code
                        ios::net_string::format(wr.label,nsfp);

                    }

                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(walker_db);
                };


                static inline void __assert_terminal_is(const bool flag, ios::ostream &fp)
                {
                    fp << "\t\tassert(" << (flag?"true":"false") << "==node->terminal);\n";
                }
            }



            void optional:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                assert(__is_internal(label));
                fp << "\t//! optional  : '" << label << "' \n";

                const walker_db &wdb = *static_cast<walker_db*>(handle); assert(wdb.search(label));
                fp << "\tinline void " << wdb.search(label)->method << "(" << walker_args << ") {\n";

                __assert_terminal_is(false,fp);
                fp << "\t\tconst LEAVES &ch = node->children();\n";
                fp << "\t\tassert(0==ch.size||1==ch.size);\n";

                fp << "\t\tif(ch.size){\n"; assert(wdb.search(jk->label));
                fp << "\t\t\t" << wdb.search(jk->label)->method << "(ch.head);\n";
                fp << "\t\t}\n";

                fp << "\t}\n";
            }


            void terminal:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t//! terminal  : '" << label << "' (" << xnode::get_property_text(modifier) << ")\n";
                const walker_db &wdb = *static_cast<walker_db*>(handle);
                switch(modifier)
                {
                    case standard:
                    case univocal:
                        assert(NULL!=wdb.search(label));
                        fp << "\t/** to be coded by user */\n";
                        fp << "\tvoid " << wdb.search(label)->method << "(" << walker_args << ");\n";
                        break;

                    case jettison:
                        assert(NULL==wdb.search(label));
                        fp << "\t//...\n";
                        break;

                    default:
                        throw exception("terminal::cpp(unexpected '%s')",xnode::get_property_text(modifier));
                }
            }

            void alternate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                assert(__is_internal(label));
                fp << "\t//! alternate : '" << label << "'\n";
                const walker_db &wdb = *static_cast<walker_db*>(handle);
                fp << "\tinline void " << wdb.search(label)->method << "(" << walker_args << ") {\n";

                __assert_terminal_is(false,fp);
                fp << "\t\tconst LEAVES &ch = node->children();\n";
                fp << "\t\tassert(1==ch.size);\n";
                fp << "\t\tconst XNODE *sub = ch.head;\n";

                fp << "\t\tswitch(hash(sub->label)) {\n";
                const operands &ops = *static_cast<const operands *>(content());
                for(const operand *op = ops.head; op; op=op->next)
                {
                    const string &sub = op->addr->label;
                    const int     h   = wdb.mph(sub); assert(h>=0);
                    fp("\t\t\tcase %3d:", h); fp << "// '" << op->addr->label << "'\n";
                    fp << "\t\t\t\t" << wdb.search(sub)->method << "(sub);\n";
                    fp("\t\t\t\tbreak;\n");
                }
                fp << "\t\t\tdefault: break;\n";
                fp << "\t\t}\n";


                fp << "}\n";
            }

            void aggregate:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t//! aggregate : '" << label << "' \n";
                const walker_db &wdb = *static_cast<walker_db*>(handle); assert(wdb.search(label));

            }


            void at_least:: cpp(Y_LANG_SYNTAX_RULE_CPPCODE_ARGS) const
            {
                fp << "\t//! at_least  : '" << label << "' \n";
            }





            void grammar:: walker_prolog(ios::ostream &fp,
                                         const string &class_name) const
            {
                walker_db       wdb(rules.size);

                //______________________________________________________________
                //
                // collect all seen rules
                //______________________________________________________________
                string        nsdb;       // net string databas
                ios::osstream nsfp(nsdb); // to write

                for(const rule *r = rules.head;r;r=r->next)
                {
                    bool do_register = true;
                    switch(r->uuid)
                    {
                        case terminal::UUID:
                        {
                            assert(r->content()!=NULL);
                            const property modifier = *static_cast<property *>(r->content());
                            if(jettison==modifier)
                            {
                                do_register = false;
                                std::cerr << "drop " << r->label << std::endl;
                            }
                        } break;

                        default:
                            break;
                    }
                    if(do_register)
                    {
                        wdb.add(r,nsfp);
                    }
                }
                wdb.mph.optimize();
                for(const rule *r = rules.head;r;r=r->next)
                {
                    const int h = wdb.mph(r->label);
                    if(h>=0)
                    {
                        std::cerr << r->label << " => " <<  h << std::endl;
                    }
                }

                //______________________________________________________________
                //
                // dedicated walker
                //______________________________________________________________
                fp << "class " << class_name << "_base : public yocto::lang::syntax::walker {\n";
                fp << "public:\n";
                fp << "\tinline virtual ~" << class_name << "_base() throw() {}\n";
                fp << "\tinline explicit " << class_name << "_base() : yocto::lang::syntax::walker(\"" << nsdb << "\") {}\n";
                fp << "private:\n";
                fp << "\t" << class_name << "_base(const " << class_name << "_base &);\n";
                fp << "\t" << class_name << "_base &operator=(const " << class_name << "_base &);\n";
                fp << "};\n\n";

                //______________________________________________________________
                //
                // prolog
                //______________________________________________________________
                fp << "class " << class_name << " : public " << class_name << "_base {\n";
                fp << "public:\n";
                fp << "\ttypedef yocto::lang::syntax::xnode  " << xnode_name  << ";\n";
                fp << "\ttypedef " << xnode_name  <<"::leaves " << leaves_name  << ";\n";

                fp << "\tvirtual ~" << class_name << "() throw();\n";
                fp << "\texplicit " << class_name << "() throw();\n";

                fp << "private:\n";
                //______________________________________________________________
                //
                // Emit know code and prototypes
                //______________________________________________________________
                for( const rule *r = rules.head;r;r=r->next)
                {
                    static const size_t ns = 64;
                    fp << "\t//";
                    for(size_t i=0;i<ns;++i) fp << "_";
                    fp << "\n";
                    fp << "\t//\n";

                    r->cpp(fp,class_name,&wdb);
                    fp << "\t//";
                    for(size_t i=0;i<ns;++i) fp << "_";
                    fp << "\n";
                    fp << "\n";
                }
                
                fp << "public:\n";
                
            }
            
            
            void grammar:: walker_epilog(ios::ostream &fp, const string &class_name) const
            {
                //______________________________________________________________
                //
                // epilog
                //______________________________________________________________
                fp << "private:\n";
                fp << "\tYOCTO_DISABLE_COPY_AND_ASSIGN(" << class_name << ");\n";
                fp << "};\n";
            }
            
        }
    }
}
