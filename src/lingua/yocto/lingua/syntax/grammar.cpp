#include "yocto/lingua/syntax/grammar.hpp"

#include "yocto/lingua/syntax/term.hpp"
#include "yocto/lingua/syntax/joker.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            grammar:: ~grammar() throw()
            {
            }

#define Y_GRAMMAR_CTOR() name(id), rules(), nopt(0), nalt(0), nzom(0), noom(0)

            grammar:: grammar(const string &id) :
            Y_GRAMMAR_CTOR()
            {
            }

            void grammar:: check(const string &label) const
            {
                for(const rule *r = rules.head;r;r=r->next)
                {
                    if(label==r->label)
                    {
                        throw exception("[%s]: multiple rules '%s'", name.c_str(), label.c_str());
                    }
                }
            }

            rule & grammar:: decl_term(const string &label,const uint32_t flags)
            {
                check(label);
                terminal *tt = new terminal(label);
                tt->flags    = flags;
                rules.push_back(tt);
                return *tt;
            }


            rule & grammar:: opt(const rule &r)
            {
                assert(rules.owns(&r));
                const string label = vformat("opt%c%u",rule::internal_char,++nopt);
                optional    *tt    = new optional(label,&r);
                rules.push_back(tt);
                return *tt;
            }

            rule & grammar:: zero_or_more(const rule &r)
            {
                assert(rules.owns(&r));
                const string label = vformat(">=0(%c%u)",rule::internal_char,++nzom);
                at_least    *tt    = new at_least(label,&r,0);
                rules.push_back(tt);
                return *tt;
            }

            rule & grammar:: one_or_more(const rule &r)
            {
                assert(rules.owns(&r));
                const string label = vformat(">=1(%c%u)",rule::internal_char,++noom);
                at_least    *tt    = new at_least(label,&r,1);
                rules.push_back(tt);
                return *tt;
            }

            aggregate & grammar:: agg(const string &id, const uint32_t flags)
            {
                check(id);
                aggregate *tt = new aggregate(id);
                rules.push_back(tt);
                tt->flags     = flags;
                return *tt;
            }

            aggregate & grammar:: agg(const char   *id,const uint32_t flags)
            {
                const string ID(id);
                return agg(ID,flags);
            }


            alternate & grammar:: alt()
            {
                const string label = vformat("alt%c%u",rule::internal_char,++nalt);
                alternate   *tt    = new alternate(label);
                rules.push_back(tt);
                return *tt;
            }

            void grammar:: top_level(rule &r) throw()
            {
                assert(rules.owns(&r));
                rules.move_to_front(&r);
            }

            const rule * grammar:: top_level() const throw()
            {
                return rules.head;
            }

            rule & grammar:: append(rule *r)
            {
                assert(r);
                try
                {
                    check(r->label);
                    rules.push_back(r);
                    return *r;
                }
                catch(...)
                {
                    delete r;
                    throw;
                }
            }
        }
    }
}

#include "yocto/ptr/auto.hpp"
namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            xnode *grammar::run(lexer &lxr, source &src) const
            {
                xnode *tree = 0;
                if(rules.size<=0) throw exception("[%s]: no top-level rule!", name.c_str());
                const rule &top = *(rules.head);

                //______________________________________________________________
                //
                // try to admit top-level rule
                //______________________________________________________________
                if(!top.admit(tree,lxr,src))
                {
                    assert(0==tree);
                    //__________________________________________________________
                    //
                    // an error happened
                    //__________________________________________________________
                    throw exception("%d:[%s] syntax error",lxr.line,name.c_str());
                }

                if(!tree)
                {
                    throw exception("%d:[%s] is invalid: no AST!",lxr.line,name.c_str());
                }

                //______________________________________________________________
                //
                // is there and error
                //______________________________________________________________
                auto_ptr<xnode> guard(tree);
                const lexeme   *lx = lxr.last(src);
                if(lx)
                {
                    const string content = lx->to_string();
                    throw exception("%d:[%s] syntax error after %s='%s'", lx->line, name.c_str(), lx->label.c_str(), content.c_str());
                }

                guard.forget();
                return xnode::ast(tree);
            }

        }
    }
}


