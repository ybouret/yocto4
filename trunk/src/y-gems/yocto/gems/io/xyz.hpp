#ifndef YOCTO_GEMS_IO_XYZ_INCLUDED
#define YOCTO_GEMS_IO_XYZ_INCLUDED 1

#include "yocto/ios/istream.hpp"
#include "yocto/functor.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto
{
    
    namespace gems
    {
        
        namespace IO
        {
            
            struct XYZ
            {
                class Line
                {
                public:
                    explicit Line( const string &ID, double X, double Y, double z );
                    virtual ~Line() throw();
                    Line(const Line &);
                    
                    const string id;
                    const double x,y,z;
                    
                    
                private:
                    YOCTO_DISABLE_ASSIGN(Line);
                };
              
                typedef vector<Line> FrameBase;
                
                class Frame : public FrameBase
                {
                public:
                    typedef functor<void,TL1(const Line&)>   Callback;
                    
                    static bool HasHeader( ios::istream &fp, size_t &num_atoms, string   &comment);
                    static void ReadAtoms( ios::istream &fp, size_t  num_atoms, Callback &cb );
                    
                    
                    explicit Frame(size_t);
                    virtual ~Frame() throw();
                    string   comment;
                    
                    typedef shared_ptr<Frame> Ptr;
                    
                    static Frame *Load( ios::istream &fp);
                    
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(Frame);
                    void AppendAtom( const Line & );
                };
                
                class Frames : public vector<Frame::Ptr>
                {
                public:
                    explicit Frames() throw();
                    virtual ~Frames() throw();
                    
                    void load( ios::istream &fp, size_t nmax=0);
                    
                    
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(Frames);
                };
                
            };
            
        }
        
    }
    
}

#endif

