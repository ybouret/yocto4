#include "yocto/lingua/syntax/walker.hpp"
#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/lingua/syntax/joker.hpp"

#include "yocto/lingua/syntax/term.hpp"
#include "yocto/ios/net-string.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/hashing/mph.hpp"

#include <iostream>
#include "yocto/ios/graphviz.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            namespace
            {
                class Emitter
                {
                public:
                    const r_list      &rules;
                    hashing::mperf     H;
                    ios::ostream      &fp;

                    inline Emitter(const r_list &grammar_rules, ios::ostream &output) :
                    rules(grammar_rules),
                    H(),
                    fp(output)
                    {
                        //______________________________________________________
                        //
                        // Building the same minimal perfect hasher
                        //______________________________________________________
                        int count = 0;
                        for(const rule *r = rules.head; r; r=r->next)
                        {
                            switch(r->uuid)
                            {
                                case aggregate::UUID:
                                case terminal ::UUID:
                                    if(r->flags==property::standard)
                                    {
                                        ios::net_string::write(r->label,fp);
                                        H.insert(r->label,count++);
                                    }
                                    break;

                                default:
                                    break;
                            }
                        }
                        H.optimize();
                    }

                    inline ~Emitter() throw()
                    {
                    }

                    inline  string method_for(const string &label) const throw()
                    {
                        return "on_" + vfs::class_name_for(label);
                    }

                    inline void run()
                    {
                        const rule *top = rules.head;
                        if(!top)
                        {
                            throw exception("unexpected empty grammar");
                        }
                        emit_rule(top);
                    }

                    //__________________________________________________________
                    //
                    //! emit any rule
                    //__________________________________________________________
                    inline void emit_rule(const rule *r)
                    {
                        assert(r);
                        const string &label  = r->label;
                        const string  method = method_for(label);
                        fp << "\tvoid " << method << "(const XNODE *node) {\n";
                        fp << "\t\tassert(node);\n";
                        switch(r->uuid)
                        {
                            case aggregate::UUID: code_aggregate(r); break;
                            case alternate::UUID: break;
                            case at_least::UUID:  break;
                            case optional::UUID:  break;
                            case terminal::UUID:  break;
                            default: throw exception("emit unhandled rule '%s'", label.c_str());
                        }
                        fp << "\t}\n";
                    }

                    inline void code_aggregate(const rule *r)
                    {
                        const aggregate       &a = *static_cast<const aggregate *>(r->self);
                        const rule::meta_list &m = a.members;
                        if(property::standard==r->flags)
                        {
                            fp << "\t\t// should call '" << r->label << "'\n";
                        }

                        for(const rule::meta_node *node = m.head;node;node=node->next)
                        {
                            const rule *s = node->addr;
                            switch(s->uuid)
                            {
                                case aggregate::UUID:

                                    break;

                                default:
                                    break;
                            }
                        }
                    }




                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(Emitter);
                };
            }

            void  grammar:: emit(ios::ostream &fp) const
            {
                const string  baseName   = vfs::base_name_from(name) + "_walker";
                const string  className  = vfs::class_name_for(baseName);

                fp << "#include \"yocto/hashing/mph.hpp\"\n";

                fp << "class " << className << "  : public yocto::lingua::syntax::tree_walker {\n";
                fp << "public:\n";
                fp << "\tyocto::hashing::mperf H;\n";

                fp << "\tinline virtual ~" << className << "() throw() {}\n";

                fp << "\tinline explicit " << className << "() :\n";
                fp << "\tH(\"";
                Emitter em(rules,fp);
                fp << "\")\n";
                fp << "\t{}\n";
                
                fp << "private:\n";
                fp << "\tYOCTO_DISABLE_COPY_AND_ASSIGN(" << className << ");\n";

                em.run();

                fp << "};\n\n";

                fp  << "#undef  YOCTO_DEFAULT_WALKER\n";
                fp  << "#define YOCTO_DEFAULT_WALKER " << className << "\n";
                
            }

        }
    }
}


