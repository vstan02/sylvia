/* Copyright (C) 2023 Stan Vlad <vstan02@protonmail.com>
 *
 * This file is part of Sylvia.
 *
 * Sylvia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

#include "core/token.hpp"
#include "core/ast/node.hpp"
#include "core/ast/stmts.hpp"
#include "core/ast/values.hpp"
#include "syntax/lexer.hpp"
#include "syntax/parser.hpp"

#define CODE_STR \
  "def x = 3\n" \
  "def y = 2\n" \
  "\n" \
  "def print_sum\n" \
  " = (x, y)\n" \
  " => writeln(add(x, y))\n" \
  "\n" \
  "def main\n" \
  " = ()\n" \
  " => print_sum(6, 8)\n"

static void print_ast(sylvia::core::ast_nodes<sylvia::core::ast_stmt>&);
static void print_ast(sylvia::core::ast_node<sylvia::core::ast_value>&);

extern int main() {
  using namespace sylvia;
  std::string code = CODE_STR;

  syntax::lexer lexer(code.cbegin(), code.cend());
  core::token token = lexer.scan_next();
  while (token.type != core::token_type::END) {
    std::cout << static_cast<int>(token.type) << " -> '" << token.value << "'\n";
    token = lexer.scan_next();
  }

  syntax::parser parser(code.cbegin(), code.cend());
  auto ast = parser.parse();

  std::cout << "=====" << '\n';
  print_ast(ast);
  return EXIT_SUCCESS;
}

static void print_ast(sylvia::core::ast_nodes<sylvia::core::ast_stmt>& nodes) {
  using namespace sylvia;
  for (const auto& node: nodes) {
    auto stmt = std::get<core::ast_stmt_const>(node->target);
    std::cout << '"' << stmt.name.value << "\" = \"";
    print_ast(stmt.value);
    std::cout << "\"\n";
  }
}

static void print_ast(sylvia::core::ast_node<sylvia::core::ast_value>& node) {
  using namespace sylvia;
  switch (node->type) {
    case core::ast_value_type::BASE: {
      auto target = std::get<core::ast_value_base>(node->target);
      std::cout << target.value.value;
      break;
    }
    case core::ast_value_type::FUNC: {
      auto target = std::get<core::ast_value_func>(node->target);
      std::cout << '(';
      for (auto& arg: target.params) {
        if (&arg != &(*target.params.cbegin()))
          std::cout << ", ";
        std::cout << arg.value;
      }
      std::cout << ") => ";
      print_ast(target.value);
      break;
    }
    default: {
      auto target = std::get<core::ast_value_call>(node->target);
      std::cout << target.name.value << "(";
      for (auto& arg: target.args) {
        if (&arg != &(*target.args.cbegin()))
          std::cout << ", ";
        print_ast(arg);
      }
      std::cout << ")";
    }
  }
}