#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            namespace
            {
                typedef map<string,int> visited_db;

            }

            void   grammar:: check_consistency() const
            {
                if(rules.size<=0)
                {
                    throw exception("[%s]: no rules !", name.c_str());

                }
                visited_db visited(rules.size, as_capacity);
                for(const rule *r = rules.head; r; r=r->next)
                {
                    if(!visited.insert(r->label,0))
                    {
                        throw exception("[%s].check(UNEXCPECTED MULTIPLE '%s')", name.c_str(), r->label.c_str());
                    }
                }

                check_rule(rules.head,&visited,name.c_str());
                for(visited_db::iterator i=visited.begin();i!=visited.end();++i)
                {
                    const string &label = i->key;
                    const int     count = *i;
                    //std::cerr << "#'" << label << "'=" << count << std::endl;
                    if(count<=0) throw exception("[%s]: standalone rule '%s'", name.c_str(), label.c_str());
                }
            }


            static inline
            int & __count_of(const string &label, visited_db &visited, const char *id)
            {
                int *pCount = visited.search(label);
                if(!pCount)
                {
                    throw exception("[%s] UNEXPECTED UNREGISTERED rule '%s'", id, label.c_str());
                }
                return *pCount;
            }


            void grammar:: check_rule(const rule *r, void *args, const char *id)
            {
                assert(r!=NULL);
                visited_db &visited = *(visited_db *)args;
                assert(r->self);
                switch(r->uuid)
                {
                    case terminal::UUID: __count_of(r->label,visited,id)++; break;

                    case optional::UUID: {
                        optional *self = static_cast<optional *>(r->self);
                        if( __count_of(self->label,visited,id)++ <= 0 )
                        {
                            check_rule(self->jk,args,id);
                        }
                    } break;

                    case at_least::UUID: {
                        at_least *self = static_cast<at_least *>(r->self);
                        if( __count_of(self->label,visited,id)++ <= 0 )
                        {
                            check_rule(self->jk,args,id);
                        }
                    } break;

                    case alternate::UUID: {
                        alternate *self = static_cast<alternate *>(r->self);
                        if(self->members.size<=0)
                        {
                            throw exception("[%s]: EMPTY ALTERNATE '%s'", id, r->label.c_str());
                        }

                        if( __count_of(self->label,visited,id)++ <= 0 )
                        {
                            for(rule::meta_node *m=self->members.head;m;m=m->next)
                            {
                                check_rule(m->addr,args,id);
                            }
                        }
                    } break;

                    case aggregate::UUID: {
                        aggregate *self = static_cast<aggregate *>(r->self);
                        if(self->members.size<=0)
                        {
                            throw exception("[%s]: EMPTY AGGREGATE '%s'", id, r->label.c_str());
                        }
                        if( __count_of(self->label,visited,id)++ <= 0 )
                        {
                            for(rule::meta_node *m=self->members.head;m;m=m->next)
                            {
                                check_rule(m->addr,args,id);
                            }
                        }
                    } break;

                    default:
                        throw exception("grammar::check(UNKNOWN UUID)");
                }

            }


            void   grammar:: no_single(const string &aggID)
            {
                for(rule *r = rules.head;r;r=r->next)
                {
                    if(aggID == r->label)
                    {
                        if(r->uuid != aggregate::UUID )
                        {
                            throw exception("%s.no_single('%s' is not an aggregate)", name.c_str(), aggID.c_str());
                        }
                        r->flags = property::noSingle;
                        return;
                    }
                }
                throw exception("%s.no_single('%s' not found)",name.c_str(),aggID.c_str());
            }


        }

    }

}


#include "yocto/ios/ocstream.hpp"
#include <iostream>
#include "yocto/sort/quick.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            void grammar:: graphviz(const string &filename) const
            {
                ios::wcstream fp(filename);
                fp << "digraph G {\n";
                for(const rule *r=rules.head;r;r=r->next)
                {
                    r->viz(fp);
                }
                fp << "}\n";
            }

            void grammar:: graphviz(const char *filename) const
            {
                const string F(filename);
                graphviz(F);
            }

            size_t grammar:: num_rules() const throw()
            {
                return rules.size;
            }

            size_t grammar::collect(vector<string> &Terms,
                                  vector<string> &Rules) const
            {
                Terms.free();
                Rules.free();
                Terms.ensure(rules.size);
                Rules.ensure(rules.size);
                size_t ans = 0;
                for(const rule *r = rules.head;r;r=r->next)
                {
                    const uint32_t flags = r->flags;
                    switch(flags)
                    {
                        case property::standard:
                        case property::univocal:
                        case property::noSingle:
                            switch(r->uuid)
                        {
                            case terminal::UUID:
                                Terms.push_back(r->label);
                                ans = max_of(ans,r->label.size());
                                break;

                            case aggregate::UUID:
                                Rules.push_back(r->label);
                                ans = max_of(ans,r->label.size());
                                break;

                            default:
                                break;
                        }

                        default:
                            break;
                    }
                }
                quicksort(Terms);
                quicksort(Rules);
                return ans;
            }


            void   grammar:: display() const
            {
                std::cerr << "|- Grammar '" << name << "' content:" << std::endl;
                vector<string> Terms;
                vector<string> Rules;
                collect(Terms,Rules);


                std::cerr << "|_Terminals:" << std::endl;
                for(size_t i=1;i<=Terms.size();++i)
                {
                    std::cerr << "  |_" << Terms[i] << std::endl;
                }
                std::cerr << "|_Internals:" << std::endl;
                for(size_t i=1;i<=Rules.size();++i)
                {
                    std::cerr << "  |_" << Rules[i] << std::endl;
                }
                std::cerr << "|- --------" << std::endl;
                std::cerr << std::endl;
                
            }
            
            
            
        }
    }
}



