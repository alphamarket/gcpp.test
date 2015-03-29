#ifndef CPPGC_TESTCASE_HPP
#define CPPGC_TESTCASE_HPP

#include "../../gcpp.hpp"
#include "../hpp/testCase.hpp"


struct base1
{
   virtual ~base1() { }
   int value1;
};

struct base2
{
   virtual ~base2() { }
   int value2;
};

struct derived : public base1, public base2
{
};


#include <sstream>
#include <stdlib.h>
#include <unordered_map>
namespace cppgc_test {
    using namespace gc;
    class cppgcTestCase : public CPP_TESTER::testCase {
        class x {
        };
    public:
        void run(size_t, void**) {
            {
            	#define e(x) cout<<x<<endl;
                derived* obj = new derived;
                e("<derived>*obj created in heap : "<<obj);
                derived robj;
                e("<derived>robj created in stack: "<<(derived*)&obj);
                e("<g<derived>>p0 = <ref<derived>>&robj");
                gc_ptr<derived> p0 = ref2ptr<derived>(&robj);       // will be consideres ad manage object,
                e("<g<base1>>p01 = <ref<base1>>&robj");
                gc_ptr<base1> p01  = ref2ptr<base1>(&robj);
                e("<g<base2>>p02 = <ref<base2>>&robj");
                gc_ptr<base2> p02  = ref2ptr<base2>(&robj);
//                gc_ptr<derived> p0_e = robj;                      // an static_assertion will stop it to work too
                e("<derived>*obj assigned to <g<derived>>p2");
                gc_ptr<derived> p2 = obj;
                e("<g<base1>>p1 created");
                gc_ptr<base1> p1;
                e("p1 = p2");
                p1 = p2;
                e("<g<base1>>x0 = p1");
                gc_ptr<base1> x0 = p1;
                e("<g<int>>x created");
                gc_ptr<int> x = gc_new int(666);
                e("<g<double>>d created");
                gc_ptr<double> d = gc_new double(.666);
                e("<g<double>>int1 created");
                gc_ptr<int> int1 = gc_new int(10);
                e("<g<double>>x = <g<int>>d");
                d = x;
            }
        }
    };
}

#endif
