#ifndef YOCTO_THREADING_THREADS_INCLUDED
#define YOCTO_THREADING_THREADS_INCLUDED 1

#include "yocto/threading/thread.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/core/list.hpp"
#include "yocto/auto-clean.hpp"

namespace yocto
{
    
	namespace threading
	{
        
		//! low level memory for threads
		class threads : public core::list_of<thread>
		{
		public:
			mutex access;
            
			//! construct and give name to access
			explicit threads(const char *id) throw();
            
			//! construct, give name to access and reserve(n)
			explicit threads(const char *id, size_t n) throw();
            
			//! finish and clean all
			virtual ~threads() throw();
            
			//! launch a generic procedure
			void launch( thread::procedure proc, void *data);
            
			//! finish a specific thread, removed from the list
			void finish( thread *thr ) throw();
            
			//! finish all
			void finish() throw();
            
			//! release pool
			void trim() throw();
            
			//! prepare some memory
			void reserve(size_t n);
            
            //! auto cleanup
			class failsafe : public auto_clean<threads>
			{
			public:
				explicit failsafe( threads &host ) throw();
				virtual ~failsafe() throw();
                
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(failsafe);
			};
            
            
            
			template <typename FUNCTION>
			void start( const FUNCTION &fn )
			{
				YOCTO_LOCK(access);
				thread *thr = query();
				try
				{
					thr->code.build< exec0<FUNCTION>, FUNCTION>( fn );
					launch( execute0<FUNCTION>, & (thr->code.as< exec0<FUNCTION> >()) );
					push_back(thr);
				}
				catch(...) { pool.store(thr); throw; }
			}
            
			template <typename FUNCTION, typename T>
			void start( const FUNCTION &function, typename type_traits<T>::parameter_type param1 )
			{
				YOCTO_LOCK(access);
				thread *thr = query();
				try
				{
					thr->code.build< exec1<FUNCTION,T>, FUNCTION, T>( function, param1);
					launch( execute1<FUNCTION,T>, &( thr->code.as< exec1<FUNCTION,T> >() ));
					push_back(thr);
				}
				catch(...)  { pool.store(thr); throw; }
			}
            
		private:
			thread *query();
			core::pool_of<thread> pool;
			YOCTO_DISABLE_COPY_AND_ASSIGN(threads);
            
            //__________________________________________________________________
            //
            // Functionoid
            //__________________________________________________________________
			template <typename FUNCTION>
			class exec0
			{
			public:
				inline  exec0( const FUNCTION &function ) : fn_(function) {}
				inline ~exec0() throw() {}
                
				inline void run() { fn_(); }
                
			private:
				FUNCTION fn_;
				YOCTO_DISABLE_COPY_AND_ASSIGN(exec0);
			};
            
            
			template <typename FUNCTION>
			static inline void execute0( void *args ) throw()
			{
				try
				{
					assert(args);
					static_cast< exec0<FUNCTION> *>(args)->run();
				}
				catch(...)
				{
                    
				}
			}
            
            //__________________________________________________________________
            //
            // Functionoid + argument
            //__________________________________________________________________
			template <typename FUNCTION, typename T>
			class exec1
			{
			public:
				inline  exec1(const FUNCTION                         &function,
                              typename type_traits<T>::parameter_type param1 ) :
                fn_(function), p1_(param1) {}
				inline ~exec1() throw() {}
                
				inline void run() { fn_(p1_); }
                
			private:
				FUNCTION                              fn_;
				typename type_traits<T>::mutable_type p1_;
				YOCTO_DISABLE_COPY_AND_ASSIGN(exec1);
			};
            
			template <typename FUNCTION, typename T>
			static inline void execute1( void *args ) throw()
			{
				try
				{
					assert(args);
					static_cast< exec1<FUNCTION,T> *>(args)->run();
				}
				catch(...)
				{
                    
				}
			}
		};
        
	}
    
}


#endif
