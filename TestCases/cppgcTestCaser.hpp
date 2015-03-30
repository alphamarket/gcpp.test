#ifndef CPPGC_TESTCASE_HPP
#define CPPGC_TESTCASE_HPP

#include "../../gcpp.hpp"
#include "../hpp/testCase.hpp"


struct base1
{
   int value1;
};

struct base2
{
   int value2, value3;
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
            derived* obj = NULL;
            assert(obj == nullptr);
            gc_ptr<derived> suv1, suv2;
            {
            	#define e(x) cout<<x<<endl;
                obj = new derived;
                e("<derived>*obj created in heap : "<<obj);
                derived robj;
                e("<derived>robj created in stack: "<<(derived*)&obj);
                e("<g<derived>>p0 = <ref<derived>>&robj");
                gc_ptr<derived> p0 = robj;       // will be consideres ad manage object,
                e("<g<base1>>p01 = <ref<base1>>&robj");
                gc_ptr<base1> p01  = robj;
                e("<g<base2>>p02 = <ref<base2>>&robj");
                gc_ptr<base2> p02  = robj;
//                gc_ptr<derived> p0_e = robj;                      // an static_assertion will stop it to work too
                e("<g<derived>>p2 = <derived>*obj");
                gc_ptr<derived> p2 = obj;
                e("<g<base2>>p1 created");
                gc_ptr<base2> p1;
                e("p1 = p2");
                p1 = p2;
                e("<g<int>>x created");
                gc_ptr<int> x = gcnew int(666);
                e("<g<double>>d created");
                gc_ptr<double> d = gcnew double(.666);
                e("<g<double>>int1 created");
                gc_ptr<int> int1 = gcnew int(10);
                e("<g<double>>d = <g<int>>x");
                d = x;
                int sx = 1;
                e("<int>sx created: "<<(int*)&sx);
                e("<g<int>>sxp = <ref<int>>&sx");
                gc_ptr<int> sxp = sx;
                test_gc_ptr_pass(p1);
                cout<<"\033[96m[refpas-exit]& "<<p1.get_pure()<<"\033[m"<<endl;
                test_gc_ptr_pass_copy(p1);
                cout<<"\033[96m[refpas-exit]<> "<<p1.get_pure()<<"\033[m"<<endl;
                test_gc_ptr_pass(&p1);
                cout<<"\033[96m[refpas-exit]& "<<p1.get_pure()<<"\033[m"<<endl;
                suv1 = p2;
                suv2 = suv1;
                e("init on fly test");
                (gc_ptr<std::pair<int, int>>)gcnew std::pair<int, int>();
            }
            assert(gc_map.size() == 1);
            assert(suv1.use_count() == suv2.use_count());
            assert(suv1.use_count() == 2);
            suv1.reset();
            suv2.reset();
            assert((suv1.use_count() || suv2.use_count()) == 0);
            assert(gc_map.size() == 0);
#ifdef GCPP_DEBUG
            cout<<endl<<endl<<gc_void_ptr_t::statistical()<<endl;
#endif
        }
        template<typename T>
        void test_gc_ptr_pass(const gc_ptr<T>& gp) {
            cout<<"\033[96m[refpas-enter]& "<<gp.get_pure()<<"\033[m"<<endl;
        }
        template<typename T>
        void test_gc_ptr_pass_copy(const gc_ptr<T> gp) {
            cout<<"\033[96m[refpas-enter]<> "<<gp.get_pure()<<"\033[m"<<endl;
        }
        template<typename T>
        void test_gc_ptr_pass(const gc_ptr<T>* const gp) {
            cout<<"\033[96m[refpas-enter]* "<<gp->get_pure()<<"\033[m"<<endl;
        }
    };
}

#endif
