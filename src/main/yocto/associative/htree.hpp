#ifndef YOCTO_ASSOCIATIVE_HTREE_INCLUDED
#define YOCTO_ASSOCIATIVE_HTREE_INCLUDED 1

#include "yocto/container/container.hpp"
#include "yocto/core/list.hpp"
#include "yocto/type/args.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/string.hpp"
#include "yocto/sort/merge.hpp"
#include "yocto/comparator.hpp"

namespace yocto
{

    
    //! code based on minimal perfect hashing method
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
            mutable_type *data; //!< dynamically associated data, NULL=no data
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

            inline void optimize() throw()
            {
                for(node_type *node=chld.head;node;node=node->next)
                {
                    node->optimize();
                }
                core::merging<node_type>::sort(chld,self_compare,NULL);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
            static inline void self_compare(const node_type *lhs,const node_type *rhs,void *) throw()
            {
                return __compare_decreasing(lhs->freq,rhs->freq);
            }
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

        inline void optimize() throw()
        {
            root->optimize();
        }

        inline size_t size()  const throw() { return size_;  }
        inline size_t nodes() const throw() { return nodes_; }


        //______________________________________________________________________
        //
        // insertion
        //______________________________________________________________________
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

        inline const_type *find(const void  *buffer,
                                const size_t buflen) const throw()
        {
            assert(!(0==buffer&&buflen>0));
            const node_type *curr = root;
            const uint8_t   *addr = static_cast<const uint8_t *>(buffer);

            for(size_t i=buflen;i>0;--i)
            {
                const uint8_t code  = *(addr++);
                bool          found = false;

                for(const node_type *node=curr->chld.head;node;node=node->next)
                {
                    if(code==node->code)
                    {
                        found = true;
                        curr  = node;
                        break;
                    }
                }

                if(!found)
                {
                    return NULL;
                }
            }
            
            return curr->data; //!< may be null
        }

        inline const_type *find(const char *txt) const throw()
        {
            return this->find(txt,length_of(txt));
        }

        inline const_type *find(const memory::ro_buffer &buf) const throw()
        {
            return this->find(buf.ro(),buf.length());
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(htree);
        node_type *root;
        size_t     size_;
        size_t     nodes_;
    };
    
    
}

#endif