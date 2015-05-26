#include "yocto/lang/syntax/grammar.hpp"
#include "yocto/ptr/alias.hpp"
#include "yocto/exception.hpp"
#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            typedef alias_ptr<string,rule> rule_alias;
            typedef set<string,rule_alias> rule_dbase;
            typedef rule                  *rule_addr;
            
            class grammar:: dbase : public rule_dbase
            {
            public:
                explicit  dbase() throw() : rule_dbase(),addr() {}
                virtual  ~dbase() throw() {}
                
                sorted_vector<rule_addr> addr;
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(dbase);
            };

            void grammar:: check_locked() const
            {
                if(!db) throw exception("[[%s]] is locked!",name.c_str());
            }

            void grammar:: enroll( rule *r )
            {
                assert(r);
                check_locked();
                const rule_alias ra(r);
                if(!db->insert(ra))
                {
                    // multiple label ?
                    throw exception("[[%s]]: unexpected failure to enroll '%s' level-1", name.c_str(),r->label.c_str());
                }
                
                try
                {
                    // multiple address ?
                    if(!db->addr.insert(r))
                    {
                        (void) db->remove(r->label);
                        throw exception("[[%s]]: unexpected failure to enroll '%s' level-2", name.c_str(),r->label.c_str());
                    }
                }
                catch(...)
                {
                    
                    (void) db->remove(r->label);
                    throw;
                }
            }
            
            void grammar:: lock() throw()
            {
                if(db)
                {
                    delete db;
                    db = 0;
                }
            }
            
            grammar:: ~grammar() throw()
            {
                if(db)
                {
                    delete db;
                    db = NULL;
                }
            }
            
            
            grammar:: grammar(const string &id) :
            name(id),
            rules(),
            db( new dbase() )
            {
                
            }
            
            grammar:: grammar(const char  *id) :
            name(id),
            rules(),
            db( new dbase() )
            {
                
            }
            
            void grammar:: top_level( rule &r ) throw()
            {
                assert( rules.owns(&r) );
                rules.move_to_front(&r);
            }
            
            
        }
        
    }
    
}


#include "yocto/lang/syntax/terminal.hpp"
#include "yocto/lang/syntax/optional.hpp"
#include "yocto/lang/syntax/at-least.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            void grammar:: check_label(const string &label) const
            {
                check_locked();
                if(db->search(label))
                {
                    throw exception("[[%s]]: multiple rule '%s'", name.c_str(), label.c_str());
                }
                
            }
            
            rule & grammar:: get_rule(const string &id)
            {
                check_locked();
                rule_alias *ppR = db->search(id);
                if(ppR)
                {
                    return **ppR;
                }
                throw exception("[[%s]]: no rule '%s'", name.c_str(),id.c_str());
            }
            
            rule & grammar:: get_rule(const char *id)
            {
                const string ID(id);
                return get_rule(ID);
            }
            
            
#define ENROLL(RULE_PTR) do {                     \
rules.push_back(RULE_PTR);                        \
try { enroll(RULE_PTR); }                         \
catch(...) { delete rules.pop_back(); throw;   }  \
} while(false)
            
            terminal & grammar:: decl_term( const string &label, const property ppty  )
            {
                check_label(label);
                terminal *r = new terminal(label,ppty);
                ENROLL(r);
                return *r;
            }
            
            
            terminal & grammar:: decl_term( const char *label, const property ppty  )
            {
                const string Label(label);
                return decl_term(Label,ppty);
            }
            
            aggregate & grammar:: agg( const string &label, const property ppty)
            {
                check_label(label);
                aggregate *r = new aggregate(label,ppty);
                ENROLL(r);
                return *r;
            }
            
            aggregate & grammar:: agg( const char *label, const property ppty)
            {
                const string Label(label); return agg(Label,ppty);
            }
            
            
            alternate & grammar:: alt()
            {
                check_locked();
                alternate *r = new alternate();
                ENROLL(r);
                return *r;
            }
            
            rule & grammar:: opt(rule &r)
            {
                if( ! has(&r) )
                    throw exception("rule '%s' doesn't belong to [[%s]]", r.label.c_str(), name.c_str());
                optional *R = new optional(r);
                ENROLL(R);
                return *R;
            }
            
            rule & grammar:: at_least(rule &r, const size_t nmin)
            {
                if( ! has(&r) )
                    throw exception("rule '%s' doesn't belong to [[%s]]", r.label.c_str(), name.c_str());
                rule *R = new syntax::at_least(r,nmin);
                ENROLL(R);
                return *R;
            }
            
            rule & grammar:: zero_or_more(rule &r)
            {
                return at_least(r, 0);
            }
            
            rule & grammar:: one_or_more(rule &r)
            {
                return at_least(r, 1);
            }
            
            
            
        }
        
    }
    
}

