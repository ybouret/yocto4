#include "yocto/exception.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-maths.hpp"

#include "yocto/fs/vfs.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/string/conv.hpp"

#include "yocto/math/stat/descr.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/math/dat/linear.hpp"
#include "yocto/math/fcn/zfind.hpp"
#include "yocto/associative/map.hpp"

#include "yocto/code/utils.hpp"

using namespace yocto;


class Student
{
public:
    const string name;
    size_t       whom;
    const int    indx;
    double       grade;
    double       score;
    double       final;
    
    inline Student( const string &id, size_t by, double result) :
    name(id),
    whom(by),
    indx(++Counter),
    grade(result),
    score(0),
    final(0)
    {
    }
    
    inline ~Student() throw() {}
    
    Student( const Student &other ) :
    name( other.name ),
    whom( other.whom ),
    indx( other.indx ),
    grade( other.grade ),
    score( other.score ),
    final(0)
    {
    }
    
    
private:
    static int Counter;
    
};

int Student::Counter = 0;

class GradeDist
{
public:
    
    const double vMin;
    const double vMax;
    const double vFac;
    
    
    GradeDist( lua_State *L, const string &fn ) :
    vMin( Lua::Config::Get<lua_Number>(L,"vMin") ),
    vMax( Lua::Config::Get<lua_Number>(L,"vMax") ),
    vFac(1),
    pdf(L,fn,true),
    PDF(this,&GradeDist::compute_pdf),
    intg(),
    ICDF(this,&GradeDist::shifted_cdf),
    solve(1e-4)
    {
        if(vMin>=vMax)
            throw exception("Invalid min/max");
        
        const double sum = intg(vMin,vMax,PDF,1e-4);
        if(sum<=0)
            throw exception("Invalid PDF");
        (double&)vFac = 1.0/sum;
        std::cerr << "vFac=" <<vFac << std::endl;
    }
    
    
    ~GradeDist() throw()
    {
    }
    
    double compute_pdf(double x)
    {
        return pdf(x);
    }
    
    double compute_cdf(double x)
    {
        if(x<=vMin)
        {
            return 0.0;
        }
        else
        {
            if(x>=vMax)
                return 1.0;
            else
                return vFac * intg(vMin,x,PDF,1e-4);
        }
    }
    
    double get_final( double y )
    {
        return ceil( solve_final(y) * 4)/4;
    }
    
    
private:
    double shifted_cdf(double x)
    {
        return compute_cdf(x) - target;
    }
    
    double                          target;
    Lua::Function<double>           pdf;
    math::numeric<double>::function PDF;
    math::integrator<double>        intg;
    math::numeric<double>::function ICDF;
    math::zfind<double>             solve;
    
    YOCTO_DISABLE_COPY_AND_ASSIGN(GradeDist);
    
    double solve_final(double y)
    {
        if(y<=0)
            return vMin;
        
        if(y>=1)
            return vMax;
        
        target = y;
        return solve(ICDF,vMin,vMax);
    }
    
};

typedef vector<string> strings;

#if 0

void load_db( lua_State *L, strings &names )
{
    std::cerr << "-- Loading special grades" << std::endl;
    
    names.free();
    
    lua_getglobal(L, "db");
    if( !lua_istable(L, -1) )
        throw exception(" 'db' is not a TABLE");
    
    const int n = lua_rawlen(L, -1);
    for( int i=1; i<=n; ++i )
    {
        lua_rawgeti(L, -1, i);
        if( !lua_isstring(L, -1) )
        {
            throw exception("name #%d is not a STRING",i);
        }
        const string id = lua_tostring(L, -1);
        names.push_back(id);
        lua_pop(L,1);
    }
    
    quicksort(names);
    std::cerr << "-- Grades for: " << names << std::endl;
    
}
#endif


void check_db( const vector<Student> &students, const strings &names )
{
    for(size_t i=1;i<=names.size();++i)
    {
        const string &id    = names[i];
        bool          found = false;
        for(size_t j=1;j<=students.size();++j)
        {
            if( students[j].name == id )
            {
                found = true;
                break;
            }
        }
        if(!found)
            throw exception("no '%s' in students", id.c_str());
    }
}

