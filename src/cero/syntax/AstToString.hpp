#pragma once

#include "cero/syntax/Ast.hpp"
#include "cero/syntax/Source.hpp"

namespace cero {

std::string ast_to_string(const Ast& ast, const SourceGuard& source, uint8_t tab_size);

} // namespace cero
