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
            
            //__________________________________________________________________
            //
            //
            // virtual node type
            //
            //__________________________________________________________________
            enum vnode_type
            {
                vnode_rule,  //!< a rule
                vnode_expr,  //!< an expression
                vnode_char,  //!< a  single char
                vnode_joker, //!< a  joker node
                vnode_or,    //!< an alternation node
                vnode_group  //!< a sub rule
            };
            
            //__________________________________________________________________
            //
            //
            // a virtual node
            //
            //__________________________________________________________________
            class vnode;
            
            typedef core::list_of_cpp<vnode> vlist;
            typedef addr_list<vnode>         alist;
            typedef alist::node_type         anode;
            
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
                unsigned             linked;
                
                explicit vnode(const string &id, syntax::xnode *nd,  vnode_type nt ) :
                name(id),
                node(nd),
                type(nt),
                next(0),
                prev(0),
                children(),
                linked(0)
                {
                    
                }
                
                virtual ~vnode() throw() {}
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(vnode);
            };
            
            
            //__________________________________________________________________
            //
            //
            // a context to build the generator tree
            //
            //__________________________________________________________________
            class vcontext
            {
            public:
                vlist  vr;   //!< virtual top level rules
                vlist  vs;   //!< virtual string rules
                vlist  vc;   //!< virtual char   rules
                vlist  vj;   //!< virtual joker  rules
                vlist  vo;   //!< virtual or     rules
                
                inline vcontext() : vr(), vs(), vc(), vj(), vo()
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
                
                //______________________________________________________________
                //
                //
                // register a top level rule
                //
                //______________________________________________________________
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
                
                
                //______________________________________________________________
                //
                //
                // compile rules
                //
                //______________________________________________________________
                inline void compile_rules( syntax::xnode *rules )
                {
                    //__________________________________________________________
                    //
                    // first pass: register all the named rules
                    //__________________________________________________________
                    assert( !rules->terminal );
                    assert("RULES" == rules->label );
                    for( syntax::xnode *xn = rules->head(); xn; xn=xn->next)
                    {
                        assert("RULE" == xn->label);
                        register_rule(xn->head());
                    }
                    
                    //__________________________________________________________
                    //
                    // second pass: build tree
                    //__________________________________________________________
                    for( vnode *parent = vr.head; parent; parent=parent->next )
                    {
                        build(parent,parent->node);
                    }
                    
                }
                
                
                inline void build( vnode *parent, syntax::xnode *xn)
                {
                    //__________________________________________________________
                    //
                    // prepare a new parent node
                    //__________________________________________________________

                    const string & parent_name = parent->name;
                    std::cerr << "building '" << parent_name << "'" << std::endl;
                    int or_index = 0;
                    
                    for(;xn;xn=xn->next)
                    {
                        const string &label = xn->label;
                        std::cerr << "\t" << label << std::endl;
                        
                        //------------------------------------------------------
                        
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
                            child_addr->linked++;
                            continue;
                        }
                        
                        //------------------------------------------------------
                        
                        if( "ATOMS" == label )
                        {
                            //__________________________________________________
                            //
                            // this is a set of rules to append to parent
                            //__________________________________________________
                            build(parent,xn->head());
                            continue;
                        }
                        
                        //------------------------------------------------------
                        
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
                        
                        //------------------------------------------------------
                        
                        if( "CHAR" == label )
                        {
                            //__________________________________________________
                            //
                            // a single char to compile
                            //__________________________________________________
                            const string expr = xn->lxm()->to_string(1,1);
                            std::cerr << "\t|_'" << expr << "'" << std::endl;
                            assert(1==expr.size());
                            vnode *vn = find_char(expr);
                            if(!vn)
                            {
                                vn = new vnode(expr,NULL,vnode_char);
                                vc.push_back(vn);
                            }
                            parent->children.append(vn);
                            continue;
                        }
                        
                        //------------------------------------------------------
                        
                        if( "JOKER" == label )
                        {
                            //__________________________________________________
                            //
                            // a joker of sub rules
                            //__________________________________________________
                            const string attr  = xn->head()->lxm()->to_string();
                            const string jname = attr;
                            vnode *jk = new vnode(jname,xn,vnode_joker);
                            vj.push_back(jk);
                            parent->children.append(jk);
                            build(jk,xn->children().tail);
                            continue;
                        }
                        
                        //------------------------------------------------------
                        
                        if( "OR" == label )
                        {
                            const string oname = parent_name + vformat(".or%d",++or_index);
                            vnode *Or = new vnode(oname,xn,vnode_or);
                            vo.push_back(Or);
                            parent->children.append(Or);
                            
                            int i=0;
                            syntax::xnode::child_list tmp;
                            while(xn->count())
                            {
                                syntax::xnode *sub = xn->pop();
                                auto_ptr<syntax::xnode> p( sub );
                                //______________________________________________
                                //
                                // prepare a new group
                                //______________________________________________
                                const string grp_id = oname + vformat(".group%d",++i);
                                vnode *grp = new vnode(grp_id,p.__get(),vnode_group);
                                vo.push_back(grp);
                                Or->children.append(grp);
                                build(grp,sub);
                                
                                //______________________________________________
                                //
                                // reduce complexity
                                //______________________________________________
                                if(grp->children.size==1)
                                {
                                    vnode *content = grp->children.head->addr;
                                    delete Or->children.pop_back();
                                    delete vo.pop_back();
                                    Or->children.append(content);
                                }
                                
                                tmp.push_back( p.yield() );
                            }
                            while(tmp.size)
                            {
                                xn->add(tmp.pop_front());
                            }
                            continue;
                        }
                        
                        throw exception("unhandled %s", label.c_str());
                    }
                   
                    
                }
                
                
                inline void graphviz(const string &fn) const
                {
                    ios::ocstream fp(fn,false);
                    fp("digraph G {\n");
                    fp("\tordering=out;\n");
                    
                    // write all rules
                    for(const vnode *vn = vr.head; vn; vn=vn->next)
                    {
                        fp.viz(vn); fp(" [label=\"%s$%u\",shape=oval];\n", vn->name.c_str(),vn->linked);
                    }
                    
                    for(const vnode *vn = vs.head; vn; vn=vn->next)
                    {
                        fp.viz(vn); fp(" [label=\"%s\",shape=box];\n", vn->name.c_str());
                    }
                    
                    for(const vnode *vn = vc.head; vn; vn=vn->next)
                    {
                        fp.viz(vn); fp(" [label=\"%s\",shape=square];\n", vn->name.c_str());
                    }
                    
                    for(const vnode *vn = vj.head; vn; vn=vn->next)
                    {
                        fp.viz(vn); fp(" [label=\"%s\",shape=diamond];\n", vn->name.c_str());
                    }
                    
                    for(const vnode *vn = vo.head; vn; vn=vn->next)
                    {
                        fp.viz(vn); fp(" [label=\"%s\",shape=egg];\n", vn->name.c_str());
                    }
                    
                    
                    // make links
                    for(const vnode *vn = vr.head; vn; vn=vn->next)
                    {
                        link(fp,vn);
                    }
                    
                    fp("}\n");
                    
                }
                
                inline void link(ios::ostream &fp, const vnode *vn ) const
                {
                    int num=0;
                    for(const anode *an = vn->children.head; an; an=an->next)
                    {
                        const vnode *sub = an->addr;
                        fp("\t");
                        fp.viz(vn); fp(" -> "); fp.viz(sub);
                        if(vn->type == vnode_or)
                        {
                            fp( "[label=\"%d\",arrowhead=onormal]", ++num);
                        }
                        
                        if(vn->type==vnode_group)
                        {
                            fp( "[label=\"%d\",labelfont=\"Helvetica\"]", ++num);
                        }
                        
                        fp(";\n");
                        switch(sub->type)
                        {
                            case vnode_joker:
                            case vnode_or:
                            case vnode_group:
                                link(fp,sub);
                                break;
                                
                            default:
                                break;
                        }
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
            
            
            
            vcontext ctx;
            ctx.compile_rules(root);
            
            ctx.graphviz("gnode.dot");
            system("dot -Tpng -o gnode.png gnode.dot");
            
            //__________________________________________________________________
            //
            // generate parser
            //__________________________________________________________________
            //auto_ptr<parser> P( new parser("generated","scanner") );
            
            
        }
        
    }
    
}
