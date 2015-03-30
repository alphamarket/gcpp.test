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
#define z       gc::gc_map::get().size()
#define _(a, b) SHOULD_BE(a, b)
#define Z(x)    _(z, x)
/**
 * constraint on test: in every test function's entry/exit the gc_map.size() must be equal to zero
 */
#define enter_test  cout<<"  [.]"<<__func__; IS_ZERO(z)
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
        }
    protected:
        bool test_basic() {
            enter_test;
            gc_ptr<int> x;
            // empty gc_ptr creation should not modify gc_map(the map)!
            Z(0);
            {
                // assigning a stack var
                x = 1;
                // should not modify the map
                Z(0), _(x.use_count(), 1);
                // create a new instance of x, test and access it's value by gc_ptr's operator*()
                SHOULD_NOT_THROW(x = gcnew int(*x + 1));
                // the map should be in creased by one
                Z(1);
                // the value assignment
                _(*x, 2), _(x.use_count(), 1);
                // make a ref. copy
                gc_ptr<int> y = &*x;
                // since we used pointing to a same point, the use count should be 2
                IS_EQUAL(x.use_count(), y.use_count()), _(x.use_count(), 2);
            }
            Z(1);
            // dispose x
            x.reset();
            exit_test;
        }
    };
}

#endif
