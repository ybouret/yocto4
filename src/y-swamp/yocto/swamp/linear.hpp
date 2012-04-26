#ifndef YOCTO_SWAMP_LINEAR_INCLUDED
#define YOCTO_SWAMP_LINEAR_INCLUDED 1

#include "yocto/swamp/layout.hpp"
#include "yocto/ios/istream.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto 
{
        
    namespace swamp
    {
        
        //! linear bytes infos and ops for a layout
        /**
         No memory is allocated at this point
         */
        template <typename T, typename LAYOUT>
        class linear : public LAYOUT
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            
            const size_t bytes; //!< linear bytes
			T           *entry; //!< must be linked to an array of LAYOUT::items
			
			
			virtual ~linear() throw() {}
			
			explicit linear( const LAYOUT &L ) throw() :
			LAYOUT( L ),
			bytes( this->items * sizeof(T) ),
			entry( NULL )
			{
			}
			
			//! set every item to 0
			inline void ldz() throw() { assert(entry); memset( entry, 0, bytes); }
            
            //==================================================================
            // virtual API
            //==================================================================
            typedef void (*callback_t)( type &, void * );       //!< element wise r/w callback
            typedef void (*const_cb_t)( const_type &, void * ); //!< element wise r/o callback 
            
            //! link to data, dimension dependent
			virtual void link( T *addr ) throw() = 0;
            
            
            virtual void foreach( const LAYOUT &sub, callback_t proc, void *args ) = 0;
            virtual void foreach( const LAYOUT &sub, const_cb_t proc, void *args ) const = 0;
            
            //==================================================================
            // non virtual API
            //==================================================================
            inline void set_all( const LAYOUT &sub, param_type v) throw() { foreach( sub, set_cb, (void*)&v); }
			inline void add_all( const LAYOUT &sub, param_type v) throw() { foreach( sub, add_cb, (void*)&v); }
			
            //inline void mul_all( const LAYOUT &sub, param_type v) throw() { foreach( sub, mul_cb, (void*)&v); }
            
			inline type sum( const LAYOUT &sub ) throw() { type ans(0); foreach( sub, sum_cb, &ans); return ans; }
			
			inline void save( ios::ostream &fp, const LAYOUT &sub) const { foreach( sub, save_cb, &fp); }
			inline void load( ios::istream &fp, const LAYOUT &sub)       { foreach( sub, load_cb, &fp); }
            
        protected:
			static inline void set_cb( type &v, void *args) throw() { v  = *(type*)args; }
			static inline void add_cb( type &v, void *args) throw() { v += *(type*)args; }
			//static inline void mul_cb( type &v, void *args) throw() { v *= *(type*)args; }
			static inline void sum_cb( const_type &v, void *args) throw() { *(type *)args += v; }
			
			static inline void set2_cb( type &v, const_type &u, void *) throw() { v = u; }
			static inline void add2_cb( type &v, const_type &u, void *) throw() { v += u; }
			static inline void muladd_cb( type &v,const_type &u, void *args) throw() { v += (*(const_type *)args) * u; }
			
			static inline void save_cb( const_type &v, void *args )
			{
				ios::ostream &fp = *(ios::ostream *)args;
				fp.save( &v, sizeof(type) );
			}
			
			static inline void load_cb( type &v, void *args )
			{
				ios::istream &fp = *(ios::istream *)args;
				fp.load( &v, sizeof(type) );
			}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear);
            
        };
        
        
    }
    
}


#endif
