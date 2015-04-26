#ifndef YOCTO_CORE_LIST_INCLUDED
#define YOCTO_CORE_LIST_INCLUDED 1

#include "yocto/code/bswap.hpp"

namespace yocto
{
	
	namespace core
	{
		//! list of nodes
		template <typename NODE>
		class list_of
		{
		public:
			inline explicit list_of() throw() : head(NULL), tail(NULL), size(0) {}
			inline virtual ~list_of() throw() { assert(NULL==head); assert(NULL==tail); assert(0==size); }
			
			NODE  *head;
			NODE  *tail;
			size_t size;
			
			void swap_with( list_of &other ) throw()
			{
				cswap( head, other.head );
				cswap( tail, other.tail );
				cswap( size, other.size );
			}
						
			inline void push_back( NODE *node ) throw()
			{
				assert(NULL!=node);
                assert(NULL==node->next);
                assert(NULL==node->prev);
                //assert( !owns(node));
				if( size <= 0 )
				{
					push_first(node);
				}
				else 
				{
					node->prev = tail;
					tail->next = node;
					tail       = node;
					++size;
				}
				
			}
			
			inline void push_front( NODE *node ) throw()
			{
				assert(NULL!=node);
                assert(NULL==node->next);
                assert(NULL==node->prev);
                //assert( !owns(node));
				if( size <= 0 )
				{
					push_first(node);
				}
				else 
				{
                    assert(head!=NULL);
					node->next = head;
					head->prev = node;
					head       = node;
					++size;
				}
				
			}
			
			inline NODE *pop_back() throw()
			{
				assert(size>0);
                assert(head!=NULL);
                assert(tail!=NULL);
				if( size <= 1 )
					return pop_last();
				else
				{
					NODE *node = tail;
					tail = tail->prev;
					tail->next = NULL;
					node->prev = NULL;
					--size;
					return node;
				}
			}
			
			
			inline NODE *pop_front() throw()
			{
				assert(size>0);
                assert(head!=NULL);
                assert(tail!=NULL);
				if( size <= 1 )
					return pop_last();
				else
				{
					NODE *node = head;
					head = head->next;
					head->prev = NULL;
					node->next = NULL;
					--size;
					return node;
				}
			}
			
			inline void merge_back( list_of &other ) throw()
			{
				assert( this != &other );
				while( other.size ) push_back( other.pop_front() );
			}
			
			inline void merge_front( list_of &other ) throw()
			{
				assert( this != &other );
				while( other.size ) push_front( other.pop_back() );
			}
			
			
			inline bool owns( const NODE *node ) const throw()
			{
				for( const NODE *scan = head; scan != NULL; scan = scan->next)
				{
					if( node == scan ) return true;
				}
				return false;
			}
			
			inline void reset() throw() { head = tail = NULL; size=0; }
					
            inline void auto_delete() throw()
            {
                NODE *node = tail;
				while( node )
				{
					NODE *prev = node->prev;
					node->next = NULL;
					node->prev = NULL;
					delete node;
					node = prev;
				}
				reset();
            }
            
			inline void delete_with( void (*proc)( NODE * ) ) throw()
			{
				assert(proc);
				NODE *node = tail;
				while( node )
				{
					NODE *prev = node->prev;
					node->next = NULL;
					node->prev = NULL;
					proc(node);
					node = prev;
				}
				reset();
			}
			
			template <typename ARGS>
			inline void delete_with( void (*proc)( NODE *, ARGS), ARGS args ) throw()
			{
				assert(proc);
				NODE *node = tail;
				while( node )
				{
					NODE *prev = node->prev;
					node->next = NULL;
					node->prev = NULL;
					proc(node,args);
					node = prev;
				}
				reset();
			}
			
			template <typename ARGS1,typename ARGS2>
			inline void delete_with( void (*proc)( NODE *, ARGS1, ARGS2 ), ARGS1 args1, ARGS2 args2 ) throw()
			{
				assert(proc);
				NODE *node = tail;
				while( node )
				{
					NODE *prev = node->prev;
					node->next = NULL;
					node->prev = NULL;
					proc(node,args1,args2);
					node = prev;
				}
				reset();
			}
			
