#ifndef CPPGC_TESTCASE_HPP
#define CPPGC_TESTCASE_HPP

#undef GCPP_DEBUG
#include "../../gcpp.hpp"
#include "../hpp/testCase.hpp"
#include "gcppStub.hpp"

#include <sstream>
#include <stdlib.h>
#include <unordered_map>

/**
 * the managed memory size
 */
#define z
#define _(a, b)     SHOULD_BE(a, b)
#define Z(x)        SHOULD_BE(gc::gc_map::get().size(), x)
#define u(a, b)     SHOULD_BE(a.use_count(), b)
#define uu(a, b)    SHOULD_BE(a.use_count(), b.use_count())
#define ptoi(p)                      reinterpret_cast<std::intptr_t>(p)
/**
 * constraint on test: in every test function's entry/exit the gc_map.size() must be equal to zero
 */
#define enter_test  cout<<"  [.]"<<__func__<<"()"; Z(0)
#define exit_test   cout<<"\r  [\u221A]"<<endl; Z(0); return true
namespace cppgc_test {
    using namespace gc;
    class gcppTestCase : public CPP_TESTER::testCase {
    public:
        void run(size_t, void**) {
            cout<<endl;
            BESURE(this->test_basic());
            BESURE(this->test_cast());
        }
    protected:
        template<typename T>
        using p = gc_ptr<T>;
        bool test_basic() {
            enter_test;
            p<int> x;
            // empty gc_ptr creation should not modify gc_map(the map)!
            Z(0);
            {
                // assigning a stack var
                x = 1;
                x = gcnew int(1);
                p<int> k(x);
            }
            Z(0);
            exit_test;
        }
        bool test_cast() {
            enter_test;
            {
                // convertion of double to int, must be done by client code
                p<int> _int = 1.1;
                p<base1> _base1 = new base1;
                p<base2> _base2 = new base2;
                p<derived1> _d1 = new derived1;
                p<derived12> _d12 = new derived12;
                _base1 = _d1;
                p<hderived123> _hd123 = new hderived123;
                auto x = _base2;
                _base2 = _hd123;
            }
            exit_test;
        }
    };
}

#endif
