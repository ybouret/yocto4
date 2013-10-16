#include "krypton.hpp"
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>
#include "yocto/fs/local-fs.hpp"
#include <iostream>
#include "yocto/ios/file-loader.hpp"
#include "yocto/pack/bwt.hpp"
#include "yocto/pack/mtf.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/zlib/zlib.h"
#include "yocto/crypto/sc/isaac-ks.hpp"

bool Krypton:: ChooseFile( string &filename )
{
    const char *fn  = filename.size() > 0 ? filename.c_str() : 0;
    const char *ans = fl_file_chooser("Choose file to Encode/Decode", 0, fn);
    if( ans )
    {
        filename = ans;
        return true;
    }
    else
        return false;
}

bool Krypton:: Passwd(string &usr, const char *mode)
{
    assert(mode);
    usr.clear();
    fl_message_title("Enter Password");
    const char *ans = fl_password(mode,0);
    if(!ans)
    {
        return false;
    }
    else
    {
        usr = ans;
        std::cerr << "Key=" << usr << std::endl;
        return true;
    }
}

static bool MayOverwrite( const string &filename )
{
    const vfs::entry ep(filename,local_fs::instance());
    if(ep.is_directory())
        throw exception("'%s' is a directory", ep.base_name);
    
    if( ep.is_regular() )
    {
        fl_message_title("Confirmation Requested");
        if( 0 == fl_choice("Overwrite '%s'", "Overwrite", "Cancel", 0, ep.base_name) )
            return true;
        else
            return false;
    }
    
    // default
    return true;
}


#define KRYPTON_EXTENSION "ykr"

void Krypton:: Cipher(const string &filename )
{
    static vfs &fs = local_fs::instance();
    const vfs::entry ep(filename,fs);
    if( !ep.is_regular() )
        throw exception("Not a regular file");
    
    string usr;
    
    if( ep.has_extension(KRYPTON_EXTENSION) )
    {
        std::cerr << "Decoding" << std::endl;
       
        string outname = filename;
        vfs::remove_extension(outname);
        std::cerr << "Should Write in " << outname << std::endl;
        
        if(!MayOverwrite(outname))
        {
            fl_message_title("Decoding Canceled");
            fl_message("No Allowed Overwrite");
            return;
        }
        
        if(!Krypton::Passwd(usr, "enter key to decode file:") )
        {
            fl_message_title("Decoding Canceled");
            fl_message("No Provided Password");
            return;
        }
        
        
        Krypton::Decode(filename, outname, usr);
        fl_message_title("Done");
        fl_message("Successful Decoding!");
    }
    else
    {
        std::cerr << "Encoding" << std::endl;
        const string outname = filename + "." + KRYPTON_EXTENSION;
        if(!MayOverwrite(outname))
        {
            fl_message_title("Encoding Canceled");
            fl_message("No Allowed Overwrite");
            return;
        }
        
        if(!Krypton::Passwd(usr, "enter key to encode file:") )
        {
            fl_message_title("Encoding Canceled");
            fl_message("No Provided Password");
            return;
        }
        
        Krypton::Encode(filename,outname,usr);
        fl_message_title("Done");
        fl_message("Successful Encoding!");
    }
    
}


void Krypton::Encode(const string &filename, const string &outname, const string &usr)
{
    
    //--------------------------------------------------------------------------
    // prepare crypto stuff
    //--------------------------------------------------------------------------
    Hash  hash;
    const digest Key = digest::checksum(hash, usr);
    std::cerr << "Key=" << Key << std::endl;
    HMAC  auth(Key);
    
    //--------------------------------------------------------------------------
    // load data
    //--------------------------------------------------------------------------
    ios::file_content P;
    ios::file_loader::append_to(P, filename);
    const digest Sgn = digest::checksum(auth,P);
    std::cerr << "Sgn=" << Sgn << std::endl;
    const size_t n = P.size();
    std::cerr << "#bytes=" << n << std::endl;
    
    //--------------------------------------------------------------------------
    // BWT/MTF
    //--------------------------------------------------------------------------
    ios::file_content Q(n,0);
    vector<size_t>    I(n,0);
    const size_t        pidx = pack::bwt::encode(Q(0), P(0), n, I(0));
    std::cerr << "pidx@" << pidx << std::endl;
    pack::move_to_front mtf;
    mtf.encode(Q.rw(), Q.ro(), Q.length());
    
    //--------------------------------------------------------------------------
    // compression
    //--------------------------------------------------------------------------
    const uLongf sourceLen = n;
    uLongf       destLen   = compressBound(sourceLen);
    ios::file_content Z(destLen,0);
    const int zerr = compress2((Bytef*)Z.rw(), &destLen, (const Bytef *)Q.ro(), sourceLen, 9);
    if( zerr != Z_OK )
        throw exception("compression failure '%s'", zError(zerr) );
    Z.keep(destLen);
    std::cerr << sourceLen << " => " << destLen << "/" << Z.size() << std::endl;
    
    
    //--------------------------------------------------------------------------
    // encryption
    //--------------------------------------------------------------------------
    crypto::isaac_key_stream  cipher(Key);
    cipher(Z);
    
    //--------------------------------------------------------------------------
    // save
    //--------------------------------------------------------------------------
    ios::ocstream fp(outname,false);
    
    //--------------------------------------------------------------------------
    // signature
    //--------------------------------------------------------------------------
    fp << Sgn;
    
    //--------------------------------------------------------------------------
    // primary index
    //--------------------------------------------------------------------------
    fp.emit<uint64_t>(pidx);
    
    //--------------------------------------------------------------------------
    // original size
    //--------------------------------------------------------------------------
    fp.emit<uint64_t>(n);
    
    //--------------------------------------------------------------------------
    // encrypted compressed data
    //--------------------------------------------------------------------------
    fp << Z;
}

