#ifndef YOCTO_GFX_XPATCH_INCLUDED
#define YOCTO_GFX_XPATCH_INCLUDED 1

#include "yocto/gfx/types.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/threading/engine.hpp"
#include "yocto/ptr/soft.hpp"

namespace yocto
{
    namespace gfx
    {

        class                               xpatch;
        typedef vector<xpatch>             _xpatches;
        typedef soft_ptr<threading::engine> thread_server;


        //! execution patch
        class xpatch : public patch, public vslot
        {
        public:
            explicit xpatch(const patch &p);

            //! does not copy slot !
            xpatch(const xpatch &other);
            virtual ~xpatch() throw();
            faked_lock        jlk;
            threading::job_id jid;

            static
            void create( _xpatches &xp, const patch &source, threading::engine *server );

            //! enqueue a method(xpatch&,lockable&)
            template <
            typename OBJECT_POINTER,
            typename METHOD_POINTER>
            inline void enqueue(OBJECT_POINTER     host,
                                METHOD_POINTER     method,
                                threading::engine *server)
            {
                assert(host);
                assert(method);
                if(server)
                {
                    jid = server->enqueue<OBJECT_POINTER,METHOD_POINTER,xpatch>(host,method,*this);
                }
                else
                {
                    jid = 0;
                    ( (*host).*method )(*this,jlk);
                }
            }


        private:
            YOCTO_DISABLE_ASSIGN(xpatch);
        };


        class xpatches : public _xpatches
        {
        public:
            explicit xpatches() {}
            virtual ~xpatches() throw() {}

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(xpatches);
        };

        
        
    }
}

#endif
