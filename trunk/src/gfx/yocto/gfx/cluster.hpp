#ifndef YOCTO_GFX_CLUSTER_INCLUDED
#define YOCTO_GFX_CLUSTER_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"
#include "yocto/exception.hpp"
#include "yocto/core/list.hpp"
#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/sort/remove-if.hpp"

namespace yocto
{
    namespace gfx
    {
        
        
        class coord
        {
        public:
            YOCTO_MAKE_OBJECT
            
            static const int __inside = 0x00;
            static const int __left   = 0x01;
            static const int __right  = 0x02;
            static const int __top    = 0x04;
            static const int __bottom = 0x08;
            
            
            static int pos2idx( const int pos )
            {
                switch(pos)
                {
                    case __top:            return 0;
                    case __top|__right:    return 1;
                    case __right      :    return 2;
                    case __bottom|__right: return 3;
                    case __bottom:         return 4;
                    case __bottom|__left:  return 5;
                    case __left:           return 6;
                    case __top|__left:     return 7;
                        
                    default:
                        break;
                }
                throw exception("pos2idx: invalid position");
            }
            
            static int mirror_pos( const int pos ) throw()
            {
                switch(pos)
                {
                    case __top:            return __bottom;
                    case __top|__right:    return __bottom|__left;
                    case __right      :    return __left;
                    case __bottom|__right: return __top|__left;
                    case __bottom:         return __top;
                    case __bottom|__left:  return __top|__right;
                    case __left:           return __right;
                    case __top|__left:     return __bottom|__right;
                        
                    default:
                        break;
                }
                throw exception("mirror_pos: invalid position");
            }
            
            const    unit_t x;
            const    unit_t y;
            coord          *next;
            coord          *prev;
            unsigned        remaining;
            bool            available[8];
            
            inline coord( const unit_t X, const unit_t Y) throw() :
            x(X),
            y(Y),
            next(0),
            prev(0),
            remaining(8),
            available()
            {
                for(size_t i=0;i<8;++i)
                {
                    available[i] = true;
                }
            }
            
            void on_left()
            {
                disable(__left);
                disable(__left|__top);
                disable(__left|__bottom);
            }
            
            void on_right()
            {
                disable(__right);
                disable(__right|__top);
                disable(__right|__bottom);
            }
            
            void on_top()
            {
                disable(__top);
                disable(__top|__left);
                disable(__top|__right);
            }
            
            void on_bottom()
            {
                disable(__bottom);
                disable(__bottom|__left);
                disable(__bottom|__right);
            }
            
            inline ~coord() throw()
            {
            }
            
            int adjacent_pos( const unit_t X, const unit_t Y ) const throw()
            {
                const unit_t dx = X - x;
                const unit_t dy = Y - y;
                switch(dx)
                {
                    case  1:
                        switch(dy)
                    {
                        case -1: return __right | __top;
                        case  0: return __right;
                        case  1: return __right | __bottom;
                        default:
                            return 0;
                    }
                        
                    case -1:
                        switch(dy)
                    {
                        case -1: return __left | __top;
                        case  0: return __left;
                        case  1: return __left | __bottom;
                        default:
                            return 0;
                    }
                        
                        
                    case  0:
                        switch(dy)
                    {
                        case -1: return __top;
                        case  1: return __bottom;
                        default:
                            return 0;
                    }
                        
                    default:
                        break;
                }
                return 0;
            }
            
            void disable(const int pos)
            {
                const int idx = pos2idx(pos);
                if( available[idx] )
                {
                    assert(remaining>0);
                    available[idx] = false;
                    --remaining;
                }
            }
            
            static bool is_inside(const coord *C ) throw()
            {
                return (C->remaining <= 0);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(coord);
        };
        
        
        class cluster : public object
        {
        public:
            typedef coord *coord_ptr;
            
            core::list_of_cpp<coord> coords;
            vector<coord_ptr>        border;
            cluster *next;
            cluster *prev;
            
            explicit cluster() throw() :
            coords(),
            border(),
            next(0),
            prev(0)
            {
            }
            
            virtual ~cluster() throw()
            {
            }
            
            void clear() throw()
            {
                coords.clear();
                border.free();
            }
            
            bool accept(const unit_t  X,
                        const unit_t  Y) const
            {
                //______________________________________________________________
                //
                // scan edges only
                //______________________________________________________________
                for(size_t i=border.size();i>0;--i)
                {
                    const coord_ptr p = border[i];
                    assert(p);
                    if( p->adjacent_pos(X,Y) > 0)
                    {
                        //std::cerr << "coord (" << p->x << "," << p->y <<") accepts (" << X << "," << Y << ")" << std::endl;
                        assert(abs(p->x-X)<=1);
                        assert(abs(p->y-Y)<=1);
                        return true;
                    }
                }
                
                return false;
            }
            
