#ifndef YOCTO_SEQ_ADDR_LIST_INCLUDED
#define YOCTO_SEQ_ADDR_LIST_INCLUDED 1

#include "yocto/core/list.hpp"
#include "yocto/object.hpp"

namespace yocto
{
    
    template <typename T>
    class addr_node
    {
    public:
        YOCTO_MAKE_OBJECT
        
        addr_node *next; //!< for list
        addr_node *prev; //!< for list
        T         *addr; //!< for object
        inline addr_node( T *ptr ) throw() : next(0), prev(0), addr(ptr) {}
        inline ~addr_node() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(addr_node);
    };
    
    
    template <typename T>
    class addr_list : public core::list_of_cpp< addr_node<T> >
    {
    public:
        typedef addr_node<T>                 node_type;
        typedef core::list_of_cpp<node_type> list_type;
        explicit addr_list() throw() : list_type() {}
        virtual ~addr_list() throw()  {}
        
        explicit addr_list( const addr_list &other ) : list_type()
        {
            try
            {
                for(const node_type *node = other.head; node; node=node->next )
                {
                    push_back( new node_type(node->addr) );
                }
            }
            catch(...)
            {
                this->clear();
                throw;
            }
        }
        
        
        inline void append( T *ptr ) { this->push_back( new node_type(ptr) ); }
        inline void remove() throw() { delete this->pop_back(); }
        
    private:
        YOCTO_DISABLE_ASSIGN(addr_list);
    };
    
}

#endif

