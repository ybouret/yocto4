#include "yocto/type/factory.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;

namespace
{
    
    class Output : public object
    {
    public:
        virtual ~Output() throw() {}
        
    protected:
        explicit Output() throw() {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Output);
        
    };
    
    class TextOutput : public Output
    {
    public:
        virtual ~TextOutput() throw() {}
        
        explicit TextOutput() throw() : idx(0)
        {
            std::cerr << "New TextOutput ZERO" << std::endl;
        }
        
        explicit TextOutput(int i) throw() : idx(i) { std::cerr << "New TextOutput "<<idx << std::endl;}
        
        static Output *Create(void)  { return new TextOutput;    }
        static Output *Create(int i) { return new TextOutput(i); }
        
        const int idx;
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(TextOutput);
    };
    
}

YOCTO_UNIT_TEST_IMPL(factory)
{
    factory<Output,void>     Factory;
    
    Factory.add<TextOutput>( TextOutput::Create );
    
    auto_ptr<Output> pText( Factory( typeid(TextOutput) ) );
    
    
    factory<Output,TL1(int)> Factory1;
    Factory1.add<TextOutput>( TextOutput::Create );
    pText.reset( Factory1( typeid(TextOutput), 7 ) );

}
YOCTO_UNIT_TEST_DONE()

