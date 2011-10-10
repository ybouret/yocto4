#ifndef YOCTO_ASSOCIATIVE_SET_INCLUDED
#define YOCTO_ASSOCIATIVE_SET_INCLUDED 1

#include "yocto/container/container.hpp"
#include "yocto/associative/key-hasher.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/associative/ktable.hpp"

#include "yocto/container/iter-linked.hpp"


namespace yocto
{
	namespace hidden
	{
		extern const char set_name[];
	}
	
	
	template <
	typename KEY,
	typename T, 
	typename KEY_HASHER = key_hasher<KEY>,
	typename ALLOCATOR  = memory::global::allocator >
	class set : public container
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
			type         data;
			inline ~node_type() throw() {}
			inline  node_type( size_t k, param_type args ) : next(NULL), prev(NULL), hkey(k), data(args) {}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(node_type);
		};
		
		//! default nothrow constructor
		explicit set() throw(): hash_(), hmem_(),ktab_() {}
		
		//! with a little memory
		explicit set( size_t n, const as_capacity_t & ) : hash_(), hmem_(), ktab_(n,hmem_) {}
		
		//! destructor
		virtual ~set() throw() { _kill(); }
		
		//! copy constructor
		inline set( const set &other ) : hash_(), hmem_(), ktab_() 
		{
			set tmp( other.size(), as_capacity );
			other._copy_into( tmp );
			mswap( tmp.ktab_, ktab_ );
		}
		
		//======================================================================
		// container interface
		//======================================================================
		virtual const char *name() const throw()     { return hidden::set_name; }
		virtual size_t      size() const throw()     { return ktab_.nlist.size; }
		virtual size_t      capacity() const throw() { return ktab_.nodes;      }
		virtual void        free() throw()    { _free(); }
		virtual void        release() throw() { _kill(); }
		virtual void        reserve( size_t n )
		{
			if( n > 0 )
			{
				set other( this->capacity() + n, as_capacity );
				_copy_into( other );
				mswap( ktab_, other.ktab_ );
			}
		}
		
		//======================================================================
		// specific interface
		//======================================================================
		virtual bool insert( param_type args )
		{
			const_key       &key  = args.key();
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
					_insert2( ktab_, hkey, args, slot );
				}
				else
				{
					set  other( next_capacity( capacity() ), as_capacity);
					_copy_into(other);
					_insert( other.ktab_,  hkey, args );
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
				assert( node == slot->head ->addr); assert( node->data.key() == key );
				ktab_.remove_front_of(slot, destruct<node_type> );
				return true;
			}
			else 
				return false;
		}
		
		inline const_type *search( param_key key ) const throw()
		{
			return lookup(key);
		}
		
		inline type *search( param_key key ) throw()
		{
			return (type *)lookup(key);
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
		YOCTO_DISABLE_ASSIGN(set);
		
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
		
		//! detroy content, keep nodes
		inline void _free() throw() { ktab_.free_with( destruct<node_type> ); }
		
		//! release all
		inline void _kill() throw() { _free();  ktab_.release_all(hmem_);     }
		
		inline void _copy_into( set &other ) const
		{
			assert( other.capacity() >= this->size() );
			assert( other.size() == 0 );
			for( node_type *src = ktab_.nlist.head; src; src = src->next )
			{
				//-- dup/insert
				_insert( other.ktab_, src->hkey, src->data );
			}
			assert( other.size() == this->size() );
			assert( other.ktab_.signature() == this->ktab_.signature() );
		}
		
		static inline node_type *_create( ktable_t &tab, size_t hkey, param_type args )
		{
			//-- atomic node creationg
			node_type       *tgt = tab.cache.query();
			try{ new (tgt) node_type(hkey,args); }
			catch(...) { tab.cache.store(tgt); throw; }
			return tgt;
		}
		
		static inline void _insert( ktable_t &tab, size_t hkey, param_type args )
		{ tab.insert( _create(tab,hkey,args) ); }
		
		static inline void _insert2( ktable_t &tab, size_t hkey, param_type args, kslot_t *slot ) throw()
		{ tab.insert2( _create(tab,hkey,args),slot); }
		
		static inline bool _match( const node_type *node, const void *params ) throw()
		{ return node->data.key() == *(const_key*)params; }
		
		
	};
	
}

#endif
