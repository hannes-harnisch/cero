#pragma once

// TODO: Remove this when updating to doctest 2.4.13
#ifdef CERO_COMPILER_CLANG
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-W#warnings"
#endif
#include <doctest/doctest.h>
#ifdef CERO_COMPILER_CLANG
	#pragma clang diagnostic pop
#endif
