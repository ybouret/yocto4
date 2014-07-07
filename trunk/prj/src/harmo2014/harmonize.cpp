#include "yocto/exception.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/string/tokenizer.hpp"

using namespace yocto;


class Student
{
public:
    const string name;
    int          whom;
    const int    indx;
    double       grade;
    
    inline Student( const string &id, int by) :
    name(id),
    whom(by),
    indx(++Counter),
    grade(0)
    {
    }
    
    inline ~Student() throw() {}
    
    Student( const Student &other ) :
    name( other.name ),
    whom( other.whom ),
    indx( other.indx ),
    grade( other.grade )
    {
    }
    
    
private:
    static int Counter;
    
};

int Student::Counter = 0;


static inline bool is_sep(char C) throw()
{
    return C == ';';
}

int main(int argc, char *argv[] )
{
    static const char *progname = vfs::get_base_name(argv[0]);
    try
    {
        //______________________________________________________________________
        //
        // config.lua
        //______________________________________________________________________
        if(argc<=1)
            throw exception("usage: %s config.lua", progname);
        const string cfg = argv[1];
        Lua::State VM;
        lua_State *L = VM();
        Lua::Config::DoFile(L,cfg);
      
        //______________________________________________________________________
        //
        // load all names
        //______________________________________________________________________
        const string names = Lua::Config::Get<string>(L, "names");
        std::cerr << "Loading '" << names << "'" << std::endl;
        
        vector<Student> students;
        
        {
            vector<string> words(4,as_capacity);
            ios::icstream fp(names);
            string line;
            while( fp.gets(line) )
            {
                std::cerr << '<' << line << '>' << std::endl;
                words.free();
                tokenizer::split(words, line, is_sep);
                std::cerr << "#words=" << words.size() << std::endl;
            }
        }
        std::cerr << "Got " << students.size() << " students..." << std::endl;
        
        
        
        return 0;
    }
    catch(const exception &e)
    {
        std::cerr << "*** " << e.what() << std::endl;
        std::cerr << "*** " << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unhandled exception in " << progname << std::endl;
    }
    return 1;
}