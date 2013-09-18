#include "yocto/lingua/compiler.hpp"

#include "yocto/counted.hpp"
#include "yocto/ptr/intr.hpp"
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
                
                
                typedef intr_ptr<string,GTerm> Ptr;
                typedef set<string,Ptr>        DB;
                
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
            
            
            ////////////////////////////////////////////////////////////////////
            //
            // Gammar RuleID database
            //
            ////////////////////////////////////////////////////////////////////
            class GRuleID : public object, public counted
            {
            public:
                const string name;
                
                inline GRuleID( const string &id ) :
                name(id)
                {
                }
                
                virtual ~GRuleID() throw()
                {
                }
                
                inline const string &key() const throw() { return name; }
                
                typedef intr_ptr<string,GRuleID> Ptr;
                typedef set<string,Ptr>          DB;
                
                static inline
                void Collect( DB &db, const syntax::xnode *node )
                {
                    assert(node);
                    if( node->label == "ID")
                    {
                        const string id = node->lex()->to_string();
                        if( ! db.search(id) )
                        {
                            const GRuleID::Ptr p( new GRuleID(id) );
                            if( ! db.insert(p) )
                                throw exception("GRuleID::DB(unexpected failure)");
                            std::cerr << "+GRuleID [" << id << "]" << std::endl;
                        }
                        return;
                    }
                    
                    if( !node->terminal )
                    {
                        for( const syntax::xnode *sub = node->children().head;sub;sub=sub->next)
                        {
                            Collect(db, sub);
                        }
                        
                    }
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(GRuleID);
            };
            
            
            
        }
        
        
        static inline
        const syntax::xnode *__get_rules( const syntax::xnode *node )
        {
            assert(node);
            if(node->terminal)
                return 0;
            else
            {
                if("RULES" == node->label)
                {
                    return node;
                }
                
                for(const syntax::xnode *sub = node->children().head; sub; sub=sub->next )
                {
                    const syntax::xnode *ans = __get_rules(sub);
                    if(ans) return ans;
                }
                return 0;
            }
        }
        
        void compiler::emit( const syntax::xnode *G )
        {
            assert(G != 0);
            //==================================================================
            //
            // dictionary to compile definitions
            //
            //==================================================================
            p_dict    dict;
            
            std::cerr << "======== Gathering Terminals" << std::endl;
            //==================================================================
            //
            // Collect all terminal => pattern
            //
            //==================================================================
            GTerm::DB TermDB;
            GTerm::Collect(TermDB, G, &dict);
            
            //==================================================================
            //
            // Collect all IDs
            //
            //==================================================================
            std::cerr << "======== Gathering Rules ID" << std::endl;
            GRuleID::DB RuleID;
            GRuleID::Collect(RuleID, G);
            
            
            //==================================================================
            //
            // find the RULES
            //
            //==================================================================
            std::cerr << "======== Finding RULES" << std::endl;
            const syntax::xnode *R = __get_rules(G);
            if(!R)
                throw exception("Missing Rules!");
            assert(!R->terminal);
            
            //==================================================================
            //
            // check that every rule has a definition
            //
            //==================================================================
            const syntax::xnode::child_list &Rules = R->children();
            std::cerr << "======== Checking " << Rules.size << " rules" << std::endl;
            for( const syntax::xnode *sub = Rules.head;sub;sub=sub->next)
            {
                assert(!sub->terminal);
                const syntax::xnode::child_list &content = sub->children();
                assert(content.size>0);
                assert("ID" == content.head->label);
                assert(content.head->terminal);
                const string id = content.head->lex()->to_string();
                std::cerr << "New Rule: <" << id << ">" << std::endl;
                if( !RuleID.search(id) )
                    throw exception("Missing Grammar Rule for <%s>", id.c_str());
            }
            
            
            //==================================================================
            //
            // OK, let us start building the grammar
            //
            //==================================================================
            
            
            
        }
        
    }
    
}
