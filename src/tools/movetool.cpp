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

#include "movetool.hpp"

#include "../canvas/canvas.hpp"
#include "../common/renderitems.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../context/uicontext.hpp"
#include "../data-structures/quadtree.hpp"
#include "../event/event.hpp"
#include "../item/item.hpp"

MoveTool::MoveTool() {
    m_cursor = QCursor(Qt::OpenHandCursor);
}

void MoveTool::mousePressed(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};
    auto button{uiContext.event().button()};

    if (button == Qt::LeftButton || button == Qt::MiddleButton) {
        SpatialContext &spatialContext{context->spatialContext()};
        RenderingContext &renderingContext{context->renderingContext()};

        m_isActive = true;

        m_initialOffsetPos = spatialContext.offsetPos();
        m_initialPos = uiContext.event().pos();

        renderingContext.canvas().setCursor(Qt::ClosedHandCursor);
    }
};

void MoveTool::mouseMoved(ApplicationContext *context) {
    if (m_isActive) {
        SpatialContext &spatialContext{context->spatialContext()};
        RenderingContext &renderingContext{context->renderingContext()};
        UIContext &uiContext{context->uiContext()};

        qreal zoom{renderingContext.zoomFactor()};
        QPointF newPoint{m_initialOffsetPos * zoom - uiContext.event().pos() + m_initialPos};

        spatialContext.setOffsetPos(newPoint / zoom);

        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
};

void MoveTool::mouseReleased(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};
    auto button{uiContext.event().button()};

    if (button == Qt::LeftButton || button == Qt::MiddleButton) {
        RenderingContext &renderingContext{context->renderingContext()};

        renderingContext.canvas().setCursor(Qt::OpenHandCursor);
        m_isActive = false;
    }
};

Tool::Type MoveTool::type() const {
    return Tool::Move;
}

QString MoveTool::tooltip() const {
    return "Move Tool";
}

IconManager::Icon MoveTool::icon() const {
    return IconManager::TOOL_MOVE;
}
