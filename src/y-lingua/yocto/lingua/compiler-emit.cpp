#include "yocto/lingua/compiler.hpp"

#include "yocto/counted.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/associative/set.hpp"

#include "yocto/exception.hpp"
#include "yocto/lingua/pattern/compiler.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        
        namespace
        {
            
            class GTerm : public object, public counted
            {
            public:
                const string      name;
                auto_ptr<pattern> motif;
                
                explicit GTerm(const string &_name,
                               const string &expr,
                               const p_dict *dict 
                               ) :
                name(_name),
                motif(compile(expr,dict))
                {
                    std::cerr << "+GTerm <" << name << ">" << std::endl;
                }
                
                virtual ~GTerm() throw()
                {
                }
                                
                static GTerm * CreateFromExpr(const syntax::xnode *node, const p_dict *dict)
                {
                    assert(node);
                    assert( "EXPR" == node->label);
                    
                    const string name = node->lex()->to_string();
                    const string expr = name;
                    return new GTerm(name,expr,dict);
                }
                
                static GTerm *CreateFromChar(const syntax::xnode *node,const p_dict *dict)
                {
                    assert(node);
                    assert( "CHAR" == node->label);
                    assert(node->lex()->size == 1);
                    const string name = node->lex()->to_string();
                    const string expr = vformat("\\x%02x", uint8_t(name[0]));
                    return new GTerm(name,expr,dict);
                    
                }
                
                
                
                typedef intrusive_ptr<string,GTerm> Ptr;
                typedef set<string,Ptr>             DB;
                
                const string & key() const throw() { return name; }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(GTerm);
            };
            
            
            static inline
            void __insert_term( GTerm::DB &db, GTerm *t )
            {
                const GTerm::Ptr p(t);
                if( ! db.insert(p) )
                {
                    throw exception("multiple terminal '%s'", p->name.c_str());
                }
                
            }
            
            
            //! In-Order terminal collection
            static inline
            void __collect_term( GTerm::DB &db, const syntax::xnode *node, const p_dict *dict )
            {
                assert(node!=0);
                if( "EXPR" == node->label )
                {
                    __insert_term(db, GTerm::CreateFromExpr(node,dict));
                    return;
                }
                
                if( "CHAR" == node->label )
                {
                    __insert_term(db, GTerm::CreateFromChar(node,dict));
                    return;
                }
                
                if( ! node->terminal )
                {
                    for( const syntax::xnode *sub = node->children().head;sub;sub=sub->next)
                    {
                        __collect_term(db, sub,dict);
                    }
                }
                
            }
        }
        
        
        void compiler::emit( const syntax::xnode *G )
        {
            assert(G != 0);
            p_dict    dict;
            GTerm::DB TermDB;
            __collect_term(TermDB, G, &dict);
        }
        
    }
    
}