void Krypton::Decode(const string &filename, const string &outname, const string &usr )
{
    //--------------------------------------------------------------------------
    // prepare crypto stuff
    //--------------------------------------------------------------------------
    Hash  hash;
    const digest Key = digest::checksum(hash, usr);
    std::cerr << "Key=" << Key << std::endl;
    HMAC  auth(Key);
    
    ios::icstream fp(filename);
    
    //--------------------------------------------------------------------------
    // Read Original Signatur
    //--------------------------------------------------------------------------
    digest OriginalSgn( hash.length );
    
    fp.load(OriginalSgn.rw(), OriginalSgn.length());
    std::cerr << "Original Sgn=" << OriginalSgn << std::endl;
    
    //--------------------------------------------------------------------------
    // Read Primary Index
    //--------------------------------------------------------------------------
    const size_t pidx = fp.read<uint64_t>();
    std::cerr << "pidx@" << pidx << std::endl;
    
    //--------------------------------------------------------------------------
    // Read Original Size
    //--------------------------------------------------------------------------
    const size_t n    = fp.read<uint64_t>();
    std::cerr << "Original Size=" << n << std::endl;
    if(pidx>=n)
        throw exception("invalid primary index!");
    
    //--------------------------------------------------------------------------
    // Read Encrypted Data
    //--------------------------------------------------------------------------
    ios::file_content Z(n,as_capacity);
    {
        char C;
        while(fp.query(C)) Z.push_back( C );
    }
    const uLongf sourceLen = Z.size();
    std::cerr << "Read " << sourceLen << std::endl;
    
    //--------------------------------------------------------------------------
    // Decypher
    //--------------------------------------------------------------------------
    {
        crypto::isaac_key_stream cipher(Key);
        cipher(Z);
    }
    
    //--------------------------------------------------------------------------
    // Decompress
    //--------------------------------------------------------------------------
    ios::file_content Q(n,0);
    uLongf destLen = n;
    const int zerr = uncompress((Bytef *)Q(0), &destLen, (const Bytef *)Z(0), sourceLen);
    if( zerr != Z_OK)
        throw exception("uncompress failure '%s'", zError(zerr));
    if(destLen!=n)
        throw exception("decompression size mismatch");
    
    std::cerr << "decompressed !" << std::endl;
    
    //--------------------------------------------------------------------------
    // BWT/MTF
    //--------------------------------------------------------------------------
    pack::move_to_front mtf;
    mtf.decode(Q.rw(),Q.ro(),Q.length());
    ios::file_content   P(n,0);
    vector<size_t>      I(n,0);
    pack::bwt::decode(P(0), Q(0), n, I(0), pidx);
    
    //--------------------------------------------------------------------------
    // signature
    //--------------------------------------------------------------------------
    const digest Sgn = digest::checksum(auth, P);
    std::cerr << "Sgn=" << Sgn << std::endl;
    
    if(Sgn != OriginalSgn)
        throw exception("Corrupted Data!");
    
    //--------------------------------------------------------------------------
    // Write the plain stream
    //--------------------------------------------------------------------------
    ios::ocstream out(outname,false);
    out << P;
}


int main(int argc, char *argv[])
{
    const char *prog = vfs::get_base_name(argv[0]);
    try
    {
        string filename;
    CHOOSE_FILE:
        while( Krypton::ChooseFile(filename) )
        {
            try
            {
                Krypton::Cipher(filename);
            }
            catch( const exception &e )
            {
                fl_message_title("Cipher Error");
                fl_alert("%s\n%s", e.what(), e.when());
            }
            catch(...)
            {
                fl_message_title("Unhandled Cipher Error");
                fl_alert("Something Went Wrong !");
            }
            
        }
        fl_message_title("Confirmation Requested");
        if( 0 != fl_choice("Really Quit", "Quit", "Continue", 0) )
            goto CHOOSE_FILE;
        
        return 0;
    }
    catch(...)
    {
        fl_message_title("Execution Error");
        fl_alert("Unhandled Exception in %s",prog);
    }
    return 1;
}