#ifndef YOCTO_SPADE_DATASPACE_INCLUDED
#define YOCTO_SPADE_DATASPACE_INCLUDED 1

#include "yocto/spade/fields-setup.hpp"
#include "yocto/spade/ghosts-setup.hpp"
#include "yocto/spade/local-ghosts.hpp"
#include "yocto/spade/async-ghosts.hpp"

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
            
            virtual ~dataspace() throw() {}
            
            const LAYOUT outline; //!< this layout + ghosts
            
            explicit dataspace(const LAYOUT       &L,
                               const fields_list  &F,
                               const ghosts_setup &G) :
            array_db( F.size() + LAYOUT::DIMENSIONS ),
            LAYOUT(L),
            outline(L)
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
                F.create( outline, *this );
            }
        
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(dataspace);
            vector<local_ghosts::ptr> local_ghosts_reg;
            vector<async_ghosts::ptr> async_ghosts_reg;

            //==================================================================
            // recompute outline from ghosts
            //==================================================================
            void recompute_outline( const ghosts_setup &G )
            {
                
                std::cerr << "recomputing outline" << std::endl;
                typename LAYOUT::coord out_lower = outline.lower;
                typename LAYOUT::coord out_upper = outline.upper;
                size_t num_local = 0;
                size_t num_async = 0;
                
                for( size_t dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
                {
                    //----------------------------------------------------------
                    //-- test for local ghosts (PBC)
                    //----------------------------------------------------------
                    {
                        const ghosts_info *g = G.get_local() + dim;
                        if( g->active && g->count>0 )
                        {
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
                            __coord(out_upper,dim) += g->count;
                            ++num_async;
                        }
                    }
                    
                    //----------------------------------------------------------
                    //-- recompute outline
                    //----------------------------------------------------------
                    new ((void*)&outline) LAYOUT(out_lower,out_upper);
                    std::cerr << "layout: " << this->as_layout() << std::endl;
                    std::cerr << "outline:" << outline           << std::endl;
                    
                    //----------------------------------------------------------
                    //-- reserve memory
                    //----------------------------------------------------------
                    local_ghosts_reg.reserve( num_local );
                    async_ghosts_reg.reserve( num_async );
                    
                }
                
            }
            
            //==================================================================
            // build the ghosts
            //==================================================================
            void build_ghosts(const ghosts_setup &G)
            {
                const LAYOUT &L = this->as_layout();
                for( size_t dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
                {
                    //----------------------------------------------------------
                    //-- create local ghosts
                    //----------------------------------------------------------
                    {
                        const ghosts_info *g = G.get_local() + dim;
                        if( g->active && g->count>0 )
                        {
                            local_ghosts           *lg = new local_ghosts(dim);
                            const local_ghosts::ptr lp( lg );
                            
                            lg->setup(g->count, outline, L);
                            local_ghosts_reg.push_back(lp);
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
                            
                            ag->setup(g->count,outline,L);
                            async_ghosts_reg.push_back(ap);
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
                            
                            ag->setup(g->count,outline,L);
                            async_ghosts_reg.push_back(ap);
                        }
                    }

                    
                }
                
            }
            
        };
        
    }
}

#endif
