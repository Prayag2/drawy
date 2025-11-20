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

#include "commandhistory.hpp"

#include <QDebug>

CommandHistory::CommandHistory(ApplicationContext *context) : m_context{context} {
    m_undoStack = std::make_unique<std::deque<std::shared_ptr<Command>>>();
    m_redoStack = std::make_unique<std::deque<std::shared_ptr<Command>>>();
}

CommandHistory::~CommandHistory() {
    qDebug() << "Object deleted: CommandHistory";
}

void CommandHistory::undo() {
    if (m_undoStack->empty())
        return;

    std::shared_ptr<Command> lastCommand{m_undoStack->front()};
    lastCommand->undo(m_context);

    m_redoStack->push_front(lastCommand);

    if (m_redoStack->size() == maxCommands)
        m_redoStack->pop_back();

    m_undoStack->pop_front();
}

void CommandHistory::redo() {
    if (m_redoStack->empty())
        return;

    std::shared_ptr<Command> nextCommand{m_redoStack->front()};
    nextCommand->execute(m_context);

    m_undoStack->push_front(nextCommand);
    if (m_undoStack->size() == maxCommands)
        m_undoStack->pop_back();

    m_redoStack->pop_front();
}

void CommandHistory::insert(std::shared_ptr<Command> command) {
    while (!m_redoStack->empty()) {
        m_redoStack->pop_front();
    }

    command->execute(m_context);

    m_undoStack->push_front(command);
    if (m_undoStack->size() == maxCommands)
        m_undoStack->pop_back();
}

void CommandHistory::clear() {
    m_undoStack->clear();
    m_redoStack->clear();
}
