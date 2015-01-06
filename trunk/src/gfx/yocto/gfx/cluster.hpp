#ifndef YOCTO_GFX_CLUSTER_INCLUDED
#define YOCTO_GFX_CLUSTER_INCLUDED 1

#include "yocto/gfx/rawpix.hpp"
#include "yocto/exception.hpp"
#include "yocto/core/list.hpp"
#include "yocto/ordered/sorted-vector.hpp"

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
                int ans = __inside;
                switch(dx)
                {
                    case  1: ans |= __right; break;
                    case -1: ans |= __left;  break;
                    default:break;
                }
                switch(dy)
                {
                    case  1: ans |= __bottom; break;
                    case -1: ans |= __top;    break;
                }
                return ans;
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
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(coord);
        };
        
        
        class cluster : public object
        {
        public:
            typedef coord *coord_ptr;
            
            core::list_of_cpp<coord> coords;
            sorted_vector<coord_ptr> border;
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
                        const unit_t  Y)
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
                // create and save coord
                coord *C = new coord(X,Y);
                coords.push_back(C);
                
                if(C->x <= 0 )        C->on_left();
                if(C->y <= 0 )        C->on_top();
                if(C->x >= from.w-1 ) C->on_right();
                if(C->y >= from.h-1 ) C->on_bottom();
                
                const size_t nb = border.size();
                try
                {
                    if(nb<=0)
                    {
                        
                        if( (C->remaining>0) && !border.insert(C))
                        {
                            throw exception("unexpected multiple coord address");
                        }
                    }
                    else
                    {
                        bool contact = false;
                        
                        if(!contact)
                        {
                            throw exception("couldn't find adjacent border");
                        }
                    }
                }
                catch(...)
                {
                    delete coords.pop_back();
                    throw;
                }
                
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(cluster);
            
        };
        
        class clusters : public core::list_of_cpp<cluster>
        {
        public:
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
                
                if(!cl)
                {
                    cl = new cluster();
                    push_back(cl);
                }
                
                cl->insert(x,y,from);
                
            }
            
            
            template <typename T>
            void build_from(const pixmap<T> &img)
            {
                clear();
                const unit_t w = img.w;
                const unit_t h = img.h;
                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &img_j = img[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        if( !is_zero_pixel(img_j[i]) )
                        {
                            insert(i,j,img);
                            return;
                        }
                    }
                }
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(clusters);
        };
        
        
    }
}

#endif
