#include "yocto/hashing/perfect.hpp"
#include "yocto/sort/merge.hpp"
#include "yocto/comparator.hpp"

namespace yocto
{
    namespace hashing
    {

        perfect::node_type::node_type(const uint8_t c) throw() :
        next(0),
        prev(0),
        hash(-1),
        chld(),
        code(c),
        freq(1)
        {
        }

        perfect:: node_type:: ~node_type() throw()
        {

        }

        void perfect::node_type:: viz( ios::ostream &fp ) const
        {
            fp.viz(this);
            fp << "[shape=egg,label=\"";
            if(code>=32&&code<127)
            {
                fp("%c",char(code));
            }
            else
            {
                fp("x%02x",unsigned(code));
            }
            fp(",#%u", unsigned(freq) );
            if(hash>=0)
            {
                fp(",[%d]",hash);
            }
            fp << "\"];\n";
            for(const node_type *node=chld.head;node;node=node->next)
            {
                node->viz(fp);
                fp.viz(this); fp << " -> "; fp.viz(node); fp << ";\n";
            }
        }

        static int compare_by_freq(const perfect::node_type *lhs, const perfect::node_type *rhs, void *) throw()
        {
            return __compare_decreasing(lhs->freq, rhs->freq);
        }

        void perfect::node_type:: optimize() throw()
        {
            core::merging<node_type>::sort(chld, compare_by_freq, 0);
            for(node_type *node=chld.head;node;node=node->next)
            {
                node->optimize();
            }
        }

    }
}

#include "yocto/exception.hpp"

namespace yocto
{
    namespace hashing
    {
        perfect:: perfect() :
        root( new node_type(0) ),
        nodes(1)
        {
        }

        perfect:: perfect( const char **words, const size_t count) :
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


        perfect:: ~perfect() throw()
        {
            delete root; root=0;
        }

    }
}


namespace yocto
{
    namespace hashing
    {
        void perfect:: insert( const memory::ro_buffer &buff, const int h)
        {
            insert(buff.ro(),buff.length(),h);
        }

        void perfect:: insert( const char *txt , const int h)
        {
            insert(txt, length_of(txt), h);
        }


        void perfect:: insert(const void *data, const size_t size, const int h)
        {
            assert(!(data==NULL&&size>0));
            assert(h>=0);

            if(!size) return;

            const uint8_t *C    = (const uint8_t *)data;
            node_type     *curr = root;

            for(size_t i=0;i<size;++i)
            {
                const uint8_t code  = C[i];
                bool          found = false;

                for(node_type *node = curr->chld.head;node;node=node->next)
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

            assert(curr);
            assert(curr!=root);
            if(curr->hash>=0)
                throw exception("hashing::perfect: multiple insertions!");
            (int &)(curr->hash) = h;
        }



    }

}

namespace yocto
{
    namespace hashing
    {

        int  perfect:: operator()(const void *data, const size_t size) const throw()
        {
            assert(!(data==NULL&&size>0));

            const uint8_t   *C    = (const uint8_t *)data;
            const node_type *curr = root;
            for(register size_t i=0;i<size;++i)
            {
                const uint8_t code  = C[i];
                bool          found = false;
                for(const node_type *node = curr->chld.head;node;node=node->next)
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
                    break;
                }
            }
            return curr->hash;
        }


        int  perfect:: operator()(const memory::ro_buffer &buff ) const throw()
        {
            return (*this)( buff.ro(), buff.length() );
        }

        int  perfect:: operator()(const char *txt) const throw()
        {
            return  (*this)(txt,length_of(txt));
        }


        void perfect:: optimize() throw()
        {
            assert(root);
            root->optimize();
        }


    }

}


#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace hashing
    {
        
        void hashing::perfect:: graphviz( const string &filename ) const
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


