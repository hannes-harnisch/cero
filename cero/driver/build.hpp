#pragma once

#include "cero/report/reporter.hpp"
#include "cero/syntax/source.hpp"

namespace cero {

/// Tries to run the build pipeline on a single source, otherwise reports an error on failure.
void build_source(const Source& source, Reporter& reporter);

} // namespace cero
