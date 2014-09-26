#include "yocto/lang/generator.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/exception.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/sequence/addr-list.hpp"


#include <cstdlib>

namespace yocto
{
    namespace lang
    {
        namespace
        {
            enum vnode_type
            {
                vnode_rule,
                vnode_term,
                vnode_joker
            };
            
            
            
            class vnode;
            
            typedef core::list_of_cpp<vnode> vlist;
            typedef addr_list<vnode>         alist;
            
            class vnode : public object
            {
            public:
                typedef vnode *                  ptr;
                
                const string         name;
                const syntax::xnode *node;
                const vnode_type     type;
                vnode               *next;
                vnode               *prev;
                alist                children;
                
                //! a rule
                explicit vnode(const string &id, const syntax::xnode *nd,  vnode_type nt ) :
                name(id),
                node(nd),
                type(nt),
                next(0),
                prev(0),
                children()
                {
                    
                }
                
                virtual ~vnode() throw() {}
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(vnode);
            };
            
            typedef map<string,vnode::ptr> vmap;
            
            
            class vcontext
            {
            public:
                vlist  vr;   //!< virtual top level rules
                vlist  vl;   //!< local rules
                vmap   vm;   //!< global map
                int    vi;   //!< index for rule naming
                p_dict dict; //!< for storing patterns
                
                inline vcontext() : vr(), vl(), vi(0), dict()
                {
                    
                }
                
                inline ~vcontext() throw()
                {
                    
                }
                
                vnode *find_in( vlist &l, const string &id ) throw()
                {
                    for(vnode *vn = l.head; vn; vn=vn->next)
                    {
                        if(vn->name == id) return vn;
                    }
                    return 0;
                }
                
                vnode *find_rule(const string &id) throw()
                {
                    return find_in(vr,id);
                }
                
                inline void register_rule(const syntax::xnode *xn)
                {
                    assert(xn);
                    assert("ID"==xn->label);
                    const string id = xn->lxm()->to_string();
                    std::cerr <<  "-- new rule " << id << std::endl;
                    if(find_rule(id))
                    {
                        throw exception("%d: multiple rule '%s'", xn->lxm()->line, id.c_str());
                    }
                    vnode *vn = new vnode(id,xn->next,vnode_rule);
                    vr.push_back(vn);
                }
                
                
                inline void collect_rules( const syntax::xnode *rules )
                {
                    assert( !rules->terminal );
                    assert("RULES" == rules->label );
                    for( const syntax::xnode *xn = rules->head(); xn; xn=xn->next)
                    {
                        assert("RULE" == xn->label);
                        register_rule(xn->head());
                        
                    }
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(vcontext);
            };
            
            
        }
        
        void generator:: process( ios::istream &fp )
        {
            //__________________________________________________________________
            //
            // Get user rules
            //__________________________________________________________________
            initialize();
            auto_ptr<XNode> Tree( this->run(fp) );
            assert( Tree.is_valid() );
            
            //__________________________________________________________________
            //
            // rewrite stuff
            //__________________________________________________________________
            XNode *root = Tree.__get();
            rewrite(root);
            root->graphviz("xnode.dot");
            system("dot -Tpng -o xnode.png xnode.dot");
            
            
            //__________________________________________________________________
            //
            // Let's rebuild the tree: gather top level rules
            //__________________________________________________________________
            vcontext ctx;
            ctx.collect_rules(root);
            
            //__________________________________________________________________
            //
            // Then recursively collect rule content
            //__________________________________________________________________
            
            
            
            //__________________________________________________________________
            //
            // generate parser
            //__________________________________________________________________
            //auto_ptr<parser> P( new parser("generated","scanner") );
            
            
            
            
        }
        
    }
    
}
