#include "yocto/lang/generator.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/associative/map.hpp"

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
            
            
            class vnode : public object
            {
            public:
                typedef vnode *                  ptr;
                
                const syntax::xnode *node;
                const string         name;
                const vnode_type     type;
                vnode               *next;
                vnode               *prev;
                vlist                children;
                
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
            
            
            static inline std::ostream & indent(std::ostream &fp, int n)
            {
                for(int i=0;i<=n;++i) fp << "    ";
                return fp;
            }
            
            static inline void build_rule(vnode *vn,
                                          vmap  &vm,
                                          vlist &vl,
                                          const int depth )
            {
                
            }
            
            static inline
            void build( vnode *vn, vmap &vm, vlist &vl, const int depth)
            {
                assert(vn);
                indent(std::cerr,depth) << "build '" << vn->name << "'" << std::endl;
                switch(vn->type)
                {
                    case vnode_rule:
                        build_rule(vn,vm,vl,depth+1);
                        break;
                        
                    default:
                        throw exception("not handled");
                }
                
            }
            
            
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
            vlist vr;
            vmap  vm;
            
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
            
            //__________________________________________________________________
            //
            // Then recursively collect rule content
            //__________________________________________________________________
            vlist vl; // list of local nodes, for memory
            for(vnode *vn=vr.head;vn;vn=vn->next)
            {
                build(vn,vm,vl,0);
            }
            
            
            //__________________________________________________________________
            //
            // generate parser
            //__________________________________________________________________
            //auto_ptr<parser> P( new parser("generated","scanner") );
            
            
            
            
        }
        
    }
    
}
