#include "yocto/utest/run.hpp"
#include "yocto/core/list.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/sort/merge.hpp"
#include "yocto/comparator.hpp"

using namespace yocto;

namespace
{

    class Node
    {
    public:
        Node         *next;
        Node         *prev;
        const uint8_t code;
        int           H;
        core::list_of_cpp<Node> children;

        static inline int Compare(const Node *lhs, const Node *rhs, void *) throw()
        {
            return __compare(lhs->code,rhs->code);
        }


        Node(const int c) throw() :
        next(0), prev(0), code(c), H(-1){}
        ~Node() throw() {}

        inline void viz( ios::ostream &fp ) const
        {
            fp.viz(this);
            if(code>=0)
            {
                if(H>=0)
                {
                    fp("[shape=egg,label=\"%c, [%d]\"];\n", char(code), H);
                }
                else
                {
                    if(code>=32&&code<127)
                    {
                        fp("[shape=egg,label=\"%c\"]\n;", char(code));
                    }
                    else
                    {
                        fp("[shape=egg,label=\"x%02x\"]\n;", unsigned(code));
                    }
                }
            }
            else
            {
                fp("[shape=box,label=\"root\"]\n;");
            }

            for(const Node *node=children.head; node; node=node->next)
            {
                node->viz(fp);
                fp.viz(this); fp << " -> "; fp.viz(node); fp << ";\n";
            }
        }

        YOCTO_MAKE_OBJECT



    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Node);

    };

    class Tree
    {
    public:
        Node  *root;
        size_t nodes;

        explicit Tree() :
        root( new Node(0) ),
        nodes(1)
        {
        }

        virtual ~Tree() throw()
        {
            delete root; root=0;
        }

        void insert( const void *data, size_t size, const int H)
        {
            if(!size) return;
            assert(!(data==NULL&&size>0));
            assert(H>=0);

            const uint8_t *C = (const uint8_t *)data;
            Node *curr = root;
            for(size_t i=0;i<size;++i)
            {
                assert(curr);
                const uint8_t  code  = C[i];
                bool           found = false;

                for(Node *node = curr->children.head;node;node=node->next)
                {
                    if(code==node->code)
                    {
                        curr  = node;
                        found = true;
                        break;
                    }
                }

                if(!found)
                {
                    Node *node = new Node(code);
                    curr->children.push_back(node);
                    core::merging<Node>::sort(curr->children, Node::Compare,0);
                    curr = node;
                    ++nodes;
                }
            }
            assert(curr); assert(curr!=root);
            if(curr->H>=0)
            {
                throw exception("already inserted!");
            }
            curr->H = H;
        }

        void insert( const memory::ro_buffer &buffer, const int H )
        {
            insert( buffer.ro(), buffer.length(), H);
        }

        void graphviz() const
        {
            {
                ios::ocstream fp("htree.dot",false);
                fp << "digraph G {\n";
                root->viz(fp);
                fp << "}\n";
            }
            (void) system("dot -Tpng -o htree.png htree.dot");
        }

        int Hash( const void *data, const size_t size ) const throw()
        {
            assert(!(data==NULL&&size>0));
            const uint8_t *C    = (const uint8_t *)data;
            const Node    *curr = root;

            for(size_t i=0;i<size;++i)
            {
                const uint8_t code = C[i];
                bool          found = false;
                for(const Node *node = curr->children.head;node;node=node->next)
                {
                    if(code==node->code)
                    {
                        curr  = node;
                        found = true;
                        break;
                    }
                }
                if(!found) break;
            }

            assert(curr);
            return curr->H;
        }


        int Hash( const memory::ro_buffer &buff ) const throw()
        {
            return Hash(buff.ro(),buff.length());
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Tree);
    };

}

YOCTO_UNIT_TEST_IMPL(phash)
{
    vector<string> words;
    {
        string line;
        ios::icstream fp( ios::cstdin );
        while( line.clear(), fp.read_line(line) >= 0 ) words.push_back(line);
    }
    std::cerr << "words=" << words  << std::endl;

    Tree tree;
    for(size_t i=1;i<=words.size();++i)
    {
        tree.insert(words[i],i-1);
    }

    tree.graphviz();
    std::cerr << "#node=" << tree.nodes << std::endl;
    for(size_t i=1;i<=words.size();++i)
    {
        std::cerr << "Hash(" << words[i] <<")=" << tree.Hash(words[i]) << std::endl;
    }

    string s = "dummy";
    std::cerr << "Hash(" << s << ")=" << tree.Hash(s) << std::endl;

}
YOCTO_UNIT_TEST_DONE()
