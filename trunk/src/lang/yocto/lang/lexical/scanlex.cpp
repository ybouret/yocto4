#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            
            void scanner:: link_to( lexer &lex )
            {
                if(mylex) throw exception("scanner '%s' is already linked to {%s}", name.c_str(), mylex->name.c_str() );
                if( &line != &(lex.line) )
                    throw exception("scanner '%s': line doesn't match {%s} line!",name.c_str(), lex.name.c_str());
                mylex = &lex;
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
                                const string   &regex,
                                const callback &onJump)
            {
                if(!mylex) throw exception("'%s' has no linked lexer",name.c_str());
                LexerMake<DoJump> host(*mylex,target,onJump);
                const action A(host);
                const string label = "jump@" + target;
                ctrl(label,regex,A);
            }
            
            void scanner:: call(const string   &target,
                                const string   &regex,
                                const callback &onCall)
            {
                if(!mylex) throw exception("'%s' has no linked lexer",name.c_str());
                const LexerMake<DoCall> host(*mylex,target,onCall);
                const action A(host);
                const string label = "call@" + target;
                ctrl(label,regex,A);
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
            
            void scanner:: back(const string   &regex,
                                const callback &onBack)
            {
                if(!mylex) throw exception("'%s' has no linked lexer",name.c_str());
                const LexerBack host(*mylex,onBack);
                const action    A(host);
                const string    label = vformat("back#%d",++iBack);
                ctrl(label,regex,A);
            }
            
        }
        
    }
    
}
