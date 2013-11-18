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
            typedef LAYOUT                 layout_type;
            typedef fields_setup<LAYOUT>   fields_list;
            typedef typename LAYOUT::coord coord;
            
            const LAYOUT         outline;          //!< this layout + ghosts
            const LAYOUT         inboard;          //!< this layout - ghosts
            const vector<LAYOUT> sides;            //!< need up to date ghosts to be comptuted
            const size_t         local_count;      //!< #local ghosts (used for PBC)
            const size_t         async_count;      //!< #async ghosts (used with MPI)
            const size_t         num_requests;     //!< #MPI requests for data exchange
            
            virtual ~dataspace() throw() {}
            
            explicit dataspace(const LAYOUT       &L,
                               const fields_list  &F,
                               const ghosts_setup &G) :
            array_db( F.size() + LAYOUT::DIMENSIONS ),
            LAYOUT(L),
            outline(L),
            inboard(L),
            sides(2*LAYOUT::DIMENSIONS,as_capacity),
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
                F.create( outline, *this);
                
                //--------------------------------------------------------------
                // and prepare ghosts memory
                //--------------------------------------------------------------
                for( size_t i=1;i<=async_count;++i)
                {
                    async_reg[i]->allocate_for( F.max_interleaved_size );
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
                //--------------------------------------------------------------
                // the outline/inboard are now set to the current layout
                //--------------------------------------------------------------
                const LAYOUT &self = this->as_layout();
                coord out_lower    = self.lower;
                coord out_upper    = self.upper;
                coord inb_lower    = self.lower;
                coord inb_upper    = self.upper;
                
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
                            //--------------------------------------------------
                            //-- modify layouts
                            //--------------------------------------------------
                            __coord(out_lower,dim) -= g->count;
                            __coord(inb_lower,dim) += g->count;
                            
                            //--------------------------------------------------
                            //-- create the corresponding side layout
                            //--------------------------------------------------
                            coord side_lower = self.lower;
                            coord side_upper = self.upper;
                            __coord(side_upper,dim) = __coord(self.lower,dim) + unit_t(g->count-1);
                            const LAYOUT side( side_lower, side_upper );
                            ( (vector<LAYOUT> &) sides).push_back(side);
                            
                            //--------------------------------------------------
                            //-- done for a lower ghost
                            //--------------------------------------------------
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
                            
                            //--------------------------------------------------
                            //-- modify layouts
                            //--------------------------------------------------
                            __coord(out_upper,dim) += g->count;
                            __coord(inb_upper,dim) -= g->count;
                            
                            //--------------------------------------------------
                            //-- create the corresponding side layout
                            //--------------------------------------------------
                            coord side_lower = self.lower;
                            coord side_upper = self.upper;
                            __coord(side_lower,dim) = __coord(self.upper,dim) - unit_t(g->count-1);
                            const LAYOUT side( side_lower, side_upper );
                            ( (vector<LAYOUT> &) sides).push_back(side);
                            
                            //--------------------------------------------------
                            //-- done for an upper ghost
                            //--------------------------------------------------
                            ++num_async;
                        }
                    }
                    
                }
                
                //--------------------------------------------------------------
                //-- recompute outline
                //--------------------------------------------------------------
                new ((void*)&outline) LAYOUT(out_lower,out_upper);
                
                //--------------------------------------------------------------
                //-- recompute inboard
                //--------------------------------------------------------------
                new ((void*)&inboard) LAYOUT(inb_lower,inb_upper);
                
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
