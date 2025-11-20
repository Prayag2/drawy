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

#include "controller.hpp"

#include <QDateTime>
#include <QWheelEvent>

#include "../canvas/canvas.hpp"
#include "../common/constants.hpp"
#include "../common/renderitems.hpp"
#include "../components/toolbar.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../context/uicontext.hpp"
#include "../event/event.hpp"
#include "../tools/tool.hpp"

Controller::Controller(QObject *parent) : QObject{parent} {
    m_context = ApplicationContext::instance(dynamic_cast<QWidget *>(parent));
    m_context->setContexts();
}

Controller::~Controller() {
    qDebug() << "Object deleted: Controller";
}

void Controller::mousePressed(QMouseEvent *event) {
    // No on really clicks in this corner (0, 0) and this solves a
    // bug on Hyprland where it would register a mouse press in this corner
    if (event->pos() == QPoint{0, 0})
        return;

    qint64 lastTime{m_lastClickTime};
    m_lastClickTime = QDateTime::currentMSecsSinceEpoch();
    if (m_lastClickTime - lastTime <= Common::doubleClickInterval && !m_mouseMoved) {
        m_clickCount++;

        if (m_clickCount == 2) {
            mouseDoubleClick(event);
        } else if (m_clickCount == 3) {
            mouseTripleClick(event);
        }
        return;
    } else {
        m_clickCount = 1;
        m_mouseMoved = false;
    }

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    if (event->button() == Qt::MiddleButton) {
        m_movingWithMiddleClick = true;
        toolBar.curTool().cleanup();
        toolBar.tool(Tool::Move).mousePressed(m_context);
        return;
    }

    toolBar.curTool().mousePressed(m_context);

    if (event->type() != QEvent::TabletPress) {
        contextEvent.setPressure(1.0);
    }
}

void Controller::mouseDoubleClick(QMouseEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    toolBar.curTool().mouseDoubleClick(m_context);
}

void Controller::mouseTripleClick(QMouseEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    toolBar.curTool().mouseTripleClick(m_context);
}

void Controller::mouseMoved(QMouseEvent *event) {
    m_mouseMoved = true;

    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    if (m_movingWithMiddleClick) {
        toolBar.tool(Tool::Move).mouseMoved(m_context);
        return;
    }

    toolBar.curTool().mouseMoved(m_context);
}

void Controller::mouseReleased(QMouseEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    contextEvent.setPos(event->pos(), canvas.scale());
    contextEvent.setButton(event->button());
    contextEvent.setModifiers(event->modifiers());

    if (event->button() == Qt::MiddleButton) {
        m_movingWithMiddleClick = false;
        toolBar.tool(Tool::Move).mouseReleased(m_context);
        canvas.setCursor(toolBar.curTool().cursor());
        return;
    }

    toolBar.curTool().mouseReleased(m_context);
}

void Controller::tablet(QTabletEvent *event) {
    Event &ev{m_context->uiContext().event()};

    // TODO: Remove magic numbers
    ev.setPressure(event->pressure() / 1.60 + 0.375);
}

void Controller::keyPressed(QKeyEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};

    contextEvent.setKey(event->key());
    contextEvent.setModifiers(event->modifiers());
    contextEvent.setText(event->text());

    toolBar.curTool().keyPressed(m_context);
}

void Controller::keyReleased(QKeyEvent *event) {
    Event &contextEvent{m_context->uiContext().event()};
    ToolBar &toolBar{m_context->uiContext().toolBar()};

    contextEvent.setKey(event->key());
    contextEvent.setModifiers(event->modifiers());
    contextEvent.setText(event->text());

    toolBar.curTool().keyReleased(m_context);
}

void Controller::inputMethodInvoked(QInputMethodEvent *event) {
}

void Controller::leave(QEvent *event) {
    ToolBar &toolBar{m_context->uiContext().toolBar()};

    toolBar.curTool().leave(m_context);
}

void Controller::wheel(QWheelEvent *event) {
    const QPointF &offsetPos{m_context->spatialContext().offsetPos()};
    const qreal zoomFactor{m_context->renderingContext().zoomFactor()};
    Canvas &canvas{m_context->renderingContext().canvas()};

    if (event->modifiers() & Qt::ControlModifier) {
        int delta{event->angleDelta().y() > 0 ? 1 : -1};
        m_context->renderingContext().updateZoomFactor(delta);
        return;
    }

    m_context->spatialContext().setOffsetPos(offsetPos - event->angleDelta() / zoomFactor);

    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();
}
