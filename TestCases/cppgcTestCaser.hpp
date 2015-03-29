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


namespace cppgc_test {
    using namespace gc;
    class cppgcTestCase : public CPP_TESTER::testCase {
        class x {
        };
    public:
        void run(size_t, void**) {
            derived obj;
            gc_ptr<base2> p2 = &obj;
            gc_ptr<base1> p1 = &obj;
            int* x = new int(666);
            double* d = new double(.666);
            cout<<&obj<<" "<<sizeof(decltype(obj))<<endl;
            cout<<p1<<" "<<sizeof(decltype(p1))<<endl;
            cout<<p2<<" "<<sizeof(decltype(p2))<<endl;
            cout<<(p1.get())<<" "
                <<(p2.get())<<" "
                <<endl;
            gc_ptr<int> int1 = gc_new int(10);
            gc_node<int> gn1(&int1);
            SHOULD_BE(gn1.getAddress(), int1.get());
            cout<<p1.use_count()<<" "<<p2.use_count()<<endl;
//            p1.reset();
//            p2.reset();
            cout<<p1.use_count()<<" "<<p2.use_count()<<endl;
        }
    };
}

#endif
