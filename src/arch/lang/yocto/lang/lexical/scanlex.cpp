#include "yocto/lang/lexer.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {


            void scanner:: link_to(lexer &parent)
            {
                if(lex)
                {
                    throw exception("<%s> already linked to {%s}", name.c_str(), lex->name.c_str());
                }

                if( &line != &parent.line)
                {
                    throw exception("<%s>'s line is not {%s}'s line", name.c_str(), parent.name.c_str());
                }

                lex  = &parent;
                dict = &parent.dict;
            }

        }

    }

}


namespace yocto
{
    namespace lang
    {


        namespace lexical
        {

            namespace {

                class JumpAction
                {
                public:
                    const string Scanner;
                    callback     OnEnter;
                    const bool   Returns;
                    lexer       &MyLexer;

                    inline JumpAction(const string   &id,
                                      const callback &cb,
                                      const bool      rt,
                                      lexer          &lx) :
                    Scanner(id),
                    OnEnter(cb),
                    Returns(rt),
                    MyLexer(lx)
                    {
                    }

                    inline ~JumpAction() throw() {}

                    inline JumpAction(const JumpAction &J) :
                    Scanner(J.Scanner),
                    OnEnter(J.OnEnter),
                    Returns(J.Returns),
                    MyLexer(J.MyLexer)
                    {
                    }

                    inline bool operator()( const token &tkn )
                    {
                        if(Returns)
                        {
                            MyLexer.call(Scanner);
                        }
                        else
                        {
                            MyLexer.jump(Scanner);
                        }

                        OnEnter(tkn);
                        return false;
                    }

                private:
                    YOCTO_DISABLE_ASSIGN(JumpAction);
                };

            }


            void scanner:: jump(const string &scanner_id, pattern *p, const callback &on_jump)
            {
                auto_ptr<pattern> q(p);
                const string label = "@jump@" + scanner_id;
                check_label(label);

                if(!lex) throw exception("<%s> '%s': no linked lexer for JUMP@%s", name.c_str(), label.c_str(), scanner_id.c_str());
                const JumpAction J(scanner_id,on_jump,false,*lex);

                const action     a(J);
                rules.push_back( rule::create(label,q.yield(),a, true) );
            }

            void scanner:: jump(const char   *scanner_id, const char *expr)
            {
                jump(scanner_id, expr, this, &scanner::discard_cb);
            }


            void scanner:: call( const string &scanner_id, pattern *p, const callback &on_call)
            {
                auto_ptr<pattern> q(p);
                const string label = "@call@" + scanner_id;
                check_label(label);

                if(!lex) throw exception("<%s> '%s': no linked lexer for CALL@%s", name.c_str(), label.c_str(), scanner_id.c_str());
                const JumpAction J(scanner_id,on_call,true,*lex);

                const action     a(J);
                rules.push_back( rule::create(label,q.yield(),a, true) );
            }

            void scanner:: call(const char   *scanner_id, const char *expr)
            {
                call(scanner_id, expr, this, &scanner::discard_cb);
            }

            namespace  {
                
                class BackAction
                {
                public:
                    callback     OnLeave;
                    lexer       &MyLexer;

                    inline ~BackAction() throw() {}

                    inline BackAction(const callback &on_leave,
                                      lexer          &my_lexer) :
                    OnLeave(on_leave),
                    MyLexer(my_lexer)
                    {
                    }


                    inline BackAction(const BackAction &B) :
                    OnLeave(B.OnLeave),
                    MyLexer(B.MyLexer)
                    {
                    }

                    inline bool operator()(const token &tkn)
                    {
                        MyLexer.back();
                        OnLeave(tkn);
                        return false;
                    }


                private:
                    YOCTO_DISABLE_ASSIGN(BackAction);
                };
            }


            void scanner:: back( pattern *p, const callback &on_back)
            {
                auto_ptr<pattern> q(p);
                const string label = vformat("@back#%u",++bidx);
                check_label(label);
                
                if(!lex) throw exception("<%s> '%s': no linked lexer for BACK", name.c_str(), label.c_str());
                
                const BackAction B(on_back,*lex);
                const action     a(B);
                rules.push_back( rule::create(label,q.yield(),a,true) );
            }
            
            void scanner::back(const char *expr)
            {
                back(expr, this, & scanner::discard_cb);
            }

            void scanner:: back_newline()
            {
                back("[:endl:]", this, & scanner::newline_cb);
            }
            
        }
        
    }
    
}
