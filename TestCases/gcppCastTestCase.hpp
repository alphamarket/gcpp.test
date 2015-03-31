#ifndef GCCP_CASTTESTCASE_HPP
#define GCCP_CASTTESTCASE_HPP

#include <string>
#include <vector>
#include <cxxabi.h>
#include <type_traits>
#include "../hpp/testCase.hpp"
#include "../hpp/teststrap.hpp"
//#include "../../../gcpp/gcpp.1/gc_cast.hpp"

#   define can_cast(FROM, TO)           std::is_convertible<FROM, TO>::value
#   define can_cast_ptr(FROM, TO)       can_cast(FROM*, to*)
#   define can_dynamic_cast(FROM, TO)   \
                                        std::is_convertible<FROM, TO>::value && \
                                        !std::is_same<FROM, TO>::value && \
                                        !std::is_const<FROM>::value /*&& \
                                        std::is_class<TO>::value && \
                                        std::is_base_of<TO, FROM>::value*/
#   define can_static_cast(FROM, TO)    can_cast(FROM, TO)


struct base1 { int bval1; double* bp1; };
struct base2 { int bval2; double* bp2; };
struct base3 { int bval3; double* bp3; };

struct derived1 : public base1 { int dval1; double* dp1; };
struct derived2 : public base2 { int dval2; double* dp2; };
struct derived3 : public base3 { int dval3; double* dp3; };

struct derived12 : public base1, public base2 { int dval12; double* dp1; };
struct derived23 : public base2, public base3 { int dval23; double* dp2; };
struct derived31 : public base3, public base1 { int dval31; double* dp3; };

struct hderived123 : public derived12, public base3 { int hdval123; double* hdp1; };
struct hderived231 : public derived23, public base1 { int hdval231; double* hdp2; };
struct hderived312 : public derived31, public base2 { int hdval312; double* hdp3; };

bool fetch_cond(std::vector<bool>& cond) {
    bool res = true;
    for(auto it=cond.begin(); it != cond.end(); it++) res &= *it;
    return res;
}

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
            nd(int, int); // not-same check
//            nd(int, double);
//            ns(base1, int);
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
