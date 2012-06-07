#include "yocto/json/json.hpp"

namespace yocto 
{
    
    namespace JSON
    {
        void Value:: output( ios::ostream &fp ) const 
        {
            out( fp, 0 );
            
        }
        
        void Value:: out( ios::ostream &fp, size_t depth ) const 
        {
            switch( type )
            {
                case IsNumber:
                    fp( "%.15g", asNumber() );
                    break;
                    
                case IsString:
                    fp.append(asString());
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
            }
            
        }
        
        void Value:: out( const Array &arr, ios::ostream &fp, size_t depth )
        {
            fp.write('[');
            for( size_t i=0; i < arr.length(); ++i )
            {
                arr[i].out(fp,depth);
                if( i < arr.length() -1 )
                {
                    fp.write(',');
                    fp.write(' ');
                }
            }
            fp.write(']');
        }
        
    }
    
}