#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hh"

namespace tests {

/// Workaround because Doctest doesn't have an API for this yet. Also, it must be implemented in the file that defines
/// DOCTEST_CONFIG_IMPLEMENT as described in https://github.com/doctest/doctest/issues/345.
const char* get_current_test_name() {
	return doctest::detail::g_cs->currentTest->m_name;
}

} // namespace tests
