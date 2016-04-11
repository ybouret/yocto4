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



        }

    }

}


namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
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
                        return false; // control, not producing lexeme
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
                const string      label = "stop";
                check(label);
                const stop_action  will_stop(lex);
                const pattern::ptr motif( regexp(expr,dict) );

                rules.push_back( new rule(label,motif,will_stop,true) );
            }


        }
    }
}


namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            namespace
            {
                class call_action
                {
                public:
                    lexer       *lxr;
                    const string id;
                    callback     cb;

                    inline call_action(lexer          *usr_lxr,
                                       const string   &scanner_id,
                                       const callback &todo ) :
                    lxr(usr_lxr),
                    id(scanner_id),
                    cb(todo)
                    {
                    }

                    inline call_action(const call_action &other) :
                    lxr(other.lxr),
                    id(other.id),
                    cb(other.cb)
                    {
                    }


                    inline ~call_action() throw() {}

                    inline bool operator()(const token &tkn)
                    {
                        lxr->call(id);
                        cb(tkn);
                        return false; // control, not producing lexeme
                    }

                private:
                    YOCTO_DISABLE_ASSIGN(call_action);
                };
            }

            void scanner:: call(const string   &id,
                                const string   &expr,
                                const callback &cb)
            {
                if(!lex) throw exception("<%s>.call: is not linked to a lexer!", name.c_str() );
                const string label = "@call_" + id + "/" + expr;
                check(label);
                const call_action  will_call(lex,id,cb);
                const pattern::ptr motif( regexp(expr,dict) );

                rules.push_back( new rule(label,motif,will_call,true) );

            }

        }
    }
}



namespace yocto
{
    namespace lingua
    {
        namespace lexical
        {
            namespace
            {
                class back_action
                {
                public:
                    lexer       *lxr;
                    callback     cb;

                    inline back_action(lexer          *usr_lxr,
                                       const callback &todo) :
                    lxr(usr_lxr),
                    cb(todo)
                    {
                    }

                    inline ~back_action() throw() {}

                    inline back_action(const back_action &other):
                    lxr(other.lxr),
                    cb(other.cb)
                    {
                    }

                    inline bool operator()(const token &tkn)
                    {
                        lxr->back();
                        cb(tkn);
                        return false;
                    }



                private:
                    YOCTO_DISABLE_ASSIGN(back_action);
                };

            }

            void scanner:: back(const string   &expr,
                                const callback &cb)
            {
                if(!lex) throw exception("<%s>.call: is not linked to a lexer!", name.c_str() );
                const string label = "@back/" + expr;
                check(label);
                const back_action  will_back(lex,cb);
                const pattern::ptr motif( regexp(expr,dict) );

                rules.push_back( new rule(label,motif,will_back,true) );
            }

        }

    }
}


