#ifndef KRYPTON_INCLUDED
#define KRYPTON_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/exception.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/hashing/hash-mac.hpp"

using namespace yocto;

struct Krypton
{
    typedef hashing::sha1       Hash;
    typedef hashing::hmac<Hash> HMAC;
    static void Cipher( const string &filename );
    static bool ChooseFile( string &filename );
    static void Encode( const string &filename, const string &outname, const string &usr );
    static void Decode( const string &filename, const string &outname, const string &usr );
    static bool Passwd( string &usr, const char *mode );
};


#endif
