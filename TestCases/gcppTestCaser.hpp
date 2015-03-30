#ifndef CPPGC_TESTCASE_HPP
#define CPPGC_TESTCASE_HPP

#undef GCPP_DEBUGX
#include "../../gcpp.hpp"
#include "../hpp/testCase.hpp"
#include "gcppStub.hpp"

#include <sstream>
#include <stdlib.h>
#include <unordered_map>

/**
 * the managed memory size
 */
#define z           gc::gc_map::get().size()
#define _(a, b)     SHOULD_BE(a, b)
#define Z(x)        SHOULD_BE(z, x)
#define u(a, b)     SHOULD_BE(a.use_count(), b)
#define uu(a, b)    SHOULD_BE(a.use_count(), b.use_count())
/**
 * constraint on test: in every test function's entry/exit the gc_map.size() must be equal to zero
 */
#define enter_test  cout<<"  [.]"<<__func__<<"()"; IS_ZERO(z)
#define exit_test   cout<<"\r  [\u221A]"<<endl; IS_ZERO(z); return true
namespace cppgc_test {
    using namespace gc;
    class gcppTestCase : public CPP_TESTER::testCase {
        class x {
        };
    public:
        void run(size_t, void**) {
            cout<<endl;
            BESURE(this->test_basic());
            BESURE(this->test_value());
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
                // should not modify the map
                Z(0), u(x, 1);
                // create a new instance of x, test and access it's value by gc_ptr's operator*()
                SHOULD_NOT_THROW(x = gcnew int(*x + 1));
                // the map should be in creased by one
                Z(1);
                // the value assignment
                _(*x, 2), u(x, 1);
                // make a ref. copy
                p<int> y = &*x;
                // since we used pointing to a same point, the use count should be 2
                uu(x, y), u(x, 2);
                // just a memory copy on stack, no re assignment happened
                p<int> k = y;
                // same senario, but still the use count should be 2
                uu(x, y), uu(x, k), u(k, 2);
                // try to change the value throw operator*()
                NOT_EQUAL(*x, 666), *x = 666, IS_EQUAL(*x, 666);
            }
            Z(1);
            // dispose x
            x.reset();
            exit_test;
        }
        bool test_value() {
            enter_test;
            auto a = 1.1;
            p<int> _int = a;
            _(*_int, 1), IS(*_int, int);
            p<double> _double = _int;
            _(*_double, 1);
            IS(*_double, double);
            exit_test;
        }
    };
}

#endif
