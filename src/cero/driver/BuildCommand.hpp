#pragma once

#include "cero/driver/CompilerOptions.hpp"
#include "cero/report/Reporter.hpp"
#include "cero/syntax/Source.hpp"

namespace cero {

/// Perform a build with the given options.
bool run_build_command(const CompilerOptions& options);

/// Build a single source input with the given reporter and options.
void try_build_source(const Source& source, Reporter& reporter, const CompilerOptions& options);

} // namespace cero
