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
        
        //! forward.
        class SIMD;
        
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
			inline void start( FUNCTION &fn )
			{
				YOCTO_LOCK(access);
				thread *thr = query();
				try
				{
                    vslot &code = thr->check_code();
                    const exec0<FUNCTION> todo(fn);
					code.make< exec0<FUNCTION> >( todo );
                    thread::procedure proc = exec0<FUNCTION>::run;
                    void             *data = & (code.as< exec0<FUNCTION> >());
					thr->launch(proc,data);
					push_back(thr);
				}
				catch(...) { pool.store(thr); throw; }
			}
            
            //! the functionoid and the parameter must be valid during  the whole thread
			template <typename FUNCTION, typename T>
			inline void start(FUNCTION &function,
                              T        &param1 )
			{
				YOCTO_LOCK(access);
				thread *thr = query();
				try
				{
                    vslot &code = thr->check_code();
                    const exec1<FUNCTION,T> todo(function,param1);
					code.make< exec1<FUNCTION,T> >( todo );
                    thread::procedure proc = exec1<FUNCTION,T>::run;
                    void             *data = &( code.as< exec1<FUNCTION,T> >() );
					thr->launch(proc,data);
					push_back(thr);
				}
				catch(...)  { pool.store(thr); throw; }
			}
            
            template <typename OBJECT_POINTER,typename METHOD_POINTER>
            inline void call(OBJECT_POINTER host, METHOD_POINTER meth)
            {
                YOCTO_LOCK(access);
				thread *thr = query();
				try
				{
                    vslot &code = thr->check_code();
                    const call0<OBJECT_POINTER,METHOD_POINTER> todo(host,meth);
                    code.make< call0<OBJECT_POINTER,METHOD_POINTER> >(todo);
                    thread::procedure proc = call0<OBJECT_POINTER,METHOD_POINTER>::run;
                    void             *data = &(code.as< call0<OBJECT_POINTER,METHOD_POINTER> >() );
                    thr->launch(proc,data);
					push_back(thr);
				}
				catch(...)  { pool.store(thr); throw; }
                
            }

            int get_index_of(const thread::handle_t) throw();

		private:
			thread *query();
			core::pool_of<thread> pool;
			YOCTO_DISABLE_COPY_AND_ASSIGN(threads);
            friend class SIMD;
            
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
                
                static inline void run(void *args) throw()
                {
                    try
                    {
                        assert(args);
                        static_cast< exec0<FUNCTION> *>(args)->fn_();
                    }
                    catch(...)
                    {
                        
                    }
                }
			private:
				FUNCTION &fn_;
				YOCTO_DISABLE_ASSIGN(exec0);
			};
            
            
            
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
                
                static inline void run( void *args ) throw()
                {
                    try
                    {
                        assert(args);
                        exec1 &self = * static_cast<exec1<FUNCTION,T>*>(args);
                        self.fn_( self.p1_ );
                    }
                    catch(...)
                    {
                        
                    }
                }
                
			private:
				FUNCTION &fn_;
				T        &p1_;
				YOCTO_DISABLE_ASSIGN(exec1);
			};
            
			
            
            //__________________________________________________________________
            //
            // Object+Method
            //__________________________________________________________________
            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            class call0
            {
            public:
                inline call0(OBJECT_POINTER pObj, METHOD_POINTER addr) throw() :
                host(pObj), meth(addr) {}
                inline call0( const call0 &other ) throw() :
                host(other.host), meth(other.meth) {}
                inline ~call0() throw() {}
                
                static inline void run( void *args )
                {
                    try
                    {
                        assert(args);
                        call0 &self = *static_cast<call0*>(args);
                        ( (*self.host).*(self.meth) )();
                    }
                    catch(...)
                    {
                    }
                }
            private:
                YOCTO_DISABLE_ASSIGN(call0);
                OBJECT_POINTER host;
                METHOD_POINTER meth;
            };
            
		};
        
	}
    
}


#endif
