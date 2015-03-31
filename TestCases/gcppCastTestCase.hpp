#ifndef GCCP_CASTTESTCASE_HPP
#define GCCP_CASTTESTCASE_HPP

#include <string>
#include <vector>
#include <cxxabi.h>
#include <type_traits>
#include "../hpp/teststrap.hpp"
#include "../hpp/testCase.hpp"
//#include "../../../gcpp/gcpp.1/gc_cast.hpp"

/*#   define can_cast(FROM, to)           std::is_convertible<FROM, to>()::value
//#   define can_cast_ptr(FROM, to)       can_cast(FROM*, to*)
//#   define can_dynamic_cast(FROM, to)   false;
//                                        can_cast(FROM, to) && \
//                                        !std::is_same<FROM, to>()::value && \
//                                        !std::is_const<FROM>()::value && \
//                                        std::is_class<TO>()::value && \
//                                        std::is_base_of<TO, FROM>()::value
//#   define can_static_cast(FROM, to)    can_cast(FROM, to)*/

bool fetch_cond(std::vector<bool>& cond) {
    bool res = true;
    for(auto it=cond.begin(); it != cond.end(); it++) res &= *it;
    return res;
}

template<typename FROM, typename to>
bool can_cast(bool type = 1, typename std::enable_if<std::is_convertible<FROM, to>::value>::type* =0) { return !type; }
template<typename FROM, typename to>
bool can_static_cast(bool type = 1, typename std::enable_if<
    std::is_convertible<FROM, to>::value
>::type* = 0) { return !type; }
template<typename FROM, typename to>
bool can_dynamic_cast(bool type = 1, typename std::enable_if<
    std::is_convertible<FROM, to>::value
>::type* = 0) { return !type; }

namespace cppgc_test {
    class gcppCastTestCase : public CPP_TESTER::testCase {
        void run(size_t, void**) {
#define fail(from, to, e) {cout<<endl<<"@line "<<__LINE__<<": "<<std::string(std::string(e? "" : "didn't")+"expect to cast from `"+type_name<from>()+"` to `"+type_name<to>()+"`; but "+std::string(e?"didn't":"did")+"!"); exit(0);}
#define c(from, to) if(!can_cast<from, to>())         fail(from, to, 1)
#define s(from, to) if(!can_static_cast<from, to>())  fail(from, to, 1)
#define d(from, to) if(!can_dynamic_cast<from, to>()) fail(from, to, 1)
#define nc(from, to) if(can_cast<from, to>(0))         fail(from, to, 0)
#define ns(from, to) if(can_static_cast<from, to>(0))  fail(from, to, 0)
#define nd(from, to) if(can_dynamic_cast<from, to>(0)) fail(from, to, 0)
            s(int, int);
            s(double, int);
            nd(double, int);
            nd(int, double);
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
