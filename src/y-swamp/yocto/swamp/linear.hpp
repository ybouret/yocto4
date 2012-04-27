#ifndef YOCTO_SWAMP_LINEAR_INCLUDED
#define YOCTO_SWAMP_LINEAR_INCLUDED 1

#include "yocto/swamp/layout.hpp"
#include "yocto/ios/istream.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/hashing/function.hpp"

namespace yocto 
{
    
    namespace swamp
    {
        
        class linear_base
        {
        public:
            virtual ~linear_base() throw();
            const size_t bytes;
            
            //! link to data, dimension dependent
			virtual void link( void *data ) throw() = 0;
                       
        protected:
            explicit linear_base( size_t num_bytes ) throw();
            static size_t compute_bytes( size_t items, size_t item_size ) throw();
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear_base);
        };
        
        //! linear bytes infos and ops for a layout
        /**
         No memory is allocated at this point
         */
        template <typename T, typename LAYOUT>
        class linear : public LAYOUT, public linear_base
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            
			T           *entry; //!< must be linked to an array of LAYOUT::items
			
			
			virtual ~linear() throw() {}
			
			explicit linear( const LAYOUT &L ) throw() :
			LAYOUT( L ),
            linear_base( linear_base::compute_bytes(this->items,sizeof(T)) ),
			entry( NULL )
			{
			}
			            
			//! set every item to 0
			inline void ldz() throw() { assert(entry); memset( entry, 0, bytes); }
            
            //==================================================================
            // virtual API
            //==================================================================
            typedef void (*callback)( type &, void * );           //!< element wise r/w callback
            typedef void (*const_cb)( const_type &, void * );     //!< element wise r/o callback 
            typedef void (*call_two)(type &,const type &,void *); //!< element pair-wise callback
            
            
            
            
            virtual void foreach( const LAYOUT &sub, callback proc, void *args ) = 0;
            virtual void foreach( const LAYOUT &sub, const_cb proc, void *args ) const = 0;
            
            //==================================================================
            // non virtual API
            //==================================================================
            inline void set_all( const LAYOUT &sub, param_type v) throw() { foreach( sub, set_cb, (void*)&v); }
			inline void add_all( const LAYOUT &sub, param_type v) throw() { foreach( sub, add_cb, (void*)&v); }
			
            //inline void mul_all( const LAYOUT &sub, param_type v) throw() { foreach( sub, mul_cb, (void*)&v); }
            
			inline type sum( const LAYOUT &sub ) throw() { type ans(0); foreach( sub, sum_cb, &ans); return ans; }
			
			inline void save( ios::ostream &fp, const LAYOUT &sub) const { foreach( sub, save_cb, &fp); }
			inline void load( ios::istream &fp, const LAYOUT &sub)       { foreach( sub, load_cb, &fp); }
            inline void hash( hashing::function &fn, const LAYOUT &sub ) const { fn.set(); foreach(sub, hash_cb, &fn); }
            
        protected:
			static inline void set_cb( type &v, void *args) throw() { v  = *(type*)args; }
			static inline void add_cb( type &v, void *args) throw() { v += *(type*)args; }
			//static inline void mul_cb( type &v, void *args) throw() { v *= *(type*)args; }
			static inline void sum_cb( const_type &v, void *args) throw() { *(type *)args += v; }
			
			static inline void set2_cb( type &v, const_type &u, void *) throw() { v = u; }
			static inline void add2_cb( type &v, const_type &u, void *) throw() { v += u; }
			//static inline void muladd_cb( type &v,const_type &u, void *args) throw() { v += (*(const_type *)args) * u; }
			
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
            
            static inline void hash_cb( const_type &v, void *args )
            {
                hashing::function &fn = *(hashing::function *)args;
                fn.run( &v, sizeof(type) );
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear);
            
        };
        
        
    }
    
}


#endif
