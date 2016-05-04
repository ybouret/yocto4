#ifndef YOCTO_FLTK_CURVE_INCLUDED
#define YOCTO_FLTK_CURVE_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/math/point2d.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/counted.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/associative/set.hpp"

#include <FL/fl_draw.H>

namespace yocto 
{
    namespace FLTK 
    {
        
        typedef point2d<double>   Point;
        typedef vector<Point>     Points;
        
        class Curve : public Points, public counted
        {
        public:
            explicit Curve(const string &id);
            explicit Curve(const char   *id);
            virtual ~Curve() throw();
            
            const string name;
            Fl_Color     color;
            
            const string &key() const throw();
            typedef intr_ptr<string,Curve>      Ptr;
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
            
            Curve & operator[]( const char * );
            const Curve & operator[]( const char *) const;
            
            
        private:
            YOCTO_DISABLE_COPY(Curves);
        };
        
    }
}

#endif
