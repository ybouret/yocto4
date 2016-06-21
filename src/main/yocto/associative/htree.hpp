#ifndef YOCTO_ASSOCIATIVE_HTREE_INCLUDED
#define YOCTO_ASSOCIATIVE_HTREE_INCLUDED 1

#include "yocto/container/container.hpp"
#include "yocto/core/list.hpp"
#include "yocto/type/args.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/string.hpp"

namespace yocto
{

#if 0
    namespace core
    {
        class htree
        {
        public:
            class node_type
            {
            public:
                typedef core::list_of_cpp<node_type> list_type;
                node_type      *next; //!< for list
                node_type      *prev; //!< for list
                const int       hash; //!< in-order hash code
                const uint8_t   code; //!< associated byte
                list_type       chld; //!< child(ren) nodes
                unsigned        freq; //!< frequency

                ~node_type() throw();
                node_type(const uint8_t C) throw();

                void viz( ios::ostream &fp ) const; //!< output graphviz
                void optimize() throw(); //!< order by decreasing frequency


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
            };

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(htree);
        };
    }
#endif

    template <typename T>
    class htree
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        typedef type *pointer_type;

        class node_type
        {
        public:
            typedef core::list_of_cpp<node_type> list_type;
            mutable_type *data; //!< dynamically associated data
            node_type    *next; //!< for list
            node_type    *prev; //!< for list
            const uint8_t code; //!< associated byte
            list_type     chld; //!< child(ren) nodes
            unsigned      freq; //!< frequency

            //! default ctor
            inline node_type(const uint8_t C) throw() :
            data(0),
            next(0),
            prev(0),
            code(C),
            chld(),
            freq(0)
            {
            }

            //! default dtor
            inline ~node_type() throw()
            {
                if(data)
                {
                    delete data;
                    data = 0;
                }
            }

            YOCTO_MAKE_OBJECT

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
        };

        inline explicit htree() :
        root( new node_type(0) ),
        size_(0),
        nodes_(1)
        {
        }

        inline virtual ~htree() throw()
        {
            delete root;
            root = 0;
        }

        inline size_t size()  const throw() { return size_; }
        inline size_t nodes() const throw() { return nodes_; }

        inline bool insert(const void  *buffer,
                           const size_t buflen,
                           pointer_type pobj )
        {
            assert(pobj); assert( !(0==buffer&&buflen>0) );
            auto_ptr<type> guard(pobj);
            //__________________________________________________________________
            //
            // initialize look up: start from root
            //__________________________________________________________________
            const uint8_t *addr = static_cast<const uint8_t *>(buffer);
            node_type     *curr = root;

            //__________________________________________________________________
            //
            // find children
            //__________________________________________________________________
            for(size_t i=buflen;i>0;--i)
            {
                const uint8_t code  = *(addr++);
                bool          found = false;

                for(node_type *node=curr->chld.head;node;node=node->next)
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
                    curr->chld.push_back( new node_type(code) );
                    curr = curr->chld.tail;
                    ++nodes_;
                }
            }

            if(0!=curr->data)
            {
                // node already occupied !
                return false;
            }

            curr->data = guard.yield();
            ++size_;
            return true;
        }

        inline bool insert(const char  *txt,
                           pointer_type pobj)
        {
            return this->insert(txt,length_of(txt),pobj);
        }

        inline bool insert(const memory::ro_buffer &buf,
                           pointer_type             pobj)
        {
            return this->insert(buf.ro(),buf.length(),pobj);
        }


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(htree);
        node_type *root;
        size_t     size_;
        size_t     nodes_;
    };


}

#endif