#ifndef YOCTO_GFX_XPATCH_INCLUDED
#define YOCTO_GFX_XPATCH_INCLUDED 1

#include "yocto/gfx/types.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/threading/engine.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace gfx
    {

        typedef arc_ptr<threading::dispatcher> thread_server;


        //! execution patch
        class xpatch : public patch, public vslot
        {
        public:
            explicit xpatch(const patch &p);


            xpatch(const xpatch &other); //!< doesn't copy vslot nor jid
            virtual ~xpatch() throw();   //!< default destructor
            threading::job_id jid;       //!< last job ID

            
            //! enqueue a method(xpatch&,lockable&)
            template <
            typename OBJECT_POINTER,
            typename METHOD_POINTER>
            inline void enqueue(OBJECT_POINTER     host,
                                METHOD_POINTER     method,
                                thread_server     &server)
            {
                assert(host);
                assert(method);
                jid = server->enqueue2<OBJECT_POINTER,METHOD_POINTER,xpatch>(host,method,*this);
            }


        private:
            YOCTO_DISABLE_ASSIGN(xpatch);
        };


        //! base class for xpatches
        typedef vector<xpatch>  _xpatches;

        //! a vector of xpatch and with the thread dispatcher
        class xpatches : public _xpatches
        {
        public:
            //! default constructor
            explicit xpatches(const patch &source, threading::dispatcher *srv);

            //! make one patch (sequential)
            explicit xpatches(const patch &source);

            //! environment based layout
            explicit xpatches(const patch &source, const bool setVerbose);

            //! destructor
            virtual ~xpatches() throw();

            thread_server server;

            //! enqueue all and flush
            template <typename HOST_POINTER,typename METHOD_POINTER>
            void submit(HOST_POINTER   host,
                        METHOD_POINTER method)
            {
                _xpatches &self = *this;
                for(size_t i=self.size();i>0;--i)
                {
                    self[i].enqueue<HOST_POINTER,METHOD_POINTER>(host,method,server);
                }
                server->flush();
            }

            void setup_from(const patch &source);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(xpatches);
        };

#define YGFX_SUBMIT(HOST,METHOD,XPS,CODE) do        {\
for(register size_t ipatch=XPS.size();ipatch>0;--i) {\
xpatch &xp = XPS[ipatch]; do { CODE; } while(false)  \
xp.enqueue(HOST,METHOD,XPS.server);                 }\
xps.server->flush(); } while(false)
        
        
    }
}

#endif
