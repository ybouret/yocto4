#include "yocto/lingua/compiler.hpp"

#include "yocto/counted.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/associative/set.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        
        namespace
        {
            
            class GTerm : public object, public counted
            {
            public:
                const string expr;
                string       attr;
                string       code;
                
                explicit GTerm(const string &_expr
                               ) :
                expr(_expr),
                attr(),
                code()
                {
                    std::cerr << "+GTerm <" << expr << ">" << std::endl;
                }
                
                virtual ~GTerm() throw()
                {
                }
                
                
                static GTerm * CreateFromAtom( const syntax::xnode *node)
                {
                    assert(node);
                    assert("ATOM" == node->label);
                    const syntax::xnode::child_list &ch = node->children();
                    assert(ch.size>0);
                    
                    const syntax::xnode *sub = ch.head;
                    assert(sub);
                    
                    GTerm *p = CreateFromCore(sub);
                    sub = sub->next;
                    if(sub)
                    {
                        assert( "ATTR" == sub->label);
                        p->attr = sub->lex()->to_string();
                    }
                    
                    sub = sub->next;
                    if(sub)
                    {
                        assert( "CODE" == sub->label );
                        p->code = sub->lex()->to_string();
                    }
                    return p;
                }
                
                static GTerm * CreateFromExpr(const syntax::xnode *node)
                {
                    assert(node);
                    assert( "EXPR" == node->label);
                    
                    const string expr = node->lex()->to_string();
                    return new GTerm(expr);
                }
                
                static GTerm *CreateFromChar(const syntax::xnode *node)
                {
                    assert(node);
                    assert( "CHAR" == node->label);
                    assert(node->lex()->size == 1);
                    const uint8_t b( node->lex()->head->data );
                    const string expr = vformat("\\x%02x", b);
                    return new GTerm(expr);
                    
                }
                
                static GTerm *CreateFromCore(const syntax::xnode *node)
                {
                    if( node->label == "EXPR" )
                        return CreateFromExpr(node);
                    
                    if( node->label == "CHAR")
                        return CreateFromChar(node);
                    
                    throw exception("Invalid Terminal Core '%s'", node->label.c_str());
                }
                
                
                
                typedef intrusive_ptr<string,GTerm> Ptr;
                typedef set<string,Ptr>             DB;
                
                const string & key() const throw() { return expr; }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(GTerm);
            };
            
            
            static inline
            void __insert_term( GTerm::DB &db, GTerm *t )
            {
                const GTerm::Ptr p(t);
                if( ! db.insert(p) )
                {
                    //! multiple expression: is it the same ?
                }
                
            }
            //! In-Order terminal collection
            static inline
            void __collect_term( GTerm::DB &db, const syntax::xnode *node )
            {
                assert(node!=0);
                if( "EXPR" == node->label )
                {
                    __insert_term(db, GTerm::CreateFromExpr(node));
                    return;
                }
                
                if( "CHAR" == node->label )
                {
                    __insert_term(db, GTerm::CreateFromChar(node));
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
