#include "yocto/json/json.hpp"

namespace yocto 
{
    
    namespace JSON
    {
        void Value:: output( ios::ostream &fp ) const 
        {
            out( fp, 0 );
            fp.write('\n');
        }
        
        string Value:: j2s( const String &s)
        {
            string ans = '"';
            
            for( size_t i=0; i < s.size(); ++i )
            {
                char C = s[i];
                
                switch( C )
                {
                        
                    case '"':
                    case '\\':
                    case '/':
                        ans.append( '\\' );
                        break;
                        
                    case '\b': ans.append('\\'); C = 'b'; break;
                    case '\f': ans.append('\\'); C = 'f'; break;
                    case '\n': ans.append('\\'); C = 'n'; break;
                    case '\r': ans.append('\\'); C = 'r'; break;
                    case '\t': ans.append('\\'); C = 't'; break;

                        
                    default:
                        break;
                }
                
                ans.append(C);
            }
            
            ans += '"';
            return ans;
        }
        
        static inline void __indent( ios::ostream &fp, size_t n )
        {
            for(size_t i=n;i>0;--i)
                fp.write(' ');
        }
                    
        void Value:: out( ios::ostream &fp, size_t depth ) const 
        {
            switch( type )
            {
                case IsNumber:
                    fp( "%.15g", data._Number );
                    break;
                    
                case IsString:
                {
                    const string s = j2s( *data._String );
                    fp << s;
                }
                    break;
                    
                case IsTrue:
                    fp<<"true";
                    break;
                    
                case IsFalse:
                    fp<<"false";
                    break;
                    
                case IsNull:
                    fp<<"null";
                    break;
                    
                case IsArray:
                    out( *data._Array, fp, depth );
                    break;
                    
                case IsObject:
                    out( *data._Object, fp, depth );
                    break;
            }
            
        }
        
        void Value:: out( const Array &arr, ios::ostream &fp, size_t depth )
        {
            fp.write('[');
            for( size_t i=0; i < arr.length(); ++i )
            {
                arr[i].out(fp,depth+1);
                if( i < arr.length() -1 )
                {
                    fp.write(',');
                    fp.write(' ');
                }
            }
            fp.write(']');
        }
        
        void Value:: out( const Object &obj, ios::ostream &fp, size_t depth )
        {
            fp.write('{'); fp.write('\n');
            const size_t   start = depth+2;
            vector<string> keys( obj.length(), as_capacity );
            size_t         kmax = 0;
            for( Object::const_iterator i = obj.begin(); i != obj.end(); ++i )
            {
                const string s = j2s( (*i).name );
                keys.push_back(s);
                if( keys.back().size() > kmax ) kmax = keys.back().size();
            }
            const size_t key_space   = kmax  + 1;
            const size_t child_start = start + key_space + 2; 
            
            size_t count = 1;
            for( Object::const_iterator i = obj.begin(); i != obj.end(); ++i, ++count )
            {
                const Pair &P = *i;
                __indent(fp,start);
                const string &key = keys[count];
                fp << key;
                __indent(fp,key_space - key.size());
                fp << ": ";
                P.value.out(fp,child_start);
                if( count < obj.length()  )
                {
                    fp.write(',');
                }
                fp.write('\n');
            }
            __indent(fp,depth); fp.write('}');
        }
        
        
    }
    
}
