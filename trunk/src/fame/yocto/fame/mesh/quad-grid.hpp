#ifndef YOCTO_FAME_QUADGRID_INCLUDED
#define YOCTO_FAME_QUADGRID_INCLUDED 1

#include "yocto/fame/mesh.hpp"
#include "yocto/fame/split/build-quad-ghosts.hpp"

#include "yocto/fame/split/quad1d.hpp"
#include "yocto/fame/split/quad2d.hpp"
#include "yocto/fame/split/quad3d.hpp"

namespace yocto
{
    namespace fame
    {
        
        template <typename Layout>
        class QuadGridData
        {
        public:
            explicit QuadGridData(const int user_rank, const int user_size) throw() :
            rank(user_rank),
            size(user_size),
            ranks(),
            sizes(),
            qlinks()
            {
                assert(rank>=0);
                assert(size>0);
                assert(rank<size);
            }
            
            virtual ~QuadGridData() throw()
            {}
            
            const int                    rank;
            const int                    size;
            const typename Layout::coord ranks;
            const typename Layout::coord sizes;
            
            
        protected:
            quad_links qlinks[ Layout::DIMENSIONS ];
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(QuadGridData);
        };
        
        //!
        template <typename Layout>
        class QuadGrid : public QuadGridData<Layout>, public Layout
        {
        public:
            static const size_t DIMENSIONS = Layout::DIMENSIONS;
            typedef typename Layout::coord    Coord;
            typedef quad_ghosts<Layout>       Ghosts;
            typedef typename Ghosts::list     GhostsList;
            typedef typename Ghosts::pointers GhostsPtrs;
            
        public:
            GhostsList   local_ghosts;
            GhostsList   async_ghosts;
            const Layout outline;
            GhostsPtrs   asynchronous[DIMENSIONS];
            
            explicit QuadGrid(const Layout &full,
                              const bool   *pbc,
                              const int     user_rank,
                              const int     user_size,
                              const int     ng
                              ) :
            QuadGridData<Layout>(user_rank,user_size),
            Layout( __split(full,this->rank,this->size,pbc, this->qlinks, this->ranks, this->sizes, int2type<DIMENSIONS>() ) ),
            local_ghosts(),
            async_ghosts(),
            outline( build_quad_ghosts<Layout>::outline_for(this->rank, this->__layout(), 1, this->qlinks, local_ghosts, async_ghosts ) ),
            asynchronous()
            {
                assert(outline.contains(*this));
                fill_async();
            }
            
            virtual ~QuadGrid() throw()
            {
            }
            
            friend inline std::ostream & operator<<( std::ostream &os, const QuadGrid &G )
            {
                os << "layout  =" << G.__layout() << std::endl;
                os << "outline =" << G.outline    << std::endl;
                os << "#local ghosts=" << G.local_ghosts.size << std::endl;
                for( const Ghosts *g = G.local_ghosts.head; g;g=g->next)
                {
                    assert(g->peer == G.rank);
                    os << "\t" << g->source.zone << " --> " << g->target.zone << std::endl;
                }
                os << "#async ghosts=" << G.async_ghosts.size << std::endl;
                for( const Ghosts *g = G.async_ghosts.head; g;g=g->next)
                {
                    os << "\t @peer=" << g->peer << " : send=" << g->source.zone << ", recv=" << g->target.zone << std::endl;
                }
                return os;
            }
            
            inline void allocate_async_for( const size_t chunk_size )
            {
                for( Ghosts *g = async_ghosts.head;g;g=g->next)
                {
                    g->allocate_for(chunk_size);
                }
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(QuadGrid);
            
            static inline
            Layout __split(const Layout &full,
                           const int     rank,
                           const int     size,
                           const bool   *pbc,
                           quad_links   *links,
                           const Coord  &__ranks,
                           const Coord  &__sizes,
                           int2type<1> )
            {
                const bool xpbc = (pbc ? pbc[0] : false);
                (Coord &)__ranks = rank;
                (Coord &)__sizes = size;
                return quad1D::split(full, rank, size, xpbc, links[0]);
            }
            
            static inline
            Layout __split(const Layout &full,
                           const int     rank,
                           const int     size,
                           const bool   *pbc,
                           quad_links   *links,
                           const Coord & __ranks,
                           const Coord & __sizes,
                           int2type<2> )
            {
                const bool has_pbc = (pbc!=NULL);
                const bool xpbc = has_pbc ? pbc[0] : false;
                const bool ypbc = has_pbc ? pbc[1] : false;
                return quad2D::split(full, rank, size, xpbc, links[0], ypbc, links[1],(Coord &)__ranks, (Coord &)__sizes);
            }
            
            static inline
            Layout __split(const Layout &full,
                           const int     rank,
                           const int     size,
                           const bool   *pbc,
                           quad_links   *links,
                           const Coord & __ranks,
                           const Coord & __sizes,
                           int2type<3> )
            {
                const bool has_pbc = (pbc!=NULL);
                const bool xpbc = has_pbc ? pbc[0] : false;
                const bool ypbc = has_pbc ? pbc[1] : false;
                const bool zpbc = has_pbc ? pbc[2] : false;
                return quad3D::split(full, rank, size, xpbc, links[0], ypbc, links[1], zpbc, links[2], (Coord &)__ranks, (Coord &)__sizes);
            }
            
            inline void fill_async() throw()
            {
                for( Ghosts *g = async_ghosts.head;g;g=g->next)
                {
                    asynchronous[g->dim].append(g);
                }
            }
            
            
        };
        
    }
}

#endif
