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

#include <unordered_set>

#include "../common/constants.hpp"
#include "tool.hpp"
class Item;
class PropertyManager;

class EraserTool : public Tool {
public:
    EraserTool();
    ~EraserTool() override = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void leave(ApplicationContext *context) override;

    void cleanup() override;

    Tool::Type type() const override;

private:
    bool m_isErasing{false};
    QRectF m_lastRect{};

    std::unordered_set<std::shared_ptr<Item>> m_toBeErased;
};
