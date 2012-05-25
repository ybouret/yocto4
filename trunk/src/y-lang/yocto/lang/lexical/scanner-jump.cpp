#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto 
{
	namespace lang
	{
        
		namespace lexical
		{

            class jump_context 
            {
            public:
                inline jump_context( lexer &target, const string &sub, const callback &on_jump ) :
                lx(target),
                id(sub),
                cb(on_jump)
                {
                }
                
                inline  jump_context( const jump_context &j ) : lx( j.lx ), id( j.id ), cb( j.cb ) {}
                inline ~jump_context() throw() {}
                
                lexer        &lx; //!< reference to the parent lexer
                const string  id; //!< sub scanner to jump to...
                callback      cb; //!< what to do after jump
                
                bool operator()( const regex::token &trigger )
                {
                    lx.jump(id);
                    cb(trigger);
                    return false; //! no more syntactic meaning
                }
                
            private:
                YOCTO_DISABLE_ASSIGN(jump_context);
            };
            
            void scanner:: jump( const string &id, regex::pattern *motif, const callback &cb )
            {
                //-- protect the motif
                auto_ptr<regex::pattern> p(motif);
                
                //-- create the label
                const string label = vformat("jump%u->%s", ++opid, id.c_str());
                
                //-- create the compound action
                assert(motif   != NULL);
                assert(parent_ != NULL);
                lexer &lx = *parent_;
                jump_context ctx( lx, id, cb );
                const action fcn( ctx );
                
                //-- make the corresponding rule
                make(label, p.yield(), &fcn);
                
            }

            
            void scanner:: jump( const string &id, const string &expr, const callback &cb )
            {
                jump( id, regex::compile(expr,dict_), cb );
            }
            
            void scanner:: jump( const char *id, const char *expr, const callback &cb )
            {
                const string ID(id);
                jump( ID, regex::compile(expr,dict_), cb );
            }

        }

    }

}
