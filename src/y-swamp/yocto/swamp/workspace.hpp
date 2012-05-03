#ifndef YOCTO_SWAMP_WORKSPACE_INCLUDED
#define YOCTO_SWAMP_WORKSPACE_INCLUDED 1

#include "yocto/swamp/ghosts.hpp"
#include "yocto/swamp/factory.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace swamp 
    {
        
        template <typename LAYOUT>
        class workspace : public LAYOUT,  public array_db
        {
        public:
            typedef typename LAYOUT::coord coord;
            
            //! prepare all layouts
            explicit workspace(const LAYOUT              &L,
                               const ghosts_setup<coord> &G
                               ) :
            LAYOUT(L),
            array_db(),
            outline( *this ),
            sync(    *this ),
            F(8),
            localGhosts(4,as_capacity),
            asyncGhosts(8,as_capacity)
            {
                apply( G );
            }
            
            virtual ~workspace() throw()
            {
            }
            
            
            template <typename ARRAY>
            inline ARRAY &create( const string &name )
            {
                F.template make<ARRAY>(name,outline,*this);
                array_db &adb = *this;
                return adb[ name ].as<ARRAY>();
            }
            
            template <typename ARRAY>
            inline ARRAY &create( const char *id )
            {
                const string name(id);
                return create<ARRAY>(name);
            }
            
            const LAYOUT outline;   //!< layout+ghosts
            const LAYOUT sync;      //!< layout - async ghosts: always synchronous
            
        private:
            factory<LAYOUT> F;
            YOCTO_DISABLE_COPY_AND_ASSIGN(workspace);
            vector<local_ghosts::ptr> localGhosts;
            vector<async_ghosts::ptr> asyncGhosts;
            
            //! compute outline and ghosts from the setup
            inline void apply( const ghosts_setup<coord> &G )
            {
                std::cerr << "######## workspace: layout=" << this->__layout() << std::endl;
                const unit_t *local_g = (const unit_t *) &G.local.count;
                const unit_t *lower_g = (const unit_t *) &G.lower.count;
                const unit_t *upper_g = (const unit_t *) &G.upper.count;
                
                //==============================================================
                //
                // First pass: compute outline and register ghosts
                //
                //==============================================================
                {
                    const unit_t *lower_p = (const unit_t *) &G.lower.peer;
                    const unit_t *upper_p = (const unit_t *) &G.upper.peer;
                    const unit_t *w       = (const unit_t *) & this->width;
                    coord         outLo( this->lower );
                    coord         outUp( this->upper );
                    unit_t  *     pLower = (unit_t *) &outLo;
                    unit_t  *     pUpper = (unit_t *) &outUp;
                    
                    for( unsigned dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
                    {
                        std::cerr << "\t ---- dim #" << dim << std::endl;
                        const unit_t local_ng = local_g[dim];
                        if( local_ng > 0 )
                        {
                            std::cerr << "\t#localGhosts=" << local_ng << std::endl;
                            //--------------------------------------------------
                            // check layout width
                            //--------------------------------------------------
                            if( local_ng > w[dim] )
                                throw exception("too many ghosts in dimension %u", dim);
                            
                            //--------------------------------------------------
                            // create the local ghosts
                            //--------------------------------------------------
                            local_ghosts::ptr pG( new local_ghosts(local_ng,dim) );
                            localGhosts.push_back( pG );
                            pLower[dim] -= local_ng;
                            pUpper[dim] += local_ng;
                        }
                        
                        {
                            const unit_t lower_ng = lower_g[dim];
                            if( lower_ng > 0 )
                            {
                                std::cerr << "\t#lowerGhosts=" << lower_ng << std::endl;
                                if( local_ng > 0 )
                                    throw exception("ghosts conflict in dimension %u", dim );
                                async_ghosts::ptr pG( new async_ghosts(lower_ng, ghost::get_lower_position(dim), lower_p[dim] ) );
                                std::cerr << "\t  peer=" << pG->peer << std::endl;
                                asyncGhosts.push_back( pG );
                                pLower[dim] -= lower_ng;
                            }
                        }
                        
                        {
                            const unit_t upper_ng = upper_g[dim];
                            if( upper_ng > 0 )
                            {
                                std::cerr << "\t#upperGhosts=" << upper_ng << std::endl;
                                if( local_ng > 0 )
                                    throw exception("ghosts conflict in dimension %u", dim );
                                async_ghosts::ptr pG( new async_ghosts(upper_ng, ghost::get_upper_position(dim), upper_p[dim] ) );
                                std::cerr << "\t  peer=" << pG->peer << std::endl;
                                asyncGhosts.push_back( pG );
                                pUpper[dim] += upper_ng;
                            }
                        }
                        
                    }
                    
                    //==========================================================
                    //
                    // recreate outline
                    //
                    //==========================================================
                    new ((void*)&outline) LAYOUT( outLo, outUp );
                    std::cerr << "######## workspace: outline=" <<  outline << std::endl;
                    
                }
                
                {
                    //==========================================================
                    //
                    // second pass: attribute offsets with sub layouts
                    //
                    //==========================================================
                    const coord outLo( outline.lower );
                    const coord outUp( outline.upper );
                    const coord layLo( this->lower );
                    const coord layUp( this->upper );
                    
                    size_t iLocal=0, iAsync=0;
                    for( unsigned dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
                    {
                        
                        //------------------------------------------------------
                        // local ghosts
                        //------------------------------------------------------
                        const unit_t local_ng = local_g[dim];
                        if( local_ng > 0 )
                        {
                            const unit_t ns = local_ng - 1;
                            local_ghosts &g = * localGhosts[ ++iLocal ];
                            //--------------------------------------------------
                            //-- lower.inside
                            //--------------------------------------------------
                            {
                                coord subLo( outLo );
                                coord subUp( outUp );
                                __coord(subLo,dim) = __coord(layLo,dim);
                                __coord(subUp,dim) = __coord(subLo,dim) + ns;
                                const LAYOUT sub(subLo,subUp);
                                std::cerr << "\t\tlocalGhosts[" << iLocal << "]=(" << local_ng << ").lower.inside=" << sub << std::endl;
                                outline.load_offsets( sub, g.lower.inside.offsets );
                                assert( sub.items == g.lower.inside.offsets.size() );
                            }
                            
                            //--------------------------------------------------
                            //-- lower.mirror = upper.outside
                            //--------------------------------------------------
                            {
                                coord subLo( outLo );
                                coord subUp( outUp );
                                __coord(subLo,dim) = __coord(layUp,dim) + 1;
                                const LAYOUT sub(subLo,subUp);
                                std::cerr << "\t\tlocalGhosts[" << iLocal << "]=(" << local_ng << ").lower.mirror=" << sub << std::endl;
                                outline.load_offsets( sub, g.lower.mirror.offsets );
                                assert( sub.items == g.lower.mirror.offsets.size() );
                            }
                            
                            //--------------------------------------------------
                            //-- upper.inside
                            //--------------------------------------------------
                            {
                                coord subLo( outLo );
                                coord subUp( outUp );
                                __coord(subUp,dim) = __coord(layUp,dim);
                                __coord(subLo,dim) = __coord(subUp,dim) - ns;
                                const LAYOUT sub(subLo,subUp);
                                std::cerr << "\t\tlocalGhosts[" << iLocal << "]=(" << local_ng << ").upper.inside=" << sub << std::endl;
                                outline.load_offsets( sub, g.upper.inside.offsets );
                                assert( sub.items == g.upper.inside.offsets.size() );
                            }
                            
                            //--------------------------------------------------
                            //-- upper.mirror = lower.outside
                            //--------------------------------------------------
                            {
                                coord subLo( outLo );
                                coord subUp( outUp );
                                __coord(subUp,dim) = __coord(layLo,dim) - 1;
                                const LAYOUT sub(subLo,subUp);
                                std::cerr << "\t\tlocalGhosts[" << iLocal << "]=(" << local_ng << ").upper.mirror=" << sub << std::endl;
                                outline.load_offsets( sub, g.upper.mirror.offsets );
                                assert( sub.items == g.upper.mirror.offsets.size() );
                            }
                            
                            //--------------------------------------------------
                            // finalize
                            //--------------------------------------------------
                            (size_t &)(g.num_offsets) = g.lower.inside.offsets.size();
                            assert( g.lower.mirror.offsets.size() == g.num_offsets);
                            assert( g.upper.inside.offsets.size() == g.num_offsets);
                            assert( g.upper.mirror.offsets.size() == g.num_offsets);
                        }
                        
                        //------------------------------------------------------
                        // lower ghosts
                        //------------------------------------------------------
                        {
                            const unit_t lower_ng = lower_g[dim];
                            if( lower_ng > 0 )
                            {
                                async_ghosts &g = * asyncGhosts[ ++iAsync ];
                                const unit_t ns = lower_ng - 1;
                                // lower.inner
                                {
                                    coord subLo( outLo );
                                    coord subUp( outUp );
                                    __coord(subLo,dim) = __coord(layLo,dim);
                                    __coord(subUp,dim) = __coord(subLo,dim) + ns;
                                    const LAYOUT sub(subLo,subUp);
                                    std::cerr << "\t\tlowerGhosts[" << iLocal << "]=(" << lower_ng << ").lower.inner=" << sub << std::endl;
                                    outline.load_offsets( sub, g.gpair.inner.offsets );
                                    assert( sub.items == g.gpair.inner.offsets.size() );
                                }
                                
                                // lower.outer
                                {
                                    coord subLo( outLo );
                                    coord subUp( outUp );
                                    __coord(subUp,dim) = __coord(layLo,dim) - 1;
                                    const LAYOUT sub(subLo,subUp);
                                    std::cerr << "\t\tlowerGhosts[" << iLocal << "]=(" << lower_ng << ").lower.outer=" << sub << std::endl;
                                    outline.load_offsets( sub, g.gpair.outer.offsets );
                                    assert( sub.items == g.gpair.outer.offsets.size() );
                                }
                                
                                // finalize
                                (size_t&)(g.num_offsets) = g.gpair.inner.offsets.size();
                                assert( g.gpair.outer.offsets.size() == g.num_offsets);
                            }
                        }
                        
                        //------------------------------------------------------
                        // upper ghosts
                        //------------------------------------------------------
                        {
                            const unit_t upper_ng = upper_g[dim];
                            if( upper_ng > 0 )
                            {
                                async_ghosts &g = * asyncGhosts[ ++iAsync ];
                                const unit_t ns = upper_ng - 1;
                                
                                // upper.inner
                                {
                                    coord subLo( outLo );
                                    coord subUp( outUp );
                                    __coord(subUp,dim) = __coord(layUp,dim);
                                    __coord(subLo,dim) = __coord(subUp,dim) - ns;
                                    const LAYOUT sub(subLo,subUp);
                                    std::cerr << "\t\tupperGhosts[" << iLocal << "]=(" << upper_ng << ").upper.inner=" << sub << std::endl;
                                    outline.load_offsets(sub, g.gpair.inner.offsets );
                                    assert( sub.items ==  g.gpair.inner.offsets.size() );
                                }
                                
                                // upper.outer
                                {
                                    coord subLo( outLo );
                                    coord subUp( outUp );
                                    __coord(subLo,dim) = __coord(layUp,dim) + 1;
                                    const LAYOUT sub(subLo,subUp);
                                    std::cerr << "\t\tupperGhosts[" << iLocal << "]=(" << upper_ng << ").upper.outer=" << sub << std::endl;
                                    outline.load_offsets(sub, g.gpair.outer.offsets );
                                    assert( sub.items ==  g.gpair.outer.offsets.size() );
                                }
                                
                                // finalize
                                (size_t&)(g.num_offsets) = g.gpair.inner.offsets.size();
                                assert( g.gpair.outer.offsets.size() == g.num_offsets);
                            }
                        }
                        
                        
                    }
                }
                
            }
        };
        
        
    }
}


#endif
