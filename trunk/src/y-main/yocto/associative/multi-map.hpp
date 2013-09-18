#ifndef YOCTO_ASSOC_MULTI_MAP_INCLUDED
#define YOCTO_ASSOC_MULTI_MAP_INCLUDED

#include "yocto/associative/set.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/intr-ptr.hpp"
#include "yocto/counted.hpp"

namespace yocto
{
    namespace hidden
    {
        extern const char multi_map_name[];
    }
    
    template <
    typename KEY,
    typename T,
    typename KEY_HASHER = key_hasher<KEY>,
    typename ALLOCATOR  = memory::global::allocator >
    class multi_map
    {
    public:
        YOCTO_ASSOCIATIVE_KEY_T;
        
        class DNode
        {
        public:
            DNode  *prev;
            DNode  *next;
            type    data;
            inline  DNode( param_type args ) : prev(0), next(0), data(args) {}
            inline ~DNode() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(DNode);
        };
        
        typedef core::list_of<DNode> DList;
        typedef core::pool_of<DNode> DPool;
        
        class GNode
        {
        public:
            GNode *prev;
            GNode *next;
            DNode *dnode;
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(GNode);
            GNode();
            ~GNode();
        };
        typedef core::pool_of<GNode> GPool;
        
        class  Group :
        public object,
        public counted,
        public core::list_of<GNode>
        {
        public:
            typedef intr_ptr<KEY,Group>                   Pointer;
            typedef set<KEY,Pointer,KEY_HASHER,ALLOCATOR> DataBase;
            const_key gkey;
            
            explicit Group( param_key k ) :
            gkey(k)
            {
            }
            
            virtual ~Group() throw()
            {
            }
            
            inline const_key &key() const throw() { return gkey; }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Group);
        };
        
        typedef typename Group::Pointer  GroupPointer;
        typedef typename Group::DataBase GroupDB;
        
        
        inline void insert( param_key key, param_type args )
        {
            //------------------------------------------------------------------
            // Find the holding group, create if necessary
            //------------------------------------------------------------------
            Group        *G   = 0;
            GroupPointer *ppG = groups.search(key);
            if(!ppG)
            {
                G = new Group(key);
                const GroupPointer tmp(G);
                groups.insert__(tmp);
            }
            else
            {
                G = (*ppG).__get();
            }
            assert(G!=0);
            
            //------------------------------------------------------------------
            // Create a Data Node
            //------------------------------------------------------------------
            DNode *dnode = dpool.size ? dpool.query() : object::acquire1<DNode>();
            try{ new (dnode) DNode(args); }catch(...){ dpool.store(dnode); }
            
            //------------------------------------------------------------------
            // Create a Group Node
            //------------------------------------------------------------------
            try
            {
                GNode *gnode = gpool.size ? gpool.query() : object::acquire1<GNode>();
                gnode->dnode = dnode;
                G->push_front(gnode);
            }
            catch(...)
            {
                __destroy(dnode);
            }
            
            dlist.push_back(dnode);
        }
        
        explicit multi_map() throw() :
        dlist(),
        dpool(),
        gpool(),
        groups()
        {
        }
        
        virtual ~multi_map() throw() { __release(); }
        
        virtual void free() throw()    { __free();    }
        virtual void release() throw() { __release(); }
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(multi_map);
        DList                    dlist;
        DPool                    dpool;
        GPool                    gpool;
        typename Group::DataBase groups;
        
        //----------------------------------------------------------------------
        // data node destruct
        //----------------------------------------------------------------------
        inline void __destroy( DNode *dnode ) throw()
        { assert(dnode); destruct(dnode); dpool.store(dnode); }
        
        //----------------------------------------------------------------------
        // free
        //----------------------------------------------------------------------
        inline void __free() throw()
        {
            for( typename GroupDB::iterator i = groups.begin(); i != groups.end(); ++i)
            {
                Group &G = **i;
                while( G.size > 0 )
                    gpool.store( G.pop_front() );
            }
            groups.free();
            while(dlist.size) __destroy(dlist.pop_back());
        }
        
        //----------------------------------------------------------------------
        // release
        //----------------------------------------------------------------------
        inline void __release() throw()
        {
            __free();
            groups.release();
            while(dpool.size) object::release1( dpool.query() );
            while(gpool.size) object::release1( gpool.query() );
        }
    };
}

#endif
