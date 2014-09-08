#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            
            void scanner:: append_context_to( string &s ) const
            {
                if(mylex&&mylex->last_label.size()>0)
                {
                    const string ctx = vformat(" after %s '%s' ", mylex->last_label.c_str(), mylex->last_token.c_str() );
                    s.append(ctx);
                }
            }
            
            void scanner:: link_to( lexer &lex )
            {
                if(mylex) throw exception("scanner <%s> is already linked to {%s}", name.c_str(), mylex->name.c_str() );
                if( &line != &(lex.line) )
                    throw exception("scanner <%s>: line doesn't match {%s} line!",name.c_str(), lex.name.c_str());
                mylex = &lex;
            }
            
            bool scanner:: linked_to( const lexer &lx ) const throw()
            {
                return &lx == mylex;
            }

            
            namespace
            {
                enum  MakeType
                {
                    DoJump,
                    DoCall
                };
                
                template <MakeType Kind>
                class LexerMake
                {
                public:
                    lexer         &parent;
                    const string   target;
                    callback       code;
                    
                    LexerMake( const LexerMake &other ) :
                    parent( other.parent ),
                    target( other.target ),
                    code(   other.code   )
                    {
                    }
                    
                    inline ~LexerMake() throw() {}
                    
                    inline LexerMake(lexer          &the_parent,
                                     const string   &the_target,
                                     const callback &the_code) :
                    parent(the_parent),
                    target(the_target),
                    code(  the_code  )
                    {
                    }
                    
                    bool operator()( const token &tkn )
                    {
                        run();
                        code(tkn);
                        return false; //! this is a control rule
                    }
                    
                    
                private:
                    void run();
                    YOCTO_DISABLE_ASSIGN(LexerMake);
                    
                };
                
                template <>
                void LexerMake<DoJump>:: run()
                {
                    parent.jump(target);
                }
                
                template <>
                void LexerMake<DoCall>:: run()
                {
                    parent.call(target);
                }
                
            }
            
            
            void scanner:: jump(const string   &target,
                                pattern        *motif,
                                const callback &onJump)
            {
                auto_ptr<pattern> q(motif);
                if(!mylex) throw exception("<%s> has no linked lexer",name.c_str());
                LexerMake<DoJump> host(*mylex,target,onJump);
                const action which(host);
                const string label = "jump@" + target;
                append(label, q.yield(), which, rule::is_control);
            }
            
            void scanner:: call(const string   &target,
                                pattern        *motif,
                                const callback &onCall)
            {
                auto_ptr<pattern> q(motif);
                if(!mylex) throw exception("<%s> has no linked lexer",name.c_str());
                const LexerMake<DoCall> host(*mylex,target,onCall);
                const action which(host);
                const string label = "call@" + target;
                append(label, q.yield(), which, rule::is_control);
            }
            
            namespace
            {
                class LexerBack
                {
                public:
                    lexer    &parent;
                    callback  code;
                    
                    inline LexerBack(lexer          &the_parent,
                                     const callback &the_code) :
                    parent(the_parent),
                    code(  the_code  )
                    {
                    }
                    
                    inline LexerBack( const LexerBack &other ) :
                    parent(other.parent),
                    code(  other.code  )
                    {
                    }
                    
                    
                    inline ~LexerBack() throw()
                    {
                        
                    }
                    
                    inline bool operator()(const token &tkn)
                    {
                        parent.back();
                        code(tkn);
                        return false;
                    }
                    
                private:
                    YOCTO_DISABLE_ASSIGN(LexerBack);
                };
            }
            
            void scanner:: back(pattern        *motif,
                                const callback &onBack)
            {
                auto_ptr<pattern> q(motif);
                if(!mylex) throw exception("<%s> has no linked lexer",name.c_str());
                const LexerBack host(*mylex,onBack);
                const action    which(host);
                const string    label = vformat("back#%d",++ibck);
                append(label, q.yield(), which, rule::is_control);
            }
            
        }
        
    }
    
}
