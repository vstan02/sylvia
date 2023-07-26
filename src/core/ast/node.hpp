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

#ifndef _SYLVIA_CORE_AST_NODE_HPP_
#define _SYLVIA_CORE_AST_NODE_HPP_

#include <memory>
#include <vector>

namespace sylvia::core {
  template <typename T>
  using ast_node = std::shared_ptr<T>;

  template <typename T>
  using ast_nodes = std::vector<ast_node<T>>;

  template <typename T>
  inline ast_node<T> make_node(T&& value) {
    return std::make_shared<T>(value);
  }
}

#endif // _SYLVIA_CORE_AST_NODE_HPP_
