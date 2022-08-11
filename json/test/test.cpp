#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TextTestRunner.h>
#include <string>

int main()
{
    CppUnit::TextTestRunner runner;
    CppUnit::TestFactoryRegistry &registry =
        CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());
    return !runner.run();
}
