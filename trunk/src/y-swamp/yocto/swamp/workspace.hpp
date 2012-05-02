#ifndef YOCTO_SWAMP_WORKSAPCE_INCLUDED
#define YOCTO_SWAMP_WORKSAPCE_INCLUDED 1

#include "yocto/swamp/ghosts.hpp"
#include "yocto/swamp/arrays.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace swamp 
    {
        
        template <typename LAYOUT>
        class workspace : public LAYOUT, public array_db
        {
        public:
            typedef typename LAYOUT::coord coord;
            explicit workspace(const LAYOUT              &L,
                               const ghosts_setup<coord> &G
                               ) :
            LAYOUT(L),
            array_db(),
            outline( *this )
            {
                apply( G );
            }
            
            virtual ~workspace() throw()
            {
            }
            
            const LAYOUT outline; //!< layout+ghosts
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(workspace);
            vector<local_ghosts::ptr> localGhosts;
            vector<async_ghosts::ptr> asyncGhosts;
            inline void apply( const ghosts_setup<coord> &G )
            {
                std::cerr << "######## workspace: layout=" << this->lower << " -> " << this->upper << std::endl;
                //--------------------------------------------------------------
                // local ghosts
                //--------------------------------------------------------------
                const unit_t *local_g = (const unit_t *) &G.local.count;
                const unit_t *lower_g = (const unit_t *) &G.lower.count;
                const unit_t *upper_g = (const unit_t *) &G.upper.count;
                {
                    const unit_t *lower_p = (const unit_t *) &G.lower.peer;
                    const unit_t *upper_p = (const unit_t *) &G.upper.peer;
                    const unit_t *w  = (const unit_t *) & this->width;
                    coord         outLo( this->lower );
                    coord         outUp( this->upper );
                    unit_t  *     pLower = (unit_t *) &outLo;
                    unit_t  *     pUpper = (unit_t *) &outUp;
                    
                    
                    //----------------------------------------------------------
                    //
                    // First pass: compute outline and register ghosts
                    //
                    //----------------------------------------------------------
                    for( unsigned dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
                    {
                        std::cerr << "workspace: dim #" << dim << std::endl;
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
                    
                    //----------------------------------------------------------
                    //
                    // recreate outline
                    //
                    //----------------------------------------------------------
                    new ((void*)&outline) LAYOUT( outLo, outUp );
                    std::cerr << "######## workspace: outline=" <<  outline.lower << " -> " << outline.upper << std::endl;
                    
                }
                
                {
                    //----------------------------------------------------------
                    //
                    // second pass: attribute offsets with sub layouts
                    //
                    //----------------------------------------------------------
                    const coord outLo( outline.lower );
                    const coord outUp( outline.upper );
                    const coord layLo( this->lower );
                    const coord layUp( this->upper );
                    
                    size_t iLocal=0, iAsync=0;
                    for( unsigned dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
                    {
                        const unit_t local_ng = local_g[dim];
                        if( local_ng > 0 )
                        {
                            const unit_t ns = local_ng - 1;
                            local_ghosts &g = * localGhosts[ ++iLocal ];
                            //-- lower.inside
                            {
                                coord subLo( outLo );
                                coord subUp( outUp );
                                __coord(subLo,dim) = __coord(layLo,dim);
                                __coord(subUp,dim) = __coord(subLo,dim) + ns;
                                const LAYOUT sub(subLo,subUp);
                                std::cerr << "\t\tlocalGhosts[" << iLocal << "]=(" << local_ng << ").lower.inside=" << sub.lower << "->" << sub.upper << "=" << sub.items << std::endl;
                            }
                            
                            //-- lower.mirror = upper.outside
                            {
                                coord subLo( outLo );
                                coord subUp( outUp );
                                __coord(subLo,dim) = __coord(layUp,dim) + 1;
                                const LAYOUT sub(subLo,subUp);
                                std::cerr << "\t\tlocalGhosts[" << iLocal << "]=(" << local_ng << ").lower.mirror=" << sub.lower << "->" << sub.upper << "=" << sub.items << std::endl;
                                
                            }
                            
                            //-- upper.inside
                            {
                                coord subLo( outLo );
                                coord subUp( outUp );
                                __coord(subUp,dim) = __coord(layUp,dim);
                                __coord(subLo,dim) = __coord(subUp,dim) - ns;
                                const LAYOUT sub(subLo,subUp);
                                std::cerr << "\t\tlocalGhosts[" << iLocal << "]=(" << local_ng << ").upper.inside=" << sub.lower << "->" << sub.upper << "=" << sub.items << std::endl;
                            }
                            
                            //-- upper.mirror = lower.outside
                            {
                                coord subLo( outLo );
                                coord subUp( outUp );
                                __coord(subUp,dim) = __coord(layLo,dim) - 1;
                                const LAYOUT sub(subLo,subUp);
                                std::cerr << "\t\tlocalGhosts[" << iLocal << "]=(" << local_ng << ").upper.mirror=" << sub.lower << "->" << sub.upper << "=" << sub.items << std::endl;
                            }
                        }
                        
                        {
                            const unit_t lower_ng = lower_g[dim];
                            if( lower_ng > 0 )
                            {
                                async_ghosts &g = * asyncGhosts[ ++iAsync ];
                            }
                        }
                        
                        {
                            const unit_t upper_ng = upper_g[dim];
                            if( upper_ng > 0 )
                            {
                                async_ghosts &g = * asyncGhosts[ ++iAsync ];
                            }
                        }
                        
                        
                    }
                }
                
            }
        };
        
        
    }
}


#endif
