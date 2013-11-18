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

            
            namespace 
            {
            
                class back_context
                {
                public:
                    inline back_context( lexer &target, const callback &on_leave ) :
                    lx( target ),
                    cb( on_leave )
                    {
                    }
                    
                    inline  back_context( const back_context &b ) : lx( b.lx ), cb( b.cb ) {}
                    inline ~back_context() throw() {}
                    
                    lexer    &lx;
                    callback  cb;
                    
                    bool operator()( const regex::token &tkn )
                    {
                        lx.back();
                        cb(tkn);
                        return false; // no syntax meaning
                    }
                    
                private:
                    YOCTO_DISABLE_ASSIGN(back_context);
                };
                
                
            }
            
            
            void scanner:: back(regex::pattern *motif, const callback &cb)
            {
                //-- protect motif
                auto_ptr<regex::pattern> p( motif );
                
                //-- make the label
                const string label = vformat("back%u", ++opid_);
                
                //-- prepare the compound action
                assert(motif);
                assert(lexer_);
                lexer &lx = *lexer_;
                const back_context ctx( lx, cb );
                const action       fcn( ctx );
                
                //-- make the rule, marked as control
                //std::cerr << "'" << name << "' => [" << label << "]" << std::endl;
                rules_.push_back( rule::create(label, p.yield(), fcn, true) );
            }
            
            void scanner:: back( const string &expr, const callback &cb)
            {
                back( regex::compile(expr,dict_), cb );
            }
            
            void scanner:: back( const char *expr, const callback &cb)
            {
                back( regex::compile(expr,dict_), cb );
            }


        }

    }

}

