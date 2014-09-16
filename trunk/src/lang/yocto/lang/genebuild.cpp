#include "yocto/lang/generator.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/exception.hpp"

#include <cstdlib>

namespace yocto
{
    namespace lang
    {
        namespace
        {
            
            class vrule;
            
            class vrule_node : public object
            {
            public:
                vrule_node  *next;
                vrule_node  *prev;
                const vrule *handle;
                
                explicit vrule_node(vrule *h) throw() :
                next(0), prev(0), handle(h)
                {
                    assert(handle);
                }
                
                virtual ~vrule_node() throw()
                {
                }
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(vrule_node);
            };
            
            typedef core::list_of_cpp<vrule_node> vrule_list;
            
            
            class vrule : public object, public counted
            {
            public:
                const syntax::xnode *node;
                const string         name;
                const string &key() const throw() { return name; }
                
                explicit vrule(const syntax::xnode *nd) throw() :
                node(nd),
                name(nd->lxm()->to_string())
                {
                }
                
                virtual ~vrule() throw()
                {
                }
                
                typedef intr_ptr<string,vrule> pointer;
                typedef set<string,pointer>    database;
                
                vrule_list parents;
                vrule_list children;
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(vrule);
                
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
            // first pass: build database of rules
            //__________________________________________________________________
            vrule::database vdb;
            {
                const syntax::xnode::child_list &ch = root->children();
                vdb.reserve(ch.size);
                for(const syntax::xnode *node = ch.head; node; node=node->next)
                {
                    if(node->label == "RULE" )
                    {
                        const syntax::xnode::child_list &content = node->children();
                        assert(content.size>0);
                        const syntax::xnode *nd = content.head;
                        assert(nd->label == "ID" );
                        assert(nd->terminal);
                        const vrule::pointer p( new vrule(nd) );
                        std::cerr << "Registering rule '"  << p->name << "'" << std::endl;
                        if( !vdb.insert(p) )
                        {
                            throw exception("multiple rule '%s'", p->name.c_str());
                        }
                        continue;
                    }
                    
                    throw exception("cannot process '%s'", node->label.c_str());
                }
            }
            
            //__________________________________________________________________
            //
            // second pass: dependency and terminals
            //__________________________________________________________________
            for( vrule::database::iterator i=vdb.begin();i!=vdb.end();++i)
            {
                vrule *parent = &(**i);
                std::cerr << "\tprocessing " << parent->name << std::endl;
                for(const syntax::xnode *node = parent->node->next; node; node=node->next)
                {
                    std::cerr << "\t\t" << node->label << std::endl;
                }
            }
            
            
        }
        
    }
    
}
