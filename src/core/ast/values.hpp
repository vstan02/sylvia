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

#ifndef _SYLVIA_CORE_AST_VALUES_HPP_
#define _SYLVIA_CORE_AST_VALUES_HPP_

#include <variant>

#include "core/token.hpp"
#include "core/ast/node.hpp"

namespace sylvia::core {
  struct ast_value;

  enum class ast_value_type {
    BASE,
    CALL,
    FUNC
  };

  struct ast_value_base {
    token value;

    ast_value_base(const token& value)
      : value(value) {}
  };

  struct ast_value_call {
    token name;
    ast_nodes<ast_value> args;

    ast_value_call(
      const token& name,
      const ast_nodes<ast_value>& args
    ) : name(name)
      , args(args) {}
  };

  struct ast_value_func {
    ast_node<ast_value> value;
    std::vector<token> params;

    ast_value_func(
      const ast_node<ast_value>& value,
      const std::vector<token> params
    ) : value(value)
      , params(params) {}
  };

  struct ast_value {
    ast_value_type type;
    std::variant<
      ast_value_base,
      ast_value_call,
      ast_value_func
    > target;

    ast_value(const ast_value_base& target)
      : type(ast_value_type::BASE)
      , target(target) {}

    ast_value(const ast_value_call& target)
      : type(ast_value_type::CALL)
      , target(target) {}

    ast_value(const ast_value_func& target)
      : type(ast_value_type::FUNC)
      , target(target) {}
  };
}

#endif // _SYLVIA_CORE_AST_VALUES_HPP_
