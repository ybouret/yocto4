#include "yocto/lingua/syntax/walker.hpp"
#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/ios/net-string.hpp"
#include "yocto/fs/vfs.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            void  grammar:: emit_prolog(ios::ostream &fp) const
            {
                const string  baseName   = vfs::base_name_from(name) + "_walker";
                const string  className  = vfs::class_name_for(baseName);
                const string  coreName   = className + "_base";

                fp << "#include \"yocto/hashing/mph.hpp\"\n";

                fp << "class " << coreName << "  : public yocto::lingua::syntax::walker {\n";
                fp << "public:\n";
                fp << "\tyocto::hashing::mperf H;\n";

                fp << "\tinline virtual ~" << coreName << "() throw() {}\n";

                fp << "\tinline explicit " << coreName << "() :\n";
                fp << "\t\tH(\"";
                for(const rule *r = rules.head; r; r=r->next)
                {
                    switch(r->uuid)
                    {
                        case aggregate::UUID:
                        case terminal ::UUID:
                            if(r->flags==property::standard)
                            {
                                ios::net_string::write(r->label,fp);
                            }
                            break;

                        default:
                            break;
                    }

                }

                fp << "\")\n";
                fp << "{}\n";
                
                fp << "private:\n";
                fp << "\tYOCTO_DISABLE_COPY_AND_ASSIGN(" << coreName << ");\n";
                fp << "};\n\n";

                fp  << "#undef  YOCTO_DEFAULT_WALKER\n";
                fp  << "#define YOCTO_DEFAULT_WALKER " << coreName << "\n";
                fp << "class " << className << " : public " << coreName << "{\n";
                fp << "public:\n";
                
            }

        }
    }
}


#include "yocto/hashing/mph.hpp"

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
                    hashing::mperf H;
                    const r_list  &rules;
                    inline Emitter(const r_list &grammar_rules) :
                    H(),
                    rules(grammar_rules)
                    {
                        //______________________________________________________
                        //
                        // first pass: gather used rules
                        //______________________________________________________
                        int count=0;
                        for(const rule *r = rules.head; r; r=r->next)
                        {
                            switch(r->uuid)
                            {
                                case aggregate::UUID:
                                case terminal ::UUID:
                                    if(r->flags==property::standard)
                                    {
                                        H.insert(r->label, count++);
                                    }
                                    break;

                                default:
                                    break;
                            }
                        }

                        //______________________________________________________
                        //
                        // second pass: gather used rules
                        //______________________________________________________

                    }




                    ~Emitter() throw()
                    {
                    }

                private:

                };
            }

            void grammar:: emit_epilog(ios::ostream &fp) const
            {
                const string  baseName   = vfs::base_name_from(name) + "_walker";
                const string  className  = vfs::class_name_for(baseName);
                fp << "private:\n";
                fp << "\tYOCTO_DISABLE_COPY_AND_ASSIGN(" << className << ");\n";

                Emitter em(rules);


                fp << "};\n";
            }

        }
    }
    
}

