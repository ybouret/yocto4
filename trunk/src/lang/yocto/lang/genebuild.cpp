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
                vnode_term
            };
            
            class vnode;
            
            typedef core::list_of_cpp<vnode> vlist;
            typedef addr_list<vnode>         alist;
            
            class vnode : public object
            {
            public:
                typedef vnode *                  ptr;
                
                const syntax::xnode *node;
                const string         name;
                const vnode_type     type;
                vnode               *next;
                vnode               *prev;
                alist                children;
                
                explicit vnode( const syntax::xnode *nd ) :
                node(nd),
                name(nd->lxm()->to_string()),
                type(vnode_rule),
                next(0),
                prev(0)
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
                vlist vr; //!< virtual top level rules
                vmap  vm; //!< named rules
                vlist vl; //!< local rules
                
                inline vcontext() : vr(), vm(), vl()
                {
                    
                }
                
                inline ~vcontext() throw()
                {
                    
                }
                
                //______________________________________________________________
                //
                // top level rules collection
                //______________________________________________________________
                inline void collect_rules_from(const syntax::xnode *root )
                {
                    
                    // TODO: better
                    if(root->count()<=0)
                        throw exception("no rules");
                    
                    // first pass: named rules
                    for(const syntax::xnode *r=root->head();r;r=r->next)
                    {
                        if( "RULE" == r->label )
                        {
                            const syntax::xnode *nd = r->head();
                            assert("ID"==nd->label);
                            vnode *vn = new vnode(nd);
                            vr.push_back(vn);
                            if(!vm.insert(vn->name,vn))
                            {
                                throw exception("multiple top level rule '%s'", vn->name.c_str());
                            }
                            std::cerr << "registered '" << vr.tail->name << "'" << std::endl;
                            continue;
                        }
                        
                        throw exception("can't handle %s", r->label.c_str());
                    }
                    
                    // second pass: virtual tree
                    build(vr.head);
                }
                
                //______________________________________________________________
                //
                // virtual tree building 
                //______________________________________________________________
                inline void build(vnode *vn)
                {
                    assert(vn);
                    switch(vn->type)
                    {
                        case vnode_rule:
                            build_rule(vn);
                            break;
                            
                        default:
                            throw exception("can't handle this type");
                    }
                }
                
                //______________________________________________________________
                //
                // parsing a rule
                //______________________________________________________________
                inline void build_rule(vnode *vn)
                {
                    assert(vn);
                    assert(vnode_rule==vn->type);
                    std::cerr << "building from rule " << vn->name << std::endl;
                    const syntax::xnode *xn = vn->node;
                    
                    //__________________________________________________________
                    //
                    // first xnode must be ID since it's a rule
                    //__________________________________________________________

                    assert("ID"==xn->label);
                    for(xn=xn->next;xn;xn=xn->next)
                    {
                        const string &label = xn->label;
                        std::cerr << "\t\t" << label << std::endl;
                        
                        if( "ID" == label )
                        {
                            //__________________________________________________
                            //
                            // a sub rule
                            //__________________________________________________
                            const string id = xn->lxm()->to_string();
                            std::cerr << "\t\t|_" << id << std::endl;
                            continue;
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
            
            
            //__________________________________________________________________
            //
            // Let's rebuild the tree: gather top level rules
            //__________________________________________________________________
            vcontext ctx;
            ctx.collect_rules_from(root);
            
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
