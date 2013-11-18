#ifndef YOCTO_SPADE_LINEAR_INCLUDED
#define YOCTO_SPADE_LINEAR_INCLUDED 1


#include "yocto/spade/types.hpp"

namespace yocto
{
    
    namespace spade
    {
        //! linear memory interface
        class linear : public object
        {
        public:
            const size_t bytes; //! #bytes once allocated
            virtual ~linear() throw();
            
            //! link to data, dimension dependent
			virtual void link( void *data ) throw() = 0;
            
            //! item size
			virtual size_t item_size() const throw() = 0;
            
            //! return the address @offset=source
            virtual const void *address_of( size_t source ) const throw() = 0;
            
            //! copy object@offset=source in object@offset=target
            virtual void local_copy( size_t target, size_t source ) throw() = 0;
            
            //! copy entry[source] into ptr
			virtual void async_store( uint8_t * &ptr, size_t source ) const throw()     = 0;
            
			//! copy ptr intro entry[source]
			virtual void async_query( const uint8_t * &ptr, size_t source ) throw()     = 0;
            
        protected:
            explicit linear( size_t num_bytes ) throw(); //!< num_bytes > 0
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear);
        };
        
        
        template <typename T,typename LAYOUT>
        class linear_of :  public linear, public LAYOUT
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef LAYOUT layout_type;
			T             *entry; //!< must be linked to an array of LAYOUT::items
            
            //! default dtor
			virtual ~linear_of() throw() {}
            
            //! default ctor
			explicit linear_of( const LAYOUT &L ) throw() :
            linear( L.items*sizeof(T) ),
            LAYOUT( L ),
            entry(0)
			{
			}
            
			//! set every item to 0
			inline void ldz() throw() { assert(entry); memset( entry, 0, bytes); }
            
            //! set every item to value
            inline void ld( param_type value )
            {
                assert(entry); for(size_t i=0;i<this->items;++i) entry[i] = value;
            }
            
            //! virtual item_size()
            virtual size_t item_size() const throw() { return sizeof(T); }
            
            //! virtual address_of
            /**
             \param source offset of the item to get
             */
            virtual const void *address_of( size_t source ) const throw() { assert(entry); assert(source<this->items); return &entry[source]; }
            
            //! virtual local_copy, assuming an assign operator exists
            /**
             \param target offset of the target item
             \param source offset of the source item
             */
            virtual void local_copy( size_t target, size_t source ) throw()
            {
                assert(entry);
                assert(target<this->items);
                assert(source<this->items);
                entry[target] = entry[source];
            }
            
            //! virtual entry[source] into ptr
			virtual void async_store( uint8_t * &ptr, size_t source ) const throw()
            {
                assert(ptr);assert(entry);assert(source<this->items);
                const uint8_t *src = (const uint8_t *) &entry[source];
                for(size_t i=sizeof(T);i>0;--i) *(ptr++) = *(src++);
            }
            
            //! copy ptr intro entry[source]
			virtual void async_query( const uint8_t * &ptr, size_t source ) throw()
            {
                assert(ptr);assert(entry);assert(source<this->items);
                uint8_t *tgt = (uint8_t *) &entry[source];
                for(size_t i=sizeof(T);i>0;--i) *(tgt++) = *(ptr++);
            }
            
            //! linear get min
            inline T get_min() const
            {
                assert(entry);
                T ans( entry[0] );
                for( size_t i=1; i < this->items;++i)
                {
                    const T tmp( entry[i] );
                    if( tmp < ans ) ans = tmp;
                }
                return ans;
            }
            
            //! linear get max
            inline T get_max() const
            {
                assert(entry);
                T ans( entry[0] );
                for( size_t i=1; i < this->items;++i)
                {
                    const T tmp( entry[i] );
                    if( tmp > ans ) ans = tmp;
                }
                return ans;
            }

            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear_of);
        };
        
    }
    
}

#endif
