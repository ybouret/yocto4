#ifndef YOCTO_CIRCULAR_LIST_INCUDED
#define YOCTO_CIRCULAR_LIST_INCUDED 1

#include "yocto/code/bswap.hpp"

namespace yocto
{
    
    namespace core
    {
        
        //! circular list
        template <typename NODE>
        class clist_of
        {
        public:
            size_t size;
            NODE  *root;
            explicit clist_of() throw() : size(0), root(0) {}
            virtual ~clist_of() throw() { assert(0==size); assert(0==root); }
            
            //! list_of compatibility
            inline void reset() throw() { size = 0; root = 0; }
            
            //! exchange content
            void swap_with( clist_of &other ) throw()
            {
                cswap(size,other.size);
                cswap(root,other.root);
            }
            
            //! list_of compatibility
            inline void push_back( NODE *node ) throw()
            {
                assert(node);
                assert(NULL==node->prev);
                assert(NULL==node->next);
                //assert(!this->owns(node));
                switch( size )
                {
                    case 0:
                        assert(NULL==root);
                        root = node;
                        break;
                        
                    case 1:
                        assert(NULL!=root);
                        root->next = node;
                        root->prev = node;
                        node->prev = root;
                        node->next = root;
                        break;
                        
                    default:
                        __push_back(node);
                        break;
                }
                ++size;
            }
            
            //! list_of compatibility
            inline void push_front( NODE *node ) throw()
            {
                assert(node);
                assert(NULL==node->prev);
                assert(NULL==node->next);
                //assert(!this->owns(node));
                switch( size )
                {
                    case 0:
                        assert(NULL==root);
                        root = node;
                        break;
                        
                    case 1:
                        assert(NULL!=root);
                        root->next = node;
                        root->prev = node;
                        node->prev = root;
                        node->next = root;
                        break;
                        
                    default:
                        __push_back(node);
                        root = root->prev;
                        break;
                }
                ++size;
                
            }
            
            //! list_of compatibility
            inline NODE * pop_back() throw()
            {
                assert(size>0);
                switch(size)
                {
                    case 1: {
                        assert(root!=NULL);
                        assert(NULL==root->next);
                        assert(NULL==root->prev);
                        NODE *node = root;
                        root = NULL;
                        size = 0;
                        return node;
                    }
                        
                    case 2:
                    {
                        assert(root->next==root->prev);
                        NODE *prev = root->prev;
                        assert(prev->prev==root);
                        assert(prev->next==root);
                        root->next = root->prev = NULL;
                        prev->next = prev->prev = NULL;
                        size = 1;
                        return prev;
                    }
                        
                    default:
                        break;
                }
                assert(size>2);
                NODE *prev = root->prev;
                assert(prev->next==root);
                assert(root->next!=prev);
                assert(prev->prev!=root);
                
                prev->prev->next = root;
                root->prev       = prev->prev;
                prev->next = prev->prev = NULL;
                --size;
                return prev;
            }
            
            //! list_of compatibility
            inline NODE *pop_front()
            {
                assert(size>0);
                if( size >= 2 )
                    root = root->next;
                return pop_back();
            }
            
            //! list_of compatibility
            inline bool owns( const NODE *node ) const throw()
            {
                assert(!(root==NULL&&size>0));
                const NODE *scan = root;
                for( size_t i=size;i>0;--i,scan=scan->next)
                {
                    if( node == scan )
                        return true;
                }
                return false;
            }
            
            //! specific code
            inline void insert_after( NODE *curr, NODE *node ) throw()
            {
                assert(curr!=NULL);assert(this->owns(curr));
                assert(node!=NULL);assert(!this->owns(node));
                assert(NULL==node->next); assert(NULL==node->prev);
                assert(size!=0);
                if( size == 1 )
                {
                    assert(curr==root);
                    root->next = node;
                    root->prev = node;
                    node->prev = root;
                    node->next = root;
                    
                }
                else
                {
                    assert(size>=2);
                    assert(curr->next != NULL );
                    NODE *_nxt = curr->next;
                    curr->next = node;
                    node->prev = curr;
                    node->next = _nxt;
                    _nxt->prev = node;
                }
                ++size;
                assert(this->owns(curr));
                assert(this->owns(node));
            }
            
            //! fetch in 0..size-1, list_of compatibility
            inline NODE *fetch( size_t index ) throw()
            {
                assert(index<size);
                assert(size>0);
                assert(root!=NULL);
                NODE *node = root;
                while(index-->0) node=node->next;
                return node;
            }
            
            //! list_of compatibility
            inline NODE *unlink( NODE *node ) throw()
            {
                assert(node!=NULL);
                assert(this->owns(node));
                assert(size>0);
                switch(size)
                {
                        
                    case 1: {
                        assert(node==root);
                        NODE *ans = pop_front();
                        assert(ans==node);
                        return ans; }
                        
                        
                    case 2: {
                        if( node == root )
                        {
                            root = root->next;
                        }
                        NODE *ans = pop_back();
                        assert(ans==node);
                        return ans; }
                        
                    default:
                        break;
                }
                assert(size>2);
                if( node == root )
                {
                    root = root->next;
                    NODE *ans = pop_back();
                    assert(ans==node);
                    return ans;
                }
                else
                {
                    NODE *prev = node->prev;
                    NODE *next = node->next;
                    prev->next = next;
                    next->prev = prev;
                    node->next = node->prev = NULL;
                    --size;
                    return node;
                }
            }
            
            inline void auto_delete() throw()
            {
                while(size) delete pop_back();
            }
            
        private:
            inline void __push_back( NODE *node ) throw()
            {
                assert(size>=2);
                assert(NULL!=root); assert(NULL!=root->prev); assert(NULL!=root->next);
                NODE *prev = root->prev; assert(prev->prev!=NULL); assert(prev->next==root);
                prev->next = node;
                node->prev = prev;
                root->prev = node;
                node->next = root;
            }
            YOCTO_DISABLE_COPY_AND_ASSIGN(clist_of);
        };
        
        
        template <typename NODE>
        class clist_of_cpp : public clist_of<NODE>
        {
        public:
            explicit clist_of_cpp() throw() : clist_of<NODE>() {}
            virtual ~clist_of_cpp() throw() { clear(); }
            
            inline void clear() throw() { this->auto_delete(); }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(clist_of_cpp);
        };
        
    }
    
}

#endif
