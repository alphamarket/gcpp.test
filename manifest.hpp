#ifndef MANIFEST_HPP
#define	MANIFEST_HPP
#include "hpp/registery.hpp"

#include "TestCases/cppgcTestCaser.hpp"
namespace CPP_TESTER {
    /**
     * bootstrap the test suite for testing
     */
    void __bootstrap() {
        registery::__register("CPPGC Tester", new cppgc_test::cppgcTestCase());
    }
}
#endif	/* MANIFEST_HPP */
