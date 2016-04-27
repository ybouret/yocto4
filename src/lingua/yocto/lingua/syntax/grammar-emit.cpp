#include "yocto/lingua/syntax/walker.hpp"
#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/lingua/syntax/joker.hpp"

#include "yocto/lingua/syntax/term.hpp"
#include "yocto/ios/net-string.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/hashing/mph.hpp"

#include <iostream>
#include "yocto/ios/graphviz.hpp"
#include "yocto/associative/map.hpp"
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
                    map<string,string> methods;

                    inline Emitter(const r_list &grammar_rules, ios::ostream &output) :
                    rules(grammar_rules),
                    H(),
                    fp(output)
                    {
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
                            const string method = "on_" + vfs::class_name_for(r->label);
                            std::cerr << "label='" << r->label << "' => method='" << method << "'" << std::endl;
                            if(!methods.insert(r->label,method))
                            {
                                throw exception("unexpected multiple rule label '%s", r->label.c_str());
                            }
                        }
                        H.optimize();
                        //H.graphviz("emit_h.dot");
                        //ios::graphviz_render("emit_h.dot");
                    }

                    inline ~Emitter() throw()
                    {
                    }

                    inline const string & _method(const string &label) const throw()
                    {
                        const string *pM = methods.search(label);
                        assert(pM);
                        return *pM;
                    }

                    inline void run()
                    {
                        const rule *top = rules.head;
                        if(!top)
                        {
                            throw exception("unexpected empty grammar");
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


