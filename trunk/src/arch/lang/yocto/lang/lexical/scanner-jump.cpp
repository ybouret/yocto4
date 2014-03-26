#include "yocto/lang/lexer.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto 
{
	namespace lang
	{
        
		namespace lexical
		{
            
            namespace {
                template <bool is_calling>
                class jump_return
                {
                public:
                    inline jump_return(lexer          &target, 
                                       const string   &sub, 
                                       const callback &on_jump) :
                    lx(target),
                    id(sub),
                    cb(on_jump)
                    {
                    }
                    
                    inline   jump_return( const jump_return &j ) : lx( j.lx ), id( j.id ), cb( j.cb ) {}
                    inline  ~jump_return() throw() {}
                    
                    lexer        &lx; //!< reference to the parent lexer
                    const string  id; //!< sub scanner to jump to...
                    callback      cb; //!< what to do after jump
                    
                    bool operator()( const regex::token &trigger )
                    {
                        __perform();
                        cb(trigger);
                        return false;
                    }
                    
                private:
                    void __perform();
                    YOCTO_DISABLE_ASSIGN(jump_return);
                };
                
                template <>
                void jump_return<false>:: __perform() { lx.jump(id); }
                
                template <>
                void jump_return<true>:: __perform() { lx.call(id); }
            }
            
            void scanner:: jump( const string &id, regex::pattern *motif, const callback &cb )
            {
                //-- protect the motif
                auto_ptr<regex::pattern> p(motif);
                
                //-- create the label
                const string label = vformat("jump%u@%s", ++opid_, id.c_str());
                
                //-- create the compound action
                assert(motif   != NULL);
                assert(lexer_  != NULL);
                lexer             &lx = *lexer_;
                jump_return<false> ctx( lx, id, cb);
                const action       fcn( ctx );
                
                //-- make the corresponding rule, control=true
                std::cerr << "'" << name << "' => [" << label << "]" << std::endl;
                rules_.push_back( rule::create(label, p.yield(), fcn, true) );
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
            
            
            void scanner:: call( const string &id, regex::pattern *motif, const callback &cb )
            {
                //-- protect the motif
                auto_ptr<regex::pattern> p(motif);
                
                //-- create the label
                const string label = vformat("call%u@%s", ++opid_, id.c_str());
                
                //-- create the compound action
                assert(motif   != NULL);
                assert(lexer_  != NULL);
                lexer            &lx = *lexer_;
                jump_return<true> ctx( lx, id, cb );
                const action      fcn( ctx );
                
                //-- make the corresponding rule
                //std::cerr << "'" << name << "' => [" << label << "]" << std::endl;
                rules_.push_back( rule::create(label, p.yield(), fcn, true) );
                
            }
            
            
            void scanner:: call( const string &id, const string &expr, const callback &cb )
            {
                call( id, regex::compile(expr,dict_), cb );
            }
            
            void scanner:: call( const char *id, const char *expr, const callback &cb )
            {
                const string ID(id);
                call( ID, regex::compile(expr,dict_), cb );
            }
            
            
        }
        
    }
    
}
