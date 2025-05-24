#pragma once

#include "cero/report/reporter.hh"
#include "cero/syntax/source.hh"

namespace cero {

/// Tries to run the build pipeline on a single source, otherwise reports an error on failure.
void build_source(const Source& source, Reporter& reporter);

} // namespace cero
