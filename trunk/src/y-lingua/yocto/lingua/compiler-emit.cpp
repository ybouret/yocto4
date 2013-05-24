#include "yocto/lingua/compiler.hpp"

#include "yocto/counted.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        
        namespace
        {
            
            class GTerm : public object, public counted
            {
            public:
                const string       expr;
                const string       attr;
                
                explicit GTerm(const string &_expr,
                               const string &_attr
                               ) :
                expr(_expr),
                attr(_attr)
                {
                }
                
                virtual ~GTerm() throw()
                {
                }
                
                typedef intrusive_ptr<string,GTerm> Ptr;
                typedef set<string,Ptr>             DB;
                
                const string & key() const throw() { return expr; }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(GTerm);
            };
            
            
            //! In-Order terminal collection
            static inline
            void __collect_term( GTerm::DB &db, const syntax::xnode *node )
            {
                assert(node!=0);
                if( "EXPR" == node->label )
                {
                    const string     attr;
                    const string     expr = node->lex()->to_string();
                    const GTerm::Ptr p( new GTerm(expr,attr) );
                    std::cerr << "New Expression: <" << expr << ">" << std::endl;
                    return;
                }
                
                if( "CHAR" == node->label )
                {
                    
                    return;
                }
                
                if( ! node->terminal )
                {
                    for( const syntax::xnode *sub = node->children().head;sub;sub=sub->next)
                    {
                        __collect_term(db, sub);
                    }
                }
                
            }
        }
        
        
        void compiler::emit( const syntax::xnode *G )
        {
            assert(G != 0);
            GTerm::DB TermDB;
            __collect_term(TermDB, G);
        }
        
    }
    
}
