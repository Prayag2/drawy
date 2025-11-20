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

#include "../tool.hpp"
class SelectionToolState;

class SelectionTool : public Tool {
public:
    SelectionTool();
    ~SelectionTool() = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void keyPressed(ApplicationContext *context) override;

    const QVector<Property::Type> properties() const override;

    Tool::Type type() const override;

private:
    std::shared_ptr<SelectionToolState> getCurrentState(ApplicationContext *context);

    std::shared_ptr<SelectionToolState> m_moveState;
    std::shared_ptr<SelectionToolState> m_selectState;
    std::shared_ptr<SelectionToolState> m_curState;

    bool m_stateLocked{false};
};
