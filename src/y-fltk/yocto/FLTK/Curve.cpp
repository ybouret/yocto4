#include "yocto/FLTK/Curve.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace FLTK
    {
        
        Curve:: ~Curve() throw() {}
        
        Curve:: Curve( const string &id ) :
        name(id),
        color(FL_WHITE)
        {
        }
        
        Curve:: Curve( const char *id ) :
        name(id),
        color(FL_WHITE)
        {
        }
        
        const string & Curve:: key() const throw() { return name; }
        
        
        Curves:: Curves() throw() : Curve::DB()
        {}
        
        
        Curves:: ~Curves() throw()
        {
        }
    
        Curve & Curves:: operator[]( const string &id ) 
        {
            Curve::Ptr *ppC = search(id);
            if( ppC )
            {
                return **ppC;
            }
            else 
            {
                Curve::Ptr p( new Curve(id) );
                if( ! insert(p) )
                    throw exception("Curves.insert('%s') unexpected failure!!!", id.c_str());
                return *p;
            }
        }

        Curve & Curves:: operator[]( const char *name )
        {
            const string id = name;
            return (*this)[id];
        }
        
        const Curve & Curves:: operator[]( const string &id ) const
        {
            const Curve::Ptr *ppC = search(id);
            if( !ppC )
            {
                throw exception("no Curves['%s']", id.c_str() );
            }
            return **ppC;
        }
        
        const Curve & Curves:: operator[]( const char *name ) const
        {
            const string id = name;
            return (*this)[id];
        }
        
        
    }
}

