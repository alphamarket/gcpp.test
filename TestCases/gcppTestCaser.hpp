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
#define z           gc::gc_map::get().size()
#define _(a, b)     SHOULD_BE(a, b)
#define Z(x)        SHOULD_BE(z, x)
#define u(a, b)     SHOULD_BE(a.use_count(), b)
#define uu(a, b)    SHOULD_BE(a.use_count(), b.use_count())
#define ptoi(p)                      reinterpret_cast<std::intptr_t>(p)
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
                // should not modify the map
                Z(0), u(x, 1), IS_TRUE(x.is_on_stack());
                // create a new instance of x, test and access it's value by gc_ptr's operator*()
                SHOULD_NOT_THROW(x = gcnew int(*x + 1));
                // the map should be in creased by one
                Z(1);
                // the value assignment
                _(*x, 2), u(x, 1), IS_FALSE(x.is_on_stack());
                // make a ref. copy
                p<int> y = &*x;
                // since we used pointing to a same point, the use count should be 2
                uu(x, y), u(x, 2), IS_FALSE(y.is_on_stack());
                // just a memory copy on stack, no re assignment happened
                p<int> k = y;
                // same senario, but still the use count should be 2
                uu(x, y), uu(x, k), u(k, 2), IS_FALSE(k.is_on_stack());;
                // try to change the value throw operator*()
                NOT_EQUAL(*x, 666), *x = 666, IS_EQUAL(*x, 666);
            }
            Z(1);
            // dispose x
            x.reset();
            exit_test;
        }
        bool test_cast() {
            enter_test;
            {
                // convertion of double to int, must be done by client code
                p<int> _int = (int)1.1;
                // validate the values
                _(*_int, 1), IS(*_int, int), IS_TRUE(_int.is_on_stack());
                p<base1> _base1 = new base1; _base1->bval1 = 1;
                IS(*_base1, base1), _(_base1->bval1, 1);
                p<base2> _base2 = new base2; _base2->bval2 = 2;
                IS(*_base2, base2), _(_base2->bval2, 2);
                p<derived1> _d1 = new derived1; _d1->bval1 = 11, _d1->dval1 = 1;
                _(_d1->bval1, 11), _(_d1->dval1, 1);
                p<derived12> _d12 = new derived12; _d12->bval1 = 121; _d12->bval2 = 122;
                _(_d12->bval1, 121), _(_d12->bval2, 122);
                Z(4);
                _base1 = _d1;
                // last pointer dies, increments the _d1's ref count
                Z(3), _(gc_map::get().at(_d1.get_pure()), 2);
                _(_base1.get_pure(), _d1.get_pure());
                _base1->bval1++;
                _(_base1->bval1, _d1->bval1);

            }
            exit_test;
        }
    };
}

#endif
