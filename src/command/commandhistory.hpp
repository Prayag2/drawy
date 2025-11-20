/*
 * Drawy - A simple brainstorming tool with an infinite canvas
 * Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <deque>
#include <memory>

#include "command.hpp"
class ApplicationContext;

class CommandHistory {
public:
    CommandHistory(ApplicationContext *context);
    ~CommandHistory();

    void undo();
    void redo();
    void insert(std::shared_ptr<Command> command);

    static constexpr int maxCommands{100};  // arbitrary

    void clear();

private:
    std::unique_ptr<std::deque<std::shared_ptr<Command>>> m_undoStack;
    std::unique_ptr<std::deque<std::shared_ptr<Command>>> m_redoStack;

    ApplicationContext *m_context;
};
