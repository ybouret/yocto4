#include "yocto/gems/io/xyz.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/exception.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
    
    namespace gems
    {
        
        namespace IO
        {
            
            ////////////////////////////////////////////////////////////////////
            //
            ////////////////////////////////////////////////////////////////////
            XYZ:: Line:: Line( const string &ID, double X, double Y, double Z) :
            id( ID ),
            x( X ),
            y( Y ),
            z( Z )
            {
            }
            
            XYZ:: Line:: ~Line() throw()
            {
                
            }
            
            XYZ:: Line:: Line( const Line &other ) :
            id( other.id ),
            x( other.x ),
            y( other.y ),
            z( other.z )
            {
            }
            
            
            ////////////////////////////////////////////////////////////////////
            // Frame
            ////////////////////////////////////////////////////////////////////
            bool XYZ:: Frame:: HasHeader(ios::istream &fp,
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
            
            
            void XYZ:: Frame:: ReadAtoms(ios::istream &fp,
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
                    
                    const string id( tkn.token(), tkn.units() );
                    double       coord[4] = { 0, 0, 0, 0};
                    
                    for( size_t j=0;j<3;++j)
                    {
                        const char *tag = atom_id[j];
                        if( !tkn.get_next( character<char>::is_space ) )
                            throw exception("XYZ(missing coord <%s> of atom %u='%s'", tag, unsigned(i), id.c_str());
                        const string val( tkn.token(), tkn.units() );
                        coord[j] = strconv::to_double( val.c_str(), tag);
                    }
                    const Line L(id,coord[0],coord[1],coord[2]);
                    cb(L);
                }
            }
            
            
            XYZ:: Frame:: Frame(size_t n) :
            FrameBase(n,as_capacity),
            comment()
            {
            }
            
            XYZ:: Frame:: ~Frame() throw() {}
            
            
            
            XYZ::Frame * XYZ:: Frame:: Load( ios::istream &fp )
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
            
            
            void XYZ:: Frame:: AppendAtom(const yocto::gems::IO::XYZ::Line &line)
            {
                push_back(line);
            }
            
            
            ////////////////////////////////////////////////////////////////////
            // Frames
            ////////////////////////////////////////////////////////////////////
            XYZ:: Frames:: Frames() throw() {}
            
            XYZ:: Frames:: ~Frames() throw() {}
            
            void XYZ:: Frames:: load ( ios::istream &fp, size_t nmax)
            {
                size_t nld = 0;
                for(;;)
                {
                    Frame *frame = XYZ::Frame::Load(fp);
                    if( !frame ) break;
                    const Frame::Ptr f( frame );
                    push_back(f);
                    std::cerr << f->size() << std::endl;
                    std::cerr << "."; std::cerr.flush();
                    ++nld;
                    if(nmax>0&&nld>=nmax) break;
                }
                std::cerr << std::endl;
            }

            
        }
        
    }
    
}