            void insert(const unit_t X,
                        const unit_t Y,
                        const bitmap &from)
            {
                assert(X>=0);
                assert(Y>=0);
                assert(X<from.w);
                assert(Y<from.h);
                
                //______________________________________________________________
                //
                // create and save coord
                //______________________________________________________________
                coord *C = new coord(X,Y);
                coords.push_back(C);
                
                if(C->x <= 0 )        C->on_left();
                if(C->y <= 0 )        C->on_top();
                if(C->x >= from.w-1 ) C->on_right();
                if(C->y >= from.h-1 ) C->on_bottom();
                
                const size_t nb = border.size();
                try
                {
                    //__________________________________________________________
                    //
                    // update borders
                    //__________________________________________________________
                    if(nb<=0)
                    {
                        if(coords.size>1)
                        {
                            throw exception("unexpected coord without border");
                        }
                    }
                    else
                    {
                        bool contact = false;
                        
                        for(size_t i=nb;i>0;--i)
                        {
                            coord_ptr B    = border[i]; assert(B!=0);
                            const int Bpos = B->adjacent_pos(C->x, C->y);
                            if(Bpos>0)
                            {
                                contact = true;
                                B->disable(Bpos);
                                C->disable( coord::mirror_pos(Bpos) );
                            }
                        }
                        
                        
                        if(!contact)
                        {
                            throw exception("couldn't find adjacent border");
                        }
                    }
                    
                    
                    border.push_back(C);
                    
                    //__________________________________________________________
                    //
                    // cleanup borders
                    //__________________________________________________________
                    remove_if(border, coord::is_inside);
                    
                }
                catch(...)
                {
                    delete coords.pop_back();
                    throw;
                }
                
            }
            
            static inline int compare(const cluster *lhs, const cluster *rhs, void *) throw()
            {
                return __compare(rhs->coords.size, lhs->coords.size );
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(cluster);
            
        };
        
        class clusters : public core::list_of_cpp<cluster>
        {
        public:
            typedef cluster::coord_ptr coord_ptr;
            
            explicit clusters() throw() {}
            virtual ~clusters() throw() {}
            
            void insert(const unit_t x,
                        const unit_t y,
                        const bitmap &from)
            {
                //______________________________________________________________
                //
                // find a cluster
                //______________________________________________________________
                cluster *cl = head;
                for(;cl;cl=cl->next)
                {
                    if(cl->accept(x,y))
                    {
                        break;
                    }
                }
                
                bool created = false;
                if(!cl)
                {
                    created = true;
                    cl = new cluster();
                    push_back(cl);
                }
                
                //______________________________________________________________
                //
                // insert point
                //______________________________________________________________
                cl->insert(x,y,from);
                
                if(created)
                {
                    merge_clusters();
                }
            }
            
            
            template <typename T>
            void build_from(const pixmap<T> &img)
            {
                clear();
                const unit_t w = img.w;
                const unit_t h = img.h;
                for(unit_t j=0;j<h;++j)
                {
                    std::cerr << j+1 << "/" << h  << "\r";
                    std::cerr.flush();
                    const typename pixmap<T>::row &img_j = img[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        if( !is_zero_pixel(img_j[i]) )
                        {
                            insert(i,j,img);
                        }
                    }
                }
                std::cerr << std::endl;
                
                std::cerr << "final..." << std::endl;
                merge_clusters();
                
                core::merging<cluster>::sort(*this,cluster::compare,NULL);

            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(clusters);
            static inline bool should_merge( const cluster *a, const cluster *b ) throw()
            {
                const array<coord_ptr> &A = a->border;
                const array<coord_ptr> &B = b->border;
                for(size_t i=A.size();i>0;--i)
                {
                    const coord_ptr p = A[i];
                    for(size_t j=B.size();j>0;--j)
                    {
                        const coord_ptr q = B[j];
                        if(p->adjacent_pos(q->x,q->y)>0)
                        {
                            return true;
                        }
                    }
                }
                return false;
            }
            
            void merge_clusters()
            {
                while( try_merge_clusters() )
                    ;
            }
            
            bool try_merge_clusters()
            {
                bool ans = false;
                if(size>1)
                {
                    core::list_of<cluster> stk;
                    stk.push_back( pop_back() );
                    while(size>0)
                    {
                        cluster *source  = tail;
                        bool     done    = false;
                        
                        for(cluster *target=stk.head;target;target=target->next)
                        {
                            if(should_merge(target,source))
                            {
                                merge_cluster(target,source);
                                done = true;
                                ans  = true;
                                break;
                            }
                        }
                        
                        source = pop_back();
                        if(!done)
                        {
                            stk.push_back( source );
                        }
                        else
                        {
                            delete source;
                        }
                    }
                    
                    swap_with(stk);
                }
                return ans;
            }
            
            void merge_cluster(cluster *target,
                               cluster *source )
            {
                // steal all coordinates
                target->coords.merge_back(source->coords); assert(0==source->coords.size);
                
                // merge borders
                const size_t nt = target->border.size();
                const size_t ns = source->border.size();
                bool  contact = false;
                for(size_t i=ns;i>0;--i)
                {
                    coord_ptr p = source->border[i];
                    target->border.push_back(p);
                    for(size_t j=nt;j>0;--j)
                    {
                        coord_ptr q   = target->border[j];
                        const int pos = p->adjacent_pos(q->x, q->y);
                        if(pos>0)
                        {
                            contact = true;
                            p->disable(pos);
                            q->disable( coord::mirror_pos(pos) );
                        }
                    }
                }
                if(!contact)
                {
                    throw exception("no contact during merge");
                }
                remove_if(target->border, coord::is_inside);
            }
            
            
            
        };
        
        
    }
}

#endif
