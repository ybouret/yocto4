#ifndef YOCTO_FLTK_CURVE_INCLUDED
#define YOCTO_FLTK_CURVE_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/geom/v2d.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/associative/set.hpp"

#include <Fl/fl_draw.H>

namespace yocto 
{
    namespace FLTK 
    {
        
        typedef geom::v2d<double> Point;
        typedef list<Point>       Points;
        
        class Curve : public Points, public counted
        {
        public:
            explicit Curve(const string &id);
            virtual ~Curve() throw();
            
            const string name;
            Fl_Color     color;
            
            const string &key() const throw();
            typedef intrusive_ptr<string,Curve> Ptr;
            typedef set<string,Curve::Ptr>      DB;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Curve);
        };
        
        
        class Curves : public Curve::DB
        {
        public:
            explicit Curves() throw();
            virtual ~Curves() throw();
            
            Curve & operator[]( const string &id ); //!< existing or create
            const Curve & operator[]( const string &id ) const; //! must exist
            
        private:
            YOCTO_DISABLE_COPY(Curves);
        };
        
    }
}

#endif
