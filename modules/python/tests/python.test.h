#ifndef IVW_PYTHON_TEST_H
#define IVW_PYTHON_TEST_H

#include <modules/python/pythonmodule.h>
#include <modules/python/pythonscript.h>

class PythonModuleTest : public ::testing::Test {
protected:
    PythonModuleTest():module_(0){}
    ~PythonModuleTest(){}
    
    virtual void SetUp(){
        const std::vector<InviwoModule*> modules = InviwoApplication::getPtr()->getModules();
        for(int i = 0;i<modules.size();i++){
            const PythonModule* pyModule = dynamic_cast<const PythonModule*>(modules[i]);
            if(pyModule){
                module_ = const_cast<PythonModule*>(pyModule);
            }
        }
    }
    virtual void TearDown(){}

    PythonModule* module_;


};

TEST_F (PythonModuleTest, init){
    ASSERT_TRUE(module_!=0);
}




#endif