#ifndef CPPGC_TESTCASE_HPP
#define CPPGC_TESTCASE_HPP

#include "../../gcpp.hpp"
#include "../hpp/testCase.hpp"
#include <typeinfo>


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


namespace cppgc_test {
    using namespace gc;
    class cppgcTestCase : public CPP_TESTER::testCase {
        class x {
        };
    public:
        void run(size_t, void**) {
            derived obj;
            gc_ptr<base1> p1 = ref_cast<base1>(&obj);
            gc_ptr<base2> p2 = ref_cast<base2>(&obj);
            int* x = new int(666);
            double* d = new double(.666);
            cout<<p1<<" "<<sizeof(decltype(p1))<<endl;
            cout<<p2<<" "<<sizeof(decltype(p2))<<endl;
            cout<<dynamic_cast<base1*>(p1.get())<<" "
                <<dynamic_cast<base2*>(p2.get())<<" "
                <<endl;
            gc_ptr<int> int1 = gc_new int(10);
            gc_node<int> gn1(&int1);
            SHOULD_BE(gn1.getAddress(), int1.get());
            p1.reset();
        }
    };
}

#endif
