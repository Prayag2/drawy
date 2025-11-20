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

#include "polygondrawingtool.hpp"

#include <memory>
#include "../command/selectcommand.hpp"
#include "../canvas/canvas.hpp"
#include "../command/commandhistory.hpp"
#include "../command/insertitemcommand.hpp"
#include "../common/renderitems.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../context/uicontext.hpp"
#include "../event/event.hpp"
#include "../item/factory/itemfactory.hpp"
#include "../item/polygon.hpp"
#include "../properties/widgets/propertymanager.hpp"

PolygonDrawingTool::PolygonDrawingTool() {
    m_cursor = QCursor(Qt::CrossCursor);

    m_properties = {Property::StrokeWidth, Property::StrokeColor};
}

void PolygonDrawingTool::mousePressed(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton) {
        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};

        curItem = std::dynamic_pointer_cast<PolygonItem>(m_itemFactory->create());

        curItem->setProperty(Property::StrokeWidth,
                             uiContext.propertyManager().value(Property::StrokeWidth));
        curItem->setProperty(Property::StrokeColor,
                             uiContext.propertyManager().value(Property::StrokeColor));

        curItem->setBoundingBoxPadding(10 * renderingContext.canvas().scale());
        curItem->setStart(transformer.viewToWorld(uiContext.event().pos()));

        qreal zoom{renderingContext.zoomFactor()};

        QPainter &painter{renderingContext.overlayPainter()};
        painter.save();
        painter.scale(zoom, zoom);

        m_isDrawing = true;
    }
};

void PolygonDrawingTool::mouseMoved(ApplicationContext *context) {
    if (m_isDrawing) {
        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};
        UIContext &uiContext{context->uiContext()};

        QPainter &overlayPainter{renderingContext.overlayPainter()};

        QPointF offsetPos{spatialContext.offsetPos()};
        curItem->erase(overlayPainter, offsetPos);
        curItem->setEnd(transformer.viewToWorld(uiContext.event().pos()));
        curItem->draw(overlayPainter, offsetPos);

        renderingContext.markForUpdate();
    }
};

void PolygonDrawingTool::mouseReleased(ApplicationContext *context) {
    UIContext &uiContext{context->uiContext()};

    if (uiContext.event().button() == Qt::LeftButton && m_isDrawing) {
        SpatialContext &spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext.coordinateTransformer()};
        RenderingContext &renderingContext{context->renderingContext()};
        CommandHistory &commandHistory{spatialContext.commandHistory()};

        QVector<std::shared_ptr<Item>> itemVector{curItem};
        commandHistory.insert(std::make_shared<InsertItemCommand>(itemVector));

        QPainter &overlayPainter{renderingContext.overlayPainter()};
        renderingContext.canvas().overlay()->fill(Qt::transparent);
        overlayPainter.restore();

        m_isDrawing = false;

        renderingContext.markForRender();
        renderingContext.markForUpdate();
    }
};

void PolygonDrawingTool::cleanup() {
    ApplicationContext *context{ApplicationContext::instance()};
    context->uiContext().event().setButton(Qt::LeftButton);
    mouseReleased(context);
}

Tool::Type PolygonDrawingTool::type() const {
    return Tool::PolygonDrawing;
}
