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
            
            ////////////////////////////////////////////////////////////////////
            //
            //  Grammar Terminal / Database
            //
            ////////////////////////////////////////////////////////////////////
            class GTerm : public object, public counted
            {
            public:
                const string      name;
                auto_ptr<pattern> motif;
                
                //! create from name and expression
                inline GTerm(const string &_name,
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
                
                static inline GTerm * CreateFromExpr(const syntax::xnode *node, const p_dict *dict)
                {
                    assert(node);
                    assert( "EXPR" == node->label);
                    
                    const string name = node->lex()->to_string();
                    const string expr = name;
                    return new GTerm(name,expr,dict);
                }
                
                static inline GTerm *CreateFromChar(const syntax::xnode *node,const p_dict *dict)
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
                
                inline const string & key() const throw() { return name; }
                
                static inline void Insert( DB &db, GTerm *t )
                {
                    const GTerm::Ptr p(t);
                    if( ! db.insert(p) )
                    {
                        throw exception("multiple terminal '%s'", p->name.c_str());
                    }
                    
                }
                
                static inline void Collect( DB &db, const syntax::xnode *node, const p_dict *dict )
                {
                    assert(node!=0);
                    if( "EXPR" == node->label )
                    {
                        Insert(db, GTerm::CreateFromExpr(node,dict));
                        return;
                    }
                    
                    if( "CHAR" == node->label )
                    {
                       Insert(db, GTerm::CreateFromChar(node,dict));
                        return;
                    }
                    
                    if( ! node->terminal )
                    {
                        for( const syntax::xnode *sub = node->children().head;sub;sub=sub->next)
                        {
                            Collect(db, sub,dict);
                        }
                    }

                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(GTerm);
            };
            
            
            class GRule : public object, public counted
            {
            public:
                const string name;
                
                inline GRule( const string &id ) :
                name(id)
                {
                }
                
                virtual ~GRule() throw()
                {
                }
                
                inline const string &key() const throw() { return name; }
                
                typedef intrusive_ptr<string,GRule> Ptr;
                typedef set<string,Ptr>             DB;
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(GRule);
            };
            
            
        }
        
        
        void compiler::emit( const syntax::xnode *G )
        {
            assert(G != 0);
            p_dict    dict;
            GTerm::DB TermDB;
            GTerm::Collect(TermDB, G, &dict);
        }
        
    }
    
}
