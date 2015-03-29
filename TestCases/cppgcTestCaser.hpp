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
            int* y;
            double* z;
            {
                derived* obj = new derived;
                derived robj;
//                gc_ptr<derived> p0(&robj);    // will give memory corruption error on its disposal
//                gc_ptr<derived> p0 = robj;    // an static_assertion will stop it to work too
                gc_ptr<derived> p2 = obj;
                gc_ptr<base1> p1;
                p1 = p2;
                gc_ptr<base1> x0 = p1;
                gc_ptr<int> x = gc_new int(666);
                gc_ptr<double> d = gc_new double(.666);
                gc_ptr<int> int1 = gc_new int(10);
                return;
                #define _start(x) reinterpret_cast<uintptr_t>(x)
                #define _end(x) _start(x) + (typeid(x).__is_pointer_p() ? sizeof(*x) : sizeof(x))
                assert(_start(obj) <= _start(p1.get()));
                assert(_end(obj) >= _end(p1.get()));
                assert(_start(obj) <= _start(p2.get()));
                assert(_end(obj) >= _end(p2.get()));


                cout<<obj<<" "<<sizeof(decltype(obj))<<endl;
                cout<<p1<<" "<<sizeof(*p1.get())<<endl;
                cout<<p2<<" "<<sizeof(*p1.get())<<endl;
                cout<<(p1.get())<<" "
                    <<(p2.get())<<" "
                    <<endl;
                cout<<p1.use_count()<<" "<<p2.use_count()<<endl;
//                p1.reset();
    //            p2.reset();
                cout<<p1.use_count()<<" "<<p2.use_count()<<endl;
                y = x.get();
                z = d.get();
            }
            cout<<*y<<" "<<*z<<endl;
        }
    };
}

#endif
