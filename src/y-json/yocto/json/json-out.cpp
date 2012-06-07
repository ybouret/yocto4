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
            ans += s;
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
                    fp( "%.15g", asNumber() );
                    break;
                    
                case IsString:
                {
                    const string s = j2s( asString() );
                    fp.append(s);
                }
                    break;
                    
                case IsTrue:
                    fp.append("true");
                    break;
                    
                case IsFalse:
                    fp.append("false");
                    break;
                    
                case IsNull:
                    fp.append("null");
                    break;
                    
                case IsArray:
                    out( asArray(), fp, depth );
                    break;
                    
                case IsObject:
                    out( asObject(), fp, depth );
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
            __indent(fp,depth); fp.write('{'); fp.write('\n');
            size_t count = 0;
            for( Object::const_iterator i = obj.begin(); i != obj.end(); ++i, ++count )
            {
                const Pair &P = *i;
                const string s = j2s( P.name );
                __indent(fp,1+depth);
                fp.append(s);
                fp.append(" : ");
                P.value.out(fp,depth+1);
                if( count < obj.length() -1 )
                {
                    fp.write(',');
                }
                fp.write('\n');
            }
            __indent(fp,depth); fp.write('}');
        }
        
        
    }
    
}