			inline NODE * unlink( NODE *node ) throw()
			{
				assert( owns(node) );
				if( head == node )
				{
					return pop_front();
				}
				else 
				{
					if( tail == node )
					{
						return pop_back();
					}
					else
					{
						assert( size > 2 );
						NODE *next = node->next;
						NODE *prev = node->prev;
						next->prev = prev;
						prev->next = next;
						node->next = NULL;
						node->prev = NULL;
						--size;
						return node;
					}
					
				}
				
			}
            
           			
			//! fetch in 0..size-1
			inline NODE *fetch( size_t index ) throw()
			{
				assert( index < size );
				if( index > (size>>1) )
				{
					NODE *node = tail;
					size_t m = size - ++index;
					while( m-- > 0 ) node=node->prev;
					return node;
				}
				else
				{
					NODE *node = head;
					while( index-- > 0 ) node=node->next;
					return node;
				}
			}
			
            inline const NODE *fetch( size_t index ) const throw()
            {
                assert( index < size );
                if( index > (size>>1) )
                {
                    const NODE *node = tail;
                    size_t m = size - ++index;
                    while( m-- > 0 ) node=node->prev;
                    return node;
                }
                else
                {
                    const NODE *node = head;
                    while( index-- > 0 ) node=node->next;
                    return node;
                }
            }

            
			inline void move_to_front( NODE *node ) throw()
			{
				assert( owns( node ) );
				push_front( unlink(node) );
			}
			
            //! replace and return mine
            inline NODE* replace( NODE *mine, NODE *yours ) throw()
            {
                assert( owns(mine) );
                assert( NULL == yours->prev );
                assert( NULL == yours->next );
                if( head == mine )
                {
                    NODE *node = pop_front();
                    push_front(yours);
                    return node;
                }
                else
                {
                    if( tail == mine )
                    {
                        NODE *node = pop_back();
                        push_back(yours);
                        return node;
                    }
                    else
                    {
                        assert( size > 2 );
						NODE *next  = mine->next;
						NODE *prev  = mine->prev;
						next->prev  = yours;
						prev->next  = yours;
                        yours->prev = prev;
                        yours->next = next;
						mine->next  = NULL;
						mine->prev  = NULL;
						return mine;
                    }
                }
            }
            
            inline size_t index_of( const NODE *node ) const throw()
            {
                assert( owns(node) );
                size_t ans = 1;
                for( const NODE *scan = head; scan!=node; scan=scan->next,++ans)
                    ;
                return ans;
            }
            
            
            inline void reverse() throw()
            {
                list_of tmp;
                while( size ) tmp.push_back( pop_back() );
                swap_with(tmp);
            }
            
					
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(list_of);
			inline void push_first( NODE *node ) throw()
			{
				assert(NULL==head); assert(NULL==tail); assert(0==size);
				head = tail = node;
				size = 1;
			}
			
			inline NODE *pop_last() throw()
			{
				assert(NULL!=head); assert(NULL!=tail); assert(1==size); assert(head==tail);
				NODE *node = head;
				assert( NULL == node->prev );
				assert( NULL == node->next );
				reset();
				return node;
			}
		};
		
        template <typename NODE>
        class list_of_cpp : public list_of<NODE>
        {
        public:
            explicit list_of_cpp() throw() : list_of<NODE>() {}
            
            inline void clear() throw()
            {
                this->auto_delete();
            }
            
            virtual ~list_of_cpp() throw()
            {
                clear();
            }

            //! valid only if a copy ctor is defined for NODE
            list_of_cpp( const list_of_cpp &other ) : list_of<NODE> ()
            {
                try
                {
                    for(const NODE *node=other.head;node;node=node->next)
                    {
                        this->push_back( new NODE( *node ) );
                    }
                }
                catch(...)
                {
                    clear();
                    throw;
                }
            }

        private:
            YOCTO_DISABLE_ASSIGN(list_of_cpp);
            
        };
		
	}
	
}

#endif
