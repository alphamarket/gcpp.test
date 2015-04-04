#ifndef MANIFEST_HPP
#define	MANIFEST_HPP
#include "hpp/registery.hpp"

#include "TestCases/gcppTestCaser.hpp"
namespace CPP_TESTER {
    /**
     * bootstrap the test suite for testing
     */
    void __bootstrap() {
        registery::__register("GCPP Tester", new cppgc_test::gcppTestCase());
    }
}
#endif	/* MANIFEST_HPP */
