#include "yocto/lang/generator.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/addr-list.hpp"
#include "yocto/ios/ocstream.hpp"


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
                vnode_expr,
                vnode_char,
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
                syntax::xnode       *node;
                const vnode_type     type;
                vnode               *next;
                vnode               *prev;
                alist                children;
                
                //! a rule
                explicit vnode(const string &id, syntax::xnode *nd,  vnode_type nt ) :
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
            
            //typedef map<string,vnode::ptr> vmap;
            
            
            class vcontext
            {
            public:
                vlist  vr;   //!< virtual top level rules
                vlist  vs;   //!< virtual string rules
                vlist  vc;   //!< virtual char   rules
                vlist  vj;   //!< virtual joker  rules
                int    vi;   //!< index for rule naming

                inline vcontext() : vr(), vs(), vc(), vj(), vi(0)
                {
                    
                }
                
                inline ~vcontext() throw()
                {
                    
                }
                
                inline vnode *find_in( vlist &l, const string &id ) throw()
                {
                    for(vnode *vn = l.head; vn; vn=vn->next)
                    {
                        if(vn->name == id) return vn;
                    }
                    return 0;
                }
                
                inline vnode *find_rule(const string &id) throw()
                {
                    return find_in(vr,id);
                }
                
                inline vnode *find_expr(const string &expr) throw()
                {
                    return find_in(vs,expr);
                }
                
                inline vnode *find_char(const string &expr) throw()
                {
                    return find_in(vc,expr);
                }
                
                // register a top level rule
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
                
                
                
                // compile rules
                inline void compile_rules( syntax::xnode *rules )
                {
                    //-- first pass: register all the named rules
                    assert( !rules->terminal );
                    assert("RULES" == rules->label );
                    for( syntax::xnode *xn = rules->head(); xn; xn=xn->next)
                    {
                        assert("RULE" == xn->label);
                        register_rule(xn->head());
                    }
                    
                    //-- second pass: build tree
                    for( vnode *parent = vr.head; parent; parent=parent->next )
                    {
                        build(parent,parent->node);
                    }
                }
                
                
                inline void build( vnode *parent, syntax::xnode *xn)
                {
                    std::cerr << "building '" << parent->name << "'" << std::endl;
                    for(;xn;xn=xn->next)
                    {
                        const string &label = xn->label;
                        std::cerr << "\t" << label << std::endl;
                        
                        if( "ID" == label )
                        {
                            //__________________________________________________
                            //
                            // this is a sub rule
                            //__________________________________________________
                            const string child_name = xn->lxm()->to_string();
                            vnode       *child_addr = find_rule(child_name);
                            std::cerr << "\t|_" << child_name << std::endl;
                            if(!child_addr)
                            {
                                throw exception("%d: unknown sub-rule '%s' in '%s'", xn->lxm()->line, child_name.c_str(),parent->name.c_str());
                            }
                            parent->children.append(child_addr);
                            continue;
                        }
                        
                        if( "ATOMS" == label )
                        {
                            //__________________________________________________
                            //
                            // this is a set of rules to append to parent
                            //__________________________________________________
                            build(parent,xn->head());
                            continue;
                        }
                        
                        
                        if( "STRING" == label )
                        {
                            //__________________________________________________
                            //
                            // and expression to compile
                            //__________________________________________________
                            const string expr = xn->lxm()->to_string();
                            std::cerr << "\t|_<" << expr << ">" << std::endl;
                            vnode *vn = find_expr(expr);
                            if(!vn)
                            {
                                vn = new vnode(expr,NULL,vnode_expr);
                                vs.push_back(vn);
                            }
                            parent->children.append(vn);
                            continue;
                        }
                        
                        if( "CHAR" == label )
                        {
                            const string expr = xn->lxm()->to_string();
                            assert(1==expr.size());
                            std::cerr << "\t|_'" << expr << "'" << std::endl;
                            vnode *vn = find_char(expr);
                            if(!vn)
                            {
                                vn = new vnode(expr,NULL,vnode_char);
                                vc.push_back(vn);
                            }
                            parent->children.append(vn);
                            continue;
                            
                        }
                        
                        if( "JOKER" == label )
                        {
                            const string jname = vformat("joker#%d",++vi);
                            vnode *jk = new vnode(jname,xn,vnode_joker);
                            vj.push_back(jk);
                            parent->children.append(jk);
                            build(jk,xn->children().tail);
                            continue;
                        }
                        
                    }
                    
                }
                
                
                void graphviz(const string &fn)
                {
                    ios::ocstream fp(fn,false);
                    
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
            
            
            
            vcontext ctx;
            ctx.compile_rules(root);
            
            
            //__________________________________________________________________
            //
            // generate parser
            //__________________________________________________________________
            //auto_ptr<parser> P( new parser("generated","scanner") );
            
            
            
            
        }
        
    }
    
}
