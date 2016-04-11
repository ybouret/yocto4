#include "yocto/lingua/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {

            void scanner:: link_to(lexer &parent) throw()
            {
                assert(0==lex);
                assert(0==dict);
                assert(&line == & (parent.line) );
                lex  = &parent;
                dict = &parent.dict;
            }


            namespace {

                class stop_action
                {
                public:
                    lexer *lxr;
                    inline stop_action(lexer *user_lxr) throw() :
                    lxr(user_lxr)
                    {
                        assert(lxr!=NULL);
                    }

                    inline ~stop_action() throw() {}

                    inline bool operator()(const token &) throw()
                    {
                        lxr->stop();
                        return false;
                    }

                    inline stop_action(const stop_action &other) throw() :
                    lxr(other.lxr)
                    {
                    }


                private:
                    YOCTO_DISABLE_ASSIGN(stop_action);
                };
            }

            void scanner :: stop(const string &expr)
            {
                if(!lex) throw exception("<%s>.stop: is not linked to a lexer!", name.c_str() );
                const stop_action will_stop(lex);
                const string      label = "stop";
                check(label);
                rules.push_back( new rule(label,regexp(expr,dict),will_stop,true) );
            }

        }

    }

}



