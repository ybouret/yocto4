#ifndef YOCTO_SPADE_DATASPACE_INCLUDED
#define YOCTO_SPADE_DATASPACE_INCLUDED 1

#include "yocto/spade/fields/setup.hpp"
#include "yocto/spade/ghosts/setup.hpp"
#include "yocto/spade/ghosts/local.hpp"
#include "yocto/spade/ghosts/async.hpp"

namespace yocto
{
    namespace spade
    {
        
        //! dataspace: original layout + ghosts
        template <typename LAYOUT>
        class dataspace : public array_db, public LAYOUT
        {
        public:
            typedef LAYOUT                layout_type;
            typedef fields_setup<LAYOUT>  fields_list;
            
            
            const LAYOUT   outline;          //!< this layout + ghosts
            linear_handles handles;          //!< from fields
            const size_t   local_count;      //!< #local ghosts
            const size_t   async_count;      //!< #async ghosts
            const size_t   num_requests;     //!< #MPI request for data exchange
            
            virtual ~dataspace() throw() {}
            
            explicit dataspace(const LAYOUT       &L,
                               const fields_list  &F,
                               const ghosts_setup &G) :
            array_db( F.size() + LAYOUT::DIMENSIONS ),
            LAYOUT(L),
            outline(L),
            handles( F.size() ),
            local_count(0),
            async_count(0),
            num_requests(0),
            local_reg(),
            async_reg()
            {
                //--------------------------------------------------------------
                // recompute outline from ghosts
                //--------------------------------------------------------------
                recompute_outline(G);
                
                //--------------------------------------------------------------
                // create ghosts from outline+layout
                //--------------------------------------------------------------
                build_ghosts(G);
                
                //--------------------------------------------------------------
                // create fields from outline
                //--------------------------------------------------------------
                F.create( outline, *this, handles );
                
                //--------------------------------------------------------------
                // and prepare ghosts memory
                //--------------------------------------------------------------
                for( size_t i=1;i<=async_count;++i)
                {
                    async_reg[i]->allocate_for( handles );
                }
            }
            
            inline local_ghosts & get_local( size_t index ) throw()
            {
                assert(index>0);assert(index<=local_count);
                return *local_reg[index];
            }
            
            inline async_ghosts & get_async(size_t index) throw()
            {
                assert(index>0);assert(index<=async_count);
                return *async_reg[index];
                
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(dataspace);
            vector<local_ghosts::ptr> local_reg;
            vector<async_ghosts::ptr> async_reg;
            
            //==================================================================
            // recompute outline from ghosts
            //==================================================================
            void recompute_outline( const ghosts_setup &G )
            {
                
                std::cerr << "recomputing outline" << std::endl;
                typename LAYOUT::coord out_lower = outline.lower;
                typename LAYOUT::coord out_upper = outline.upper;
                size_t &num_local = (size_t&)local_count;
                size_t &num_async = (size_t&)async_count;
                
                for( size_t dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
                {
                    //----------------------------------------------------------
                    //-- test for local ghosts (PBC)
                    //----------------------------------------------------------
                    {
                        const ghosts_info *g = G.get_local() + dim;
                        if( g->active && g->count>0 )
                        {
                            ghost::check(dim, __coord(this->width,dim), g->count);
                            __coord(out_lower,dim) -= g->count;
                            __coord(out_upper,dim) += g->count;
                            ++num_local;
                        }
                    }
                    
                    //----------------------------------------------------------
                    //-- test for lower async ghost
                    //----------------------------------------------------------
                    const size_t at_lower = dim << 1;
                    {
                        const ghosts_info *g = G.get_async() + at_lower;
                        if( g->active && g->count>0)
                        {
                            ghost::check(dim, __coord(this->width,dim), g->count);
                            __coord(out_lower,dim) -= g->count;
                            ++num_async;
                        }
                    }
                    
                    //----------------------------------------------------------
                    //-- test for upper async ghost
                    //----------------------------------------------------------
                    const size_t at_upper = at_lower + 1;
                    {
                        const ghosts_info *g = G.get_async() + at_upper;
                        if( g->active && g->count>0)
                        {
                            ghost::check(dim, __coord(this->width,dim), g->count);
                            __coord(out_upper,dim) += g->count;
                            ++num_async;
                        }
                    }
                    
                }
                //--------------------------------------------------------------
                //-- recompute outline
                //--------------------------------------------------------------
                new ((void*)&outline) LAYOUT(out_lower,out_upper);
                std::cerr << "layout: " << this->as_layout() << std::endl;
                std::cerr << "outline:" << outline           << std::endl;
                
                //--------------------------------------------------------------
                //-- reserve memory
                //--------------------------------------------------------------
                local_reg.reserve( num_local );
                async_reg.reserve( num_async );
                
                //----------------------------------------------------------
                //-- compute num_requests
                //----------------------------------------------------------
                (size_t&) num_requests = 2 * num_async;
                
                
            }
            
            //==================================================================
            // build the ghosts from the outline and the setup
            //==================================================================
            void build_ghosts(const ghosts_setup &G)
            {
                //const LAYOUT &L = this->as_layout();
                for( size_t dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
                {
                    //----------------------------------------------------------
                    //-- create local ghosts
                    //----------------------------------------------------------
                    {
                        const ghosts_info *g = G.get_local() + dim;
                        if( g->active && g->count>0 )
                        {
                            local_ghosts           *lg = new local_ghosts( dimension_t(dim) );
                            const local_ghosts::ptr lp( lg );
                            
                            lg->setup(g->count, outline);
                            local_reg.push_back(lp);
                        }
                    }
                    
                    //----------------------------------------------------------
                    //-- create lower async ghost
                    //----------------------------------------------------------
                    const size_t at_lower = dim << 1;
                    {
                        const ghosts_info *g = G.get_async() + at_lower;
                        if( g->active && g->count>0)
                        {
                            async_ghosts           *ag = new async_ghosts( ghost::position(at_lower) );
                            const async_ghosts::ptr ap(ag);
                            
                            ag->peer = g->peer;
                            ag->setup(g->count,outline);
                            async_reg.push_back(ap);
                        }
                    }
                    
                    //----------------------------------------------------------
                    //-- create lower async ghost
                    //----------------------------------------------------------
                    const size_t at_upper = at_lower+1;
                    {
                        const ghosts_info *g = G.get_async() + at_upper;
                        if( g->active && g->count>0)
                        {
                            async_ghosts           *ag = new async_ghosts( ghost::position(at_upper) );
                            const async_ghosts::ptr ap(ag);
                            
                            ag->peer = g->peer;
                            ag->setup(g->count,outline);
                            async_reg.push_back(ap);
                        }
                    }
                }
                assert( local_count == local_reg.size() );
                assert( async_count == async_reg.size() );
            }
            
        };
        
    }
}

#endif
