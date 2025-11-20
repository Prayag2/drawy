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

#include "../item/text.hpp"
#include "drawingtool.hpp"
class PropertyManager;

class TextTool : public DrawingTool {
public:
    TextTool();
    ~TextTool() override = default;

    void mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    void mouseReleased(ApplicationContext *context) override;
    void keyPressed(ApplicationContext *context) override;
    void keyReleased(ApplicationContext *context) override;
    void mouseDoubleClick(ApplicationContext *context) override;
    void mouseTripleClick(ApplicationContext *context) override;

    void cleanup() override;

    Tool::Type type() const override;
    QString tooltip() const override;
    IconManager::Icon icon() const override;

private:
    std::shared_ptr<TextItem> m_curItem{nullptr};

    bool m_isSelecting{false};
    bool m_mouseMoved{false};
    bool m_doubleClicked{false};
    bool m_tripleClicked{false};
};
