#ifndef YOCTO_GEMS_IO_XYZ_INCLUDED
#define YOCTO_GEMS_IO_XYZ_INCLUDED 1

#include "yocto/ios/istream.hpp"
#include "yocto/functor.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/math/point3d.hpp"

namespace yocto
{
    
    namespace gems
    {
        
        //! Handling XYZ files
        template <typename T>
        struct XYZ
        {
            //! a parsed line: atom x y z
            class Line
            {
            public:
                explicit Line( const string &ID, T X, T Y, T Z );
                virtual ~Line() throw();
                Line(const Line &);
                
                const string      id;
                T                 x,y,z;
                
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
            
            class Frames : public vector<typename Frame::Ptr>
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

#endif

