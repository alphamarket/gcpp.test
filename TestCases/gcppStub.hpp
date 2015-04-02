#ifndef GCPPSTUB_HPP
#define GCPPSTUB_HPP

#include "../../gcpp.hpp"

struct base1 { int bval1; double* bp1; virtual ~base1(){ } };
struct base2 { int bval2; double* bp2; virtual ~base2(){ } };
struct base3 { int bval3; double* bp3; virtual ~base3(){ } };

struct derived1 : public base1 { int dval1; double* dp1; };
struct derived2 : public base2 { int dval2; double* dp2; };
struct derived3 : public base3 { int dval3; double* dp3; };

struct derived12 : public base1, public base2 { int dval12; double* dp1; };
struct derived23 : public base2, public base3 { int dval23; double* dp2; };
struct derived31 : public base3, public base1 { int dval31; double* dp3; };

struct hderived123 : public derived12, public base3 { int hdval123; double* hdp1; };
struct hderived231 : public derived23, public base1 { int hdval231; double* hdp2; };
struct hderived312 : public derived31, public base2 { int hdval312; double* hdp3; };


#endif // GCPPSTUB_HPP
