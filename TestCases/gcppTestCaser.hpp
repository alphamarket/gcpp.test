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
                x = new int(1);
                // should not modify the map
                Z(1), u(x, 1), IS_FALSE(x.stack_referred());
                // create a new instance of x, test and access it's value by gc_ptr's operator*()
                SHOULD_NOT_THROW(x = new int(*x + 1));
                // the map should be in creased by one
                Z(1);
                // the value assignment
                _(*x, 2), u(x, 1), IS_FALSE(x.stack_referred());
                // make a ref. copy
                p<int> y = &*x;
                // since we used pointing to a same point, the use count should be 2
                uu(x, y), u(x, 2), IS_FALSE(y.stack_referred());
                // just a memory copy on stack, no re assignment happened
                p<int> k = y;
                // same senario, but still the use count should be 2
                uu(x, y), uu(x, k), u(k, 2), IS_FALSE(k.stack_referred());;
                // try to change the value throw operator*()
                NOT_EQUAL(*x, 666), *x = 666, IS_EQUAL(*x, 666);
            }
            Z(1);
            IS_EQUAL(*x, 666);
            // dispose
            x.dispose();
            exit_test;
        }
        bool test_cast() {
            enter_test;
            cout<<endl;
#define m(_int) cout<<string(#_int)<<": 0x"<<_int.get_id()<<": "<<gc_map::get().at(_int.get_id())<<" #"<<gc_map::get().size()<<endl;
            {
                // convertion of double to int, must be done by client code
                p<int> _int = new int(1.1);
                m(_int);
                p<base1> _base1 = new base1;
                m(_base1);
                p<base2> _base2 = new base2;
                m(_base2);
                p<derived1> _d1 = new derived1;
                m(_d1);
                p<derived12> _d12 = new derived12;
                m(_d12);
                m(_base1);
                _base1 = _d1;
                m(_base1);
                p<hderived123> _hd123 = new hderived123;
                m(_hd123);
//                p<base2> x = &*_base2;
//                cout<<typeid(x).name()<<" "<<typeid(_base2).name()<<endl;
//                m(x);
//                _base2 = _hd123;
                m(_base2);
                m(_hd123);
            }
            cout<<endl<<gc_map::get().size()<<endl;
            exit_test;
        }
    };
}

#endif
