#include "yocto/math/ztype.hpp"
#include "yocto/gems/io/xyz.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    
    namespace gems
    {
        using namespace math;
        
        ////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////
        template <>
        XYZ<real_t>:: Line:: Line( const string &ID, real_t X, real_t Y, real_t Z) :
        id( ID ),
        x( X ),
        y( Y ),
        z( Z )
        {
        }
        
        template <>
        XYZ<real_t>:: Line:: ~Line() throw()
        {
            
        }
        
        template <>
        XYZ<real_t>:: Line:: Line( const Line &other ) :
        id( other.id ),
        x( other.x ),
        y( other.y ),
        z( other.z )
        {
        }
        
        
        ////////////////////////////////////////////////////////////////////
        // Frame
        ////////////////////////////////////////////////////////////////////
        template <>
        bool XYZ<real_t>:: Frame:: HasHeader(ios::istream &fp,
                                             size_t       &num_atoms,
                                             string       &comment)
        {
            
            string &in = comment;
            //--------------------------------------------------------------
            // #atoms
            //--------------------------------------------------------------
            in.clear();
            if( fp.read_line(in) < 0 ) return false;
            in.clean( character<char>::is_space );
            num_atoms = strconv::to_size( in, "#atoms");
            if( num_atoms <= 0 )
                throw exception("XYZ(invalid #atoms)");
            
            //--------------------------------------------------------------
            // Comment
            //--------------------------------------------------------------
            in.clear();
            if( fp.read_line(in) < 0 )
                throw exception("XYZ(missing comment)");
            return true;
        }
        
        
        template <>
        void XYZ<real_t>:: Frame:: ReadAtoms(ios::istream &fp,
                                             size_t n,
                                             Callback &cb)
        {
            static const char *atom_id[] = { "x", "y", "z", "" };
            assert(n>0);
            
            string in;
            //--------------------------------------------------------------
            // process atoms
            //--------------------------------------------------------------
            for(size_t i=1; i <= n; ++i )
            {
                in.clear();
                if( fp.read_line(in) < 0 )
                    throw exception("XYZ(missing atom %u/%u)", unsigned(i), unsigned(n));
                tokenizer tkn( in );
                if( !tkn.get_next( character<char>::is_space ) )
                    throw exception("XYZ(missing ID of atom %u", unsigned(i));
                
                const string id( tkn.to_string() );
                real_t       coord[4] = { 0, 0, 0, 0};
                
                for( size_t j=0;j<3;++j)
                {
                    const char *tag = atom_id[j];
                    if( !tkn.get_next( character<char>::is_space ) )
                        throw exception("XYZ(missing coord <%s> of atom %u='%s'", tag, unsigned(i), id.c_str());
                    const string val( tkn.token(), tkn.units() );
                    coord[j] = strconv::to<real_t>( val.c_str(), tag);
                }
                const Line L(id,coord[0],coord[1],coord[2]);
                cb(L);
            }
        }
        
        template <>
        XYZ<real_t>:: Frame:: Frame(size_t n) :
        FrameBase(n,as_capacity),
        comment()
        {
        }
        
        template <>
        XYZ<real_t>:: Frame:: ~Frame() throw() {}
        
        template <>
        void XYZ<real_t>:: Frame:: AppendAtom(const XYZ<real_t>::Line &line)
        {
            push_back(line);
        }
        
        template <>
        XYZ<real_t>::Frame * XYZ<real_t>:: Frame:: Load( ios::istream &fp )
        {
            size_t num_atoms = 0;
            string comment;
            if( HasHeader(fp, num_atoms, comment) )
            {
                auto_ptr<Frame> frame( new Frame(num_atoms) );
                frame->comment.swap_with(comment);
                Frame::Callback cb( frame.__get(), & Frame::AppendAtom );
                Frame::ReadAtoms(fp, num_atoms, cb);
                return frame.yield();
            }
            else
                return 0;
        }
        
       
        
        
        ////////////////////////////////////////////////////////////////////
        // Frames
        ////////////////////////////////////////////////////////////////////
        template <>
        XYZ<real_t>:: Frames:: Frames() throw() {}
        
        template <>
        XYZ<real_t>:: Frames:: ~Frames() throw() {}
        
        template <>
        void XYZ<real_t>:: Frames:: load ( ios::istream &fp, size_t nmax)
        {
            size_t nld = 0;
            for(;;)
            {
                Frame *frame = XYZ::Frame::Load(fp);
                if( !frame ) break;
                const Frame::Ptr f( frame );
                push_back(f);
                std::cerr << "."; std::cerr.flush();
                ++nld;
                if(nmax>0&&nld>=nmax) break;
            }
            std::cerr << std::endl;
        }
        
        
    }
    
    
}
