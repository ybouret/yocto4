#ifndef YOCTO_FAME_POINTS_INCLUDED
#define YOCTO_FAME_POINTS_INCLUDED 1

#include "yocto/fame/types.hpp"
#include "yocto/core/list.hpp"


namespace yocto
{
    namespace fame
    {
        
        template <size_t DIM,typename T>
        class Point
        {
        public:
            YOCTO_MAKE_OBJECT
            
            typedef typename vertex_for<DIM,T>::type vtx_t;
            Point       *next;
            Point       *prev;
            const vtx_t *vtx;
            
            inline Point( const vtx_t &v ) throw() :
            next(0), prev(0), vtx(&v) {}
            
            inline ~Point() throw() {}
        
            class List : public core::list_of_cpp<Point>
            {
            public:
                inline explicit List() throw() {}
                inline virtual ~List() throw() {}
                inline void append( const vtx_t &v ) { this->push_back( new Point(v) ); }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(List);
            };
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Point);
        };
        
        
    }
}

#endif