bool is_special( const Student &s, const strings &names )
{
    for(size_t i=1;i<=names.size();++i)
    {
        if( s.name == names[i] )
            return true;
    }
    return false;
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
        // load all names/data
        //______________________________________________________________________
        const string names = Lua::Config::Get<string>(L, "names");
        std::cerr << "Loading '" << names << "'" << std::endl;
        
        vector<Student> students;
        size_t whom_max = 0;
        {
            ios::icstream fp(names);
            vector<size_t> pos(8,as_capacity);
            string line;
            while( fp.gets(line) )
            {
                std::cerr << '<' << line << '>' << std::endl;
                pos.free();
                for(size_t i=0;i<line.size();++i)
                {
                    if( line[i] == ';' )
                    {
                        line[i] = 0;
                        pos.push_back(i);
                    }
                }
                if(pos.size()<1)
                    throw exception("Invalid line '%s'", line.c_str());
                
                const string name = &line[0];
                const size_t num  = pos.size();
                
                int    whom  = 0;
                double grade = 0;
                for(size_t i=1;i<=num;++i)
                {
                    string word = &line[pos[i]+1];
                    for(size_t j=0;j<=word.size();++j)
                    {
                        if(word[j]==',') word[j] = '.';
                    }
                    //std::cerr << "\tword#" << i << "='" << word << "'" << std::endl;
                    if(word.size()>0)
                    {
                        if(whom>0)
                            throw exception("Multiple Grades for '%s'", name.c_str());
                        whom  = i;
                        grade = strconv::to<double>( word, "grade");
                        if(whom>whom_max)
                            whom_max = whom;
                    }
                }
                const Student s(name,whom,grade);
                students.push_back(s);
            }
        }
        std::cerr << "Got " << students.size() << " students..." << std::endl;
        std::cerr << "Got " << whom_max << " examiners..." << std::endl;
        
        
        
        //______________________________________________________________________
        //
        // Prepare the final distribution
        //______________________________________________________________________
        GradeDist G(L,"pdf");
        {
            ios::ocstream fp("g.dat",false);
            for(size_t i=0;i<=100;++i)
            {
                const double X = G.vMin + (i*(G.vMax-G.vMin))/100;
                fp("%g %g %g\n", X, G.compute_pdf(X), G.compute_cdf(X));
            }
        }
        
        
        //______________________________________________________________________
        //
        // compute scores
        //______________________________________________________________________
        const size_t   N = students.size();
        vector<double> scores(N,as_capacity);
        
        strings db;
        //load_db(L, db);
        
        
        const double vTop = Lua::Config::Get<lua_Number>(L,"vTop");
        
        for(size_t w=1;w<=whom_max;++w)
        {
            vector<double> grades(N,as_capacity);
            for(size_t i=1;i<=N;++i)
            {
                const Student &s = students[i];
                if(s.whom==w)
                {
                    grades.push_back(s.grade);
                    if(s.grade>=vTop)
                    {
                        db.push_back(s.name);
                    }
                }
            }
            const size_t M = grades.size();
            std::cerr << "Examiner" << w << ": " << std::endl;
            std::cerr << "\t" << M << " students" << std::endl;
            if(M>1)
            {
                double ave = 0;
                double sig = 0;
                math::compute_average_and_stddev(ave, sig, grades);
                std::cerr << "\t\tave=" << ave << std::endl;
                std::cerr << "\t\tsig=" << sig << std::endl;
            }
            double median = 0;
            double absdev = 0;
            math::compute_median_and_absdev(median, absdev, grades);
            std::cerr << "\t\tmedian=" << median << std::endl;
            std::cerr << "\t\tabsdev=" << absdev << std::endl;
            
            for(size_t i=1;i<=N;++i)
            {
                Student &s = students[i];
                if(s.whom==w)
                {
                    if(s.grade<vTop)
                    {
                        s.score = (s.grade-median)/absdev;
                        scores.push_back(s.score);
                    }
                }
            }
        }
        quicksort(db);
        check_db(students,db);
        
        
        
        const size_t P = scores.size() + db.size();
        std::cerr << "#Present=" << P << std::endl;
        
        quicksort(scores);
        vector<double> scr;
        vector<double> cdf;
        
        math::build_cdf(scr, cdf, scores);
        const size_t Q = scr.size();
        for(size_t i=2;i<Q;++i)
        {
            cdf[i] = (cdf[i]-cdf[1])/(cdf[Q]-cdf[1]);
        }
        cdf[1] = 0;
        cdf[Q] = 1;
        
        {
            ios::ocstream fp("scores_cdf.dat", false);
            for(size_t i=1;i<=scr.size();++i)
            {
                fp("%g %g\n", scr[i], cdf[i]);
            }
        }
        
        
        
        {
            ios::ocstream fp("scores.txt", false);
            for(size_t i=1;i<=N;++i)
            {
                const Student &s = students[i];
                if(s.whom<=0)
                {
                    fp << "ABS";
                }
                else
                {
                    fp("%7.3f",s.score);
                }
                fp << ' '  << s.name;
                if(s.whom>0)
                {
                    
                    fp << ' ';
                    fp("(%u,%f)", unsigned(s.whom), s.grade);
                }
                fp << '\n';
            }
        }
        
        //______________________________________________________________________
        //
        // inverse the population
        //______________________________________________________________________
        
        
        
        
        vector<double> finals(N,as_capacity);
        
        vector<double> up_max(whom_max,0);
        vector<double> lo_max(whom_max,0);
        vector<Student*> up_max_id(whom_max,0);
        vector<Student*> lo_max_id(whom_max,0);
        
        for(size_t i=1;i<=N;++i)
        {
            Student &s = students[i];
            if(s.whom<=0)
                continue;
            std::cerr << s.name << ": score=" << s.score;
            if( is_special(s, db) )
            {
                const double final = s.grade;
                std::cerr << "\tfinal= " << final << " \tSPECIAL";
                finals.push_back(final);
                s.final = s.grade;
            }
            else
            {
                const double value = math::linear(s.score, scr, cdf);
                const double final = G.get_final(value);
                std::cerr << "\tfinal= " << final << " <= " << s.grade;
                finals.push_back(final);
                s.final = final;
                const double delta = final - s.grade;
                if(delta>=0)
                {
                    if(delta>=up_max[s.whom])
                    {
                        up_max[s.whom]    = delta;
                        up_max_id[s.whom] = &s;
                    }
                }
                else
                {
                    if(delta<=lo_max[s.whom])
                    {
                        lo_max[s.whom]    = delta;
                        lo_max_id[s.whom] = &s;
                    }
                }
                
            }
            std::cerr << std::endl;
        }
        
        if(finals.size()>1)
        {
            double final_ave  = 0;
            double final_sig  = 0;
            math::compute_average_and_stddev(final_ave, final_sig, finals);
            std::cerr << "Final Average=" << final_ave << std::endl;
            std::cerr << "Final Std_Dev=" << final_sig << std::endl;
        }
        
        for(size_t i=1;i<=whom_max;++i)
        {
            std::cerr << "Examiner #" << i << std::endl;
            const Student *s = up_max_id[i];
            if(s)
            {
                std::cerr << "\tUpMax: " << s->grade << " => " << s->final << " is " << s->name << " (+" << up_max[i] << ")" << std::endl;
            }
            s= lo_max_id[i];
            if(s)
            {
                std::cerr << "\tLoMax: " << s->grade << " => " << s->final << " is " << s->name << " (" << lo_max[i] << ")" << std::endl;
            }
        }
        
        
        
        vector<double> H(20,0);
        vector<double> bins(20,0);
        for(size_t i=1;i<=20;++i) bins[i] = i;
        math::histogram(H, bins, finals);
        {
            ios::ocstream fp("hist.dat",false);
            
            for(size_t i=1;i<=20;++i)
            {
                fp("%g %g\n", bins[i], H[i]);
            }
        }
        
        
        
        
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