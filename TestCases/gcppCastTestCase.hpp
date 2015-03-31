#ifndef GCCP_CASTTESTCASE_HPP
#define GCCP_CASTTESTCASE_HPP

#include <string>
#include <vector>
#include <cxxabi.h>
#include <type_traits>
#include "../hpp/testCase.hpp"
#include "../hpp/teststrap.hpp"
#include "gcppStub.hpp"
#include "../../../gcpp/gcpp.1/gc_cast.hpp"

namespace cppgc_test {
    class gcppCastTestCase : public CPP_TESTER::testCase {
        void run(size_t, void**) {
#define fail(from, to, e) {cout<<endl<<"@line "<<__LINE__<<": "<<std::string(std::string(e? "" : "didn't")+"expect to cast from `"+type_name<from>()+"` to `"+type_name<to>()+"`; but "+std::string(e?"didn't":"did")+"!")<<endl; terminate();}
#define c(from, to)  static_assert(can_cast(from, to), "expecting to cast, but couldn't!")
#define s(from, to)  static_assert(can_static_cast(from, to), "expecting static cast, but couldn't!")
#define d(from, to)  static_assert(can_dynamic_cast(from, to), "expecting dynamic cast, but couldn't!")
#define nc(from, to) static_assert(!can_cast(from, to), "didn't expecting to cast, but could!")
#define ns(from, to) static_assert(!can_static_cast(from, to), "didn't expecting static cast, but could!")
#define nd(from, to) static_assert(!can_dynamic_cast(from, to), "didn't expecting dynamic cast, but could!")
            s(int, int);
            s(double, int);
            nd(int, int);
            nd(int, double);
            ns(base1, int);
            nd(base1, int);
            s(base1, base1);
            d(base1, base1);
            ns(base1, base2);
            nd(base1, base2);
            ns(base2, base1);
            nd(base2, base1);
            s(derived1, base1);
            d(derived1, base1);
            ns(derived2, base1);
            nd(derived2, base1);
            ns(base1, derived1);
            nd(base1, derived1);
            ns(hderived123, derived1);
            nd(hderived123, derived1);
            s(hderived123, derived12);
            d(hderived123, derived12);
            ns(hderived123, derived2);
            nd(hderived123, derived2);
            s(hderived123, base3);
            d(hderived123, base3);
            s(hderived123, base1);
            d(hderived123, base1);
            s(hderived123, base2);
            d(hderived123, base2);
            s(hderived123, base3);
            d(hderived123, base3);
            derived1* w = new derived1;
            w->bval1 = 1000;
            void* x = w;
            auto y = reinterpret_cast<intptr_t>(x);
            auto z = reinterpret_cast<derived1*>(y);
            cout<<z->bval1<<endl;
            // there are not sibling dynamic cast allowed
            // the client needs to perform those types of conversions by them selves
        }
    protected:
        template<typename T>
        std::string type_name()
        {
            int status;
            std::string tname = typeid(T).name();
            char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
            if(status == 0) {
                tname = demangled_name;
                std::free(demangled_name);
            }
            return tname;
        }
    };
}

#endif // GCCASTTESTCASE_HPP
