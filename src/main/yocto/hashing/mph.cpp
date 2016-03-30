#include "yocto/hashing/mph.hpp"
#include "yocto/exception.hpp"
#include "yocto/sort/merge.hpp"
#include "yocto/comparator.hpp"

namespace yocto
{
    namespace hashing
    {

        mperf::node_type:: ~node_type() throw()
        {
        }

        mperf::node_type:: node_type(const uint8_t c) throw() :
        next(0),
        prev(0),
        hash(-1),
        code(c),
        chld(),
        freq(1)
        {
        }

        void mperf::node_type:: viz( ios::ostream &fp ) const
        {
            fp.viz(this);
            const char *shape = (hash>=0) ? "egg" : "box";
            fp("[shape=%s,label=\"",shape);
            if(code>=32&&code<127)
            {
                fp("%c",char(code));
            }
            else
            {
                fp("x%02x",unsigned(code));
            }
            if(freq>1)
            {
                fp("#%u", unsigned(freq) );
            }
            if(hash>=0)
            {
                fp("[%d]",int(hash));
            }
            fp << "\"];\n";
            //int cnt = 0;
            for(const node_type *node=chld.head;node;node=node->next)
            {
                node->viz(fp);
                fp.viz(this); fp << " -> "; fp.viz(node);
                //fp( "[label=\"%d\"]",++cnt);
                fp << ";\n";
            }
        }

        static inline int __compare_mph_nodes(const mperf::node_type *lhs,
                                              const mperf::node_type *rhs,
                                              void *) throw()
        {
            return __compare_decreasing(lhs->freq,rhs->freq);
        }

        void mperf::node_type:: optimize() throw()
        {

            for(node_type *node = chld.head;node;node=node->next)
            {
                node->optimize();
            }
            core::merging<node_type>::sort(chld, __compare_mph_nodes, 0);
        }


    }
}

#include <iostream>

namespace yocto
{
    namespace hashing
    {
        mperf:: ~mperf() throw()
        {
            assert(root);
            delete root; root=0;
        }

        mperf:: mperf() :
        root( new node_type(0) ),
        nodes(1)
        {
        }

        mperf:: mperf( const char **words, const size_t count) :
        root( new node_type(0) ),
        nodes(1)
        {
            assert(!(words==NULL&&count>0));
            for(size_t i=0;i<count;++i)
            {
                const char *word = words[i];
                insert(word,i);
            }
            optimize();
        }


        void mperf:: insert(const void *data, const size_t size, const int h)
        {
            assert( ! ( (NULL==data) && (size>0) ) );
            assert(h>=0);
            if(h<0)   throw exception("mp_hasher: invalid user's hash code=%d",h);
            if(!size) return;

            //__________________________________________________________________
            //
            // initialize look up: start from root
            //__________________________________________________________________

            const uint8_t *addr = static_cast<const uint8_t *>(data);
            node_type     *curr = root;

            for(size_t i=size;i>0;--i)
            {
                const uint8_t  code  = *(addr++);
                bool           found = false;

                //______________________________________________________________
                //
                // find which child to follow
                //______________________________________________________________
                node_type::list_type &chld = curr->chld;
                for(node_type *node = chld.head;node;node=node->next)
                {
                    if(code==node->code)
                    {
                        found = true;
                        curr  = node;
                        ++(curr->freq);
                        break;
                    }
                }

                if(!found)
                {
                    curr->chld.push_back(new node_type(code));
                    curr = curr->chld.tail;
                    ++(size_t &)nodes;
                }
            }

            //__________________________________________________________________
            //
            // current is the last visited node
            //__________________________________________________________________

            assert(curr);
            assert(curr!=root);
            if(curr->hash>=0)
                throw exception("mp_hasher: multiple insertions!");
            (int &)(curr->hash) = h;


        }


    }
}


namespace yocto
{
    namespace hashing
    {

        int mperf:: find(const void *data, const size_t size) const throw()
        {
            assert( ! ( (NULL==data) && (size>0) ) );
            const uint8_t   *addr = static_cast<const uint8_t *>(data);
            const node_type *curr = root;
            for(size_t i=size;i>0;--i)
            {
                const uint8_t        code    = *(addr++);
                bool                 found   = false;;
                for(const node_type *node = curr->chld.head;node;node=node->next)
                {
                    if(code==node->code)
                    {
                        curr = node;
                        found= true;
                        break;
                    }
                }

                if(!found)
                {
                    return -1;
                }
            }
            return curr->hash;
        }

    }
}

#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace hashing
    {

        void mperf:: graphviz( const string &filename ) const
        {
            {
                ios::ocstream fp(filename,false);
                fp << "digraph G{\n";
                root->viz(fp);
                fp << "}\n";
            }
            
        }
        
    }
    
}


#include "yocto/ios/imstream.hpp"
#include "yocto/ios/net-string.hpp"

namespace yocto
{
    namespace hashing
    {
        mperf:: mperf(const char *nsdb) :
        root( new node_type(0) ),
        nodes(1)
        {
            ios::imstream fp(nsdb,length_of(nsdb));
            string        word;
            int           indx=0;
            while( ios::net_string::read(fp, word))
            {
                insert(word, indx);
                ++indx;
            }
            optimize();
        }

    }
}
