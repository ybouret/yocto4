#ifndef YOCTO_ASSOCIATIVE_HTREE_INCLUDED
#define YOCTO_ASSOCIATIVE_HTREE_INCLUDED 1

#include "yocto/container/container.hpp"
#include "yocto/core/list.hpp"
#include "yocto/type/args.hpp"
#include "yocto/string.hpp"
#include "yocto/sort/merge.hpp"
#include "yocto/comparator.hpp"
#include "yocto/code/round.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{

    
    //! code based on minimal perfect hashing method
    template <typename T>
    class htree
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        typedef void (*print_proc)(const_type &,ios::ostream&);

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
                    destruct<mutable_type>(data);
                    object::release1<mutable_type>(data);
                }
            }

            //! build data into workspace
            inline void build(param_type args)
            {
                assert(NULL==data);
                data = object::acquire1<mutable_type>();
                try
                {
                    new (data) mutable_type(args);
                }
                catch(...)
                {
                    object::release1<mutable_type>(data);
                    throw;
                }
            }

            //! optimize by frequencies
            inline void optimize() throw()
            {
                for(node_type *node=chld.head;node;node=node->next)
                {
                    node->optimize();
                }
                core::merging<node_type>::sort(chld,self_compare,NULL);
            }

            YOCTO_MAKE_OBJECT

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
            static inline int self_compare(const node_type *lhs,const node_type *rhs,void *) throw()
            {
                const int ans = __compare_decreasing(lhs->freq,rhs->freq);
                if(0!=ans)
                {
                    return ans;
                }
                else
                {
                    return __compare(lhs->code,rhs->code);
                }
            }

        public:
            inline void viz(ios::ostream &fp, print_proc proc) const
            {
                fp.viz(this);
                const char *shape = (data!=NULL) ? "egg" : "box";
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
                if(data!=NULL)
                {
                    if(proc)
                    {
                        proc(*data,fp);
                    }
                    else
                    {
                        fp("*");
                    }
                }
                fp << "\"];\n";
                for(const node_type *node=chld.head;node;node=node->next)
                {
                    node->viz(fp,proc);
                    fp.viz(this); fp << " -> "; fp.viz(node);
                    fp << ";\n";
                }
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
        inline size_t bytes() const throw() { return sizeof(T) * size_ + sizeof(node_type) * nodes_; }

        //______________________________________________________________________
        //
        //! insertion
        //______________________________________________________________________
        inline bool insert(const void  *buffer,
                           const size_t buflen,
                           param_type   args)
        {
            assert( !(0==buffer&&buflen>0) );

            //__________________________________________________________________
            //
            // initialize look up: start from root
            //__________________________________________________________________
            const uint8_t                 *addr = static_cast<const uint8_t *>(buffer);
            node_type                     *curr = root;
            typename node_type::list_type *from = NULL;

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
                        from  = & (curr->chld);
                        curr  = node;
                        assert(from->owns(curr));
                        ++(curr->freq);
                        break;
                    }
                }

                if(!found)
                {
                    curr->chld.push_back( new node_type(code) );
                    from  = &(curr->chld);
                    curr = curr->chld.tail;
                    assert(from->owns(curr));
                    ++nodes_;
                }
            }

            //__________________________________________________________________
            //
            // found where we are
            //__________________________________________________________________
            if(0!=curr->data)
            {
                // node already occupied !
                return false;
            }

            //__________________________________________________________________
            //
            // build data
            //__________________________________________________________________
            try
            {
                curr->build(args);
            }
            catch(...)
            {
                if(from)
                {
                    delete from->unlink(curr);
                    --nodes_;
                }
            }
            ++size_;
            return true;
        }

        //!insertion wrapper
        inline bool insert(const char  *text,
                           param_type   args)
        {
            return this->insert(text,length_of(text),args);
        }

        //! insertion wrapper
        inline bool insert(const memory::ro_buffer &buff,
                           param_type               args)
        {
            return this->insert(buff.ro(),buff.length(),args);
        }

        //______________________________________________________________________
        //
        //! look up, CONST
        //______________________________________________________________________
        inline const_type *find(const void  *buffer,
                                const size_t buflen) const throw()
        {
            return lookup(buffer,buflen);
        }

        //! lookup CONST wrapper
        inline const_type *find(const char *txt) const throw()
        {
            return lookup(txt,length_of(txt));
        }

        //! lookup CONST wrapper
        inline const_type *find(const memory::ro_buffer &buf) const throw()
        {
            return lookup(buf.ro(),buf.length());
        }

        //______________________________________________________________________
        //
        //! look up
        //______________________________________________________________________
        inline type *find(const void  *buffer,
                          const size_t buflen)  throw()
        {
            return (type *)lookup(buffer,buflen);
        }

        //! lookup CONST wrapper
        inline  type *find(const char *txt) throw()
        {
            return (type*)lookup(txt,length_of(txt));
        }

        //! lookup CONST wrapper
        inline type *find(const memory::ro_buffer &buf) throw()
        {
            return (type*)lookup(buf.ro(),buf.length());
        }


        inline void graphviz( const string &filename, print_proc proc) const
        {
            ios::ocstream fp(filename,false);
            fp << "digraph G{\n";
            root->viz(fp,proc);
            fp << "}\n";
        }


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(htree);
        node_type *root;
        size_t     size_;
        size_t     nodes_;

        inline const_type *lookup(const void  *buffer,
                                const size_t buflen) const throw()
        {
            assert(!(0==buffer&&buflen>0));

            //__________________________________________________________________
            //
            // Initialize travel
            //__________________________________________________________________
            const node_type *curr = root;
            const uint8_t   *addr = static_cast<const uint8_t *>(buffer);

            //__________________________________________________________________
            //
            // Travelling down
            //__________________________________________________________________
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

    };
    
    
}

#endif