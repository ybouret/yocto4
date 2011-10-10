#ifndef YOCTO_ASSOCIATIVE_MAP_INCLUDED
#define YOCTO_ASSOCIATIVE_MAP_INCLUDED 1

#include "yocto/container/associative.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/associative/ktable.hpp"

#include "yocto/container/iter-linked.hpp"

namespace yocto
{
	
	namespace hidden
	{
		extern const char map_name[];
	}
	
	template <
	typename KEY,
	typename T, 
	typename KEY_HASHER = key_hasher<KEY>,
	typename ALLOCATOR  = memory::global::allocator >
	class map : public associative<KEY,T>
	{
	public:
		YOCTO_ASSOCIATIVE_KEY_T;
		
		//! data key/obj node
		class node_type
		{
		public:
			node_type   *next;
			node_type   *prev;
			const size_t hkey; 
			const_key    key;
			type         data;
			inline ~node_type() throw() {}
			inline  node_type( size_t h, param_key k,  param_type args ) : next(NULL), prev(NULL), hkey(h), key(k), data(args) {}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
		};
		
		
		
		
		explicit map() throw(): hash_(), hmem_(),ktab_() {}
		explicit map( size_t n, const as_capacity_t & ) : hash_(), hmem_(), ktab_(n,hmem_) {}
		virtual ~map() throw() { _kill(); }
		
		inline map( const map &other ) : hash_(), hmem_(), ktab_() 
		{
			map tmp( other.size(), as_capacity );
			other._copy_into( tmp );
			mswap( tmp.ktab_, ktab_ );
		}
		
		//======================================================================
		// container interface
		//======================================================================
		virtual const char *name() const throw()     { return hidden::map_name; }
		virtual size_t      size() const throw()     { return ktab_.nlist.size; }
		virtual size_t      capacity() const throw() { return ktab_.nodes;      }
		virtual void        free() throw()    { _free(); }
		virtual void        release() throw() { _kill(); }
		virtual void        reserve( size_t n )
		{
			if( n > 0 )
			{
				map other( this->capacity() + n, as_capacity );
				_copy_into( other );
				mswap( ktab_, other.ktab_ );
			}
		}
		
		//======================================================================
		// associative interface
		//======================================================================
		virtual bool insert( param_key key, param_type args )
		{
			const size_t     hkey = hash_(key);
			kslot_t         *slot = NULL;
			const node_type *node = ktab_.search( hkey, slot, _match, &key );
			if( node )
			{
				return false;
			}
			else 
			{
				if( size() < capacity() )
				{
					assert(slot!=NULL);
					_insert2( ktab_, hkey, key, args, slot );
				}
				else
				{
					map  other( container::next_capacity( capacity() ), as_capacity);
					_copy_into(other);
					_insert( other.ktab_,  hkey, key, args );
					mswap( other.ktab_, ktab_ );
				}
				return true;
			}
		}
		
		virtual bool remove( param_key key ) throw()
		{
			const size_t     hkey = hash_(key);
			kslot_t         *slot = NULL;
			const node_type *node = ktab_.search( hkey, slot, _match, &key );
			if( node )
			{
				assert( slot != NULL); assert( slot->head );
				assert( node == slot->head ->addr); assert( node->key == key );
				ktab_.remove_front_of(slot, destruct<node_type> );
				return true;
			}
			else 
				return false;
		}
		
		//======================================================================
		// iterators
		//======================================================================
		typedef iterating::linked<type,node_type,iterating::forward> iterator;
		inline iterator begin() throw() { return iterator( ktab_.nlist.head ); }
		inline iterator end()   throw() { return iterator( NULL );       }
		
		typedef iterating::linked<const_type,const node_type,iterating::forward> const_iterator;
		inline const_iterator begin() const throw() { return iterator( ktab_.nlist.head  ); }
		inline const_iterator end()   const throw() { return iterator( NULL );       }
		
	private:
		YOCTO_DISABLE_ASSIGN(map);
		
		typedef core::ktable<node_type>    ktable_t;
		typedef typename ktable_t::kslot_t kslot_t;
		typedef typename ktable_t::knode_t knode_t;
		mutable KEY_HASHER hash_;
		ALLOCATOR          hmem_;
		mutable ktable_t   ktab_;
		
		virtual const_type *lookup( param_key key ) const throw()
		{
			const size_t     hkey = hash_(key);
			kslot_t         *slot = NULL;
			const node_type *node = ktab_.search( hkey, slot, _match, &key );
			return node ?  & node->data : NULL;
		}
		
		inline void _free() throw() { ktab_.free_with( destruct<node_type> ); }
		inline void _kill() throw() { _free();  ktab_.release_all(hmem_);     }
		inline void _copy_into( map &other ) const
		{
			assert( other.capacity() >= this->size() );
			assert( other.size() == 0 );
			for( const node_type *src = ktab_.nlist.head; src; src = src->next )
			{
				//-- dup/insert
				_insert( other.ktab_, src->hkey, src->key, src->data );
			}
			assert( other.size() == this->size() );
			assert( other.ktab_.signature() == this->ktab_.signature() );
		}
		
		static inline node_type *_create( ktable_t &tab, size_t hkey, param_key key, param_type args )
		{
			//-- atomic node creationg
			node_type       *tgt = tab.cache.query();
			try{ new (tgt) node_type(hkey,key,args); }
			catch(...) { tab.cache.store(tgt); throw; }
			return tgt;
		}
		
		static inline void _insert( ktable_t &tab, size_t hkey, param_key key, param_type args )
		{ tab.insert( _create(tab,hkey,key,args) ); }
		
		static inline void _insert2( ktable_t &tab, size_t hkey, param_key key, param_type args, kslot_t *slot ) throw()
		{ tab.insert2(_create(tab,hkey,key,args),slot); }
		
		
		static inline bool _match( const node_type *node, const void *params ) throw()
		{ return node->key == *(const_key*)params; }
		
	};
	
}

#endif