#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            void grammar:: append( rule *r )
            {
                assert(r);
                auto_ptr<rule> p(r);
                check_label(r->label);
                p.forget();
                ENROLL(r);
            }
            
            
            xnode *grammar:: accept( lexer &lxr, source &src, ios::istream &fp)
            {
                //______________________________________________________________
                //
                // Let's go, try to accept the top level rules
                //______________________________________________________________
                const char *gname = name.c_str();
                if(rules.size<=0) throw exception("[[%s]]: no top level rule!",gname);
                
                xnode          *tree = 0;
                const rule     &root = *(rules.head);

                if( !root.accept(tree,lxr,src,fp) )
                {
                    assert(NULL==tree);
                    //__________________________________________________________
                    //
                    // not accepted: what happened ?
                    //__________________________________________________________
                    failure(lxr);
                }

                //______________________________________________________________
                //
                // Let's study the result
                //______________________________________________________________
                auto_ptr<xnode> ast(tree);
                const lexeme   *lx = lxr.peek(src, fp);
                if(lx)
                {
                    failure(lxr);
                }
                
                
                //______________________________________________________________
                //
                // simplify the syntax tree
                //______________________________________________________________
                return xnode::AST(ast.yield());
                
            }

            void grammar::failure(lexer &lxr)
            {
                const char     *gname = name.c_str();
                const rule     &root  = *(rules.head);

                switch(lxr.size)
                {
                    case 0:
                        throw exception("[[%s]]: no lexeme, expecting %s",gname,root.label.c_str());

                    case 1:
                    {
                        const lexeme *lx = lxr.head;
                        const string  tx = lx->to_string();
                        throw exception("%d:[[%s]]: unexpected single lexeme %s('%s')", lx->line, gname, lx->label.c_str(), tx.c_str());
                    }

                    default:
                    {
                        assert(lxr.tail);
                        assert(lxr.tail->prev);
                        const lexeme *tail   = lxr.tail;
                        const string  tail_s = tail->to_string();
                        const lexeme *prev   = tail->prev;
                        const string  prev_s = prev->to_string();

                        throw exception("%d:[[%s]]: unexpected %s('%s') after %s('%s')",
                                        tail->line,
                                        gname,
                                        tail->label.c_str(),
                                        tail_s.c_str(),
                                        prev->label.c_str(),
                                        prev_s.c_str());
                    }

                }

            }


            bool grammar:: has(const string &id) const throw()
            {
                check_locked();
                return NULL != db->search(id);
            }
            
            bool grammar:: has(const char *id) const
            {
                const string ID(id);
                return has(ID);
            }
            
            bool  grammar:: has(const rule *r) const throw()
            {
                assert(r);
                check_locked();
                return db->addr.search((rule*)r);
            }

            
            rule &grammar:: top_level()
            {
                if(rules.size<=0)
                {
                    throw exception("[[%s]]: no top level rule", name.c_str());
                }
                return *(rules.head);
            }
            
            size_t grammar:: count() const throw()
            {
                return rules.size;
            }
            
            void grammar:: cleanup()
            {
                check_locked();
                r_list stk;
                while(rules.size>0 )
                {
                    rule *r = rules.pop_front();
                    if(r->uuid==aggregate::UUID)
                    {
                        operands *ops = (operands *)(r->content());
                        if(ops->size<=0)
                        {
                            remove(r->label);
                        }
                        else
                        {
                            stk.push_back(r);
                        }
                    }
                    else
                    {
                        stk.push_back(r);
                    }
                }
                stk.swap_with(rules);
            }
            
        }
    }
}

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            void grammar:: remove( const string &label )
            {
                check_locked();
                rule_alias *ppR = db->search(label);
                if(!ppR)
                {
                    throw exception("[[%s]]: no '%s' to remove", name.c_str(), label.c_str());
                }
                rule *r = rules.unlink(&(**ppR));
                (void) db->addr.remove(r);
                (void) db->remove(label);
                // TODO: check if we can remove it...
                
                delete r;
            }
            
        }
        
    }
    
}



#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            void grammar:: gramviz( const string &filename ) const
            {
                ios::ocstream fp(filename,false);
                fp << "digraph G{\n";
                fp << "\tordering=out;\n";
                
                // prepare all nodes
                for(const rule *r=rules.head;r;r=r->next)
                {
                    r->viz(fp);
                }
                
                // prepare all links
                for(const rule *r=rules.head;r;r=r->next)
                {
                    r->lnk(fp);
                }
                
                
                fp << "}";
            }
            
        }
        
    }
}


