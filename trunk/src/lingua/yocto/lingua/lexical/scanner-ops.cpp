#include "yocto/lingua/lexer.hpp"

namespace yocto
{
    namespace lingua
    {
        
        namespace lexical
        {
            
            
            void scanner:: link_to( lexer &L ) throw()
            {
                assert(0==mylex);
                mylex = &L;
            }
            
            
            namespace
            {
                enum OpsType
                {
                    OpsJump,
                    OpsCall
                };
                
                template <OpsType Kind>
                class Change
                {
                public:
                    lexer       &parent;
                    const string scanID;
                    callback     onJump;
                    
                    inline Change(lexer          &lr,
                                  const string   &id,
                                  const callback &cb
                                  ) :
                    parent(lr),
                    scanID(id),
                    onJump(cb)
                    {
                    }
                    
                    inline bool operator()( const token &trigger )
                    {
                        handle();
                        onJump(trigger);
                        return false; // no lexeme production
                    }
                    
                    inline ~Change() throw()
                    {
                    }
                    
                    inline Change( const Change &other ) :
                    parent(other.parent),
                    scanID(other.scanID),
                    onJump(other.onJump)
                    {
                        
                    }
                    
                    
                private:
                    void handle();
                    YOCTO_DISABLE_ASSIGN(Change);
                };
                
                template <>
                void Change<OpsJump> :: handle() { parent.jump(scanID); }
                
                template <>
                void Change<OpsCall>:: handle() { parent.call(scanID); }
                
            }
            
            
            void scanner:: jump(const string  &id,
                                pattern        *motif,
                                const callback &todo)
            {
                assert(motif);
                assert(mylex);
                auto_ptr<pattern> p(motif);
                
                //-- create a label
                const string label = vformat("jump#%u@",opsID++) + id;
                
                
                //-- wraps the jump
                const Change<OpsJump> J( *mylex, id, todo);
                
                //-- make it a lexical action
                const action A(J);
                
                //-- make a control rule
                rules.push_back( rule::create( label, p.yield(), A, true) );
            }
            
            void scanner:: call(const string  &id,
                                pattern        *motif,
                                const callback &todo)
            {
                assert(motif);
                assert(mylex);
                auto_ptr<pattern> p(motif);
                
                //-- create a label
                const string label = vformat("call#%u@",opsID++) + id;
                
                
                //-- wraps the call
                const Change<OpsCall> J( *mylex, id, todo);
                
                //-- make it a lexical action
                const action A(J);
                
                //-- make a control rule
                rules.push_back( rule::create( label, p.yield(), A, true) );
            }
            
            namespace
            {
                class Back
                {
                public:
                    lexer     &parent;
                    callback   onBack;
                    
                    inline Back( lexer &lx, const callback &cb ) :
                    parent(lx),
                    onBack(cb)
                    {
                    }
                    
                    inline ~Back() throw() {}
                    
                    Back( const Back &other ) :
                    parent( other.parent ),
                    onBack( other.onBack )
                    {
                    }
                    
                    inline bool operator()( const token &trigger )
                    {
                        parent.back();
                        onBack(trigger);
                        return false; // no lexeme production
                    }
                    
                private:
                    YOCTO_DISABLE_ASSIGN(Back);
                };
                
            }
            
            void scanner:: back(pattern *motif,
                                const callback &onBack )
            {
                assert(motif);
                assert(mylex);
                auto_ptr<pattern> p(motif);
                
                //-- create a label
                const string label = vformat("back#%u@",opsID++);
                
                //-- wraps it in a function
                const Back J( *mylex, onBack );
                
                //-- make it a lexical action
                const action A(J);
                
                //-- create the rule
                rules.push_back( rule::create(label, p.yield(), A, true) );
            }

            
            void scanner:: emit( lexer &L, token &content) const
            {
                lexeme *lx = new lexeme(name,line);
                L.unget(lx);
                lx->swap_with(content);
            }
            
            void scanner:: emit( lexer &L, const string &s ) const
            {
                token content(s);
                emit(L,content);
            }
            
            
        }
        
    }
}

