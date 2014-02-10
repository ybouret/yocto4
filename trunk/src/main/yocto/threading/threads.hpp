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
            
            
            //! the functionoid must be valid during all thread execution
			template <typename FUNCTION>
			void start( FUNCTION &fn )
			{
				YOCTO_LOCK(access);
				thread *thr = query();
				try
				{
                    const exec0<FUNCTION> todo(fn);
					thr->code.make< exec0<FUNCTION> >( todo );
					launch( execute0<FUNCTION>, & (thr->code.as< exec0<FUNCTION> >()) );
					push_back(thr);
				}
				catch(...) { pool.store(thr); throw; }
			}
            
            //! the functionoid and the parameter must be valid during  the whole thread
			template <typename FUNCTION, typename T>
			void start(FUNCTION &function,
                       T        &param1 )
			{
				YOCTO_LOCK(access);
				thread *thr = query();
				try
				{
                    const exec1<FUNCTION,T> todo(function,param1);
					thr->code.make< exec1<FUNCTION,T> >( todo );
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
				inline  exec0( FUNCTION &function ) throw(): fn_(function)  {}
                inline  exec0( const exec0 &other ) throw(): fn_(other.fn_) {}
				inline ~exec0() throw() {}
                
				inline void run() { fn_(); }
                
			private:
				FUNCTION &fn_;
				YOCTO_DISABLE_ASSIGN(exec0);
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
				inline  exec1(FUNCTION &function,
                              T        &param1 ) throw() :
                fn_(function), p1_(param1) {}
                inline  exec1(const exec1 &other ) throw() :
                fn_(other.fn_), p1_(other.p1_) {}
                
				inline ~exec1() throw() {}
                
				inline void run() { fn_(p1_); }
                
			private:
				FUNCTION &fn_;
				T        &p1_;
				YOCTO_DISABLE_ASSIGN(exec1);
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
