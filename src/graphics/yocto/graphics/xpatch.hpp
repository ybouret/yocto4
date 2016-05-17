#ifndef YOCTO_GRAPHICS_XPATCH_INCLUDED
#define YOCTO_GRAPHICS_XPATCH_INCLUDED 1

#include "yocto/graphics/types.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/threading/engine.hpp"

namespace yocto
{
    namespace graphics
    {

        class                  xpatch;
        typedef vector<xpatch> xpatches;

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
            void create( xpatches &xp, const patch &source, threading::engine *server );

            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            inline void enqueue(OBJECT_POINTER    host,
                                METHOD_POINTER    method,
                                threading::engine &server)
            {
                assert(host);
                assert(method);
                jid = server.enqueue<OBJECT_POINTER,METHOD_POINTER,xpatch>(host,method,*this);
            }


            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            inline void execute(OBJECT_POINTER    host,
                                METHOD_POINTER    method)
            {
                assert(host);
                assert(method);
                jid = 0;
                ( (*host).*method )(*this,jlk);
            }



        private:
            YOCTO_DISABLE_ASSIGN(xpatch);
        };
        
        
        
    }
}

#endif
