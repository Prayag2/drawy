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

#include "applicationcontext.hpp"

#include "coordinatetransformer.hpp"
#include "renderingcontext.hpp"
#include "selectioncontext.hpp"
#include "spatialcontext.hpp"
#include "uicontext.hpp"

ApplicationContext *ApplicationContext::m_instance = nullptr;

ApplicationContext::ApplicationContext(QWidget *parent) : QObject{parent}, m_parentWidget{parent} {
    m_selectionContext = new SelectionContext(this);
    m_renderingContext = new RenderingContext(this);
    m_uiContext = new UIContext(this);
    m_spatialContext = new SpatialContext(this);
}

ApplicationContext::~ApplicationContext() {
    qDebug() << "Object deleted: ApplicationContext";
}

void ApplicationContext::setContexts() {
    m_renderingContext->setRenderingContext();
    m_uiContext->setUIContext();
    m_spatialContext->setSpatialContext();
    m_spatialContext->coordinateTransformer().setCoordinateTransformer();
}

QWidget *ApplicationContext::parentWidget() const {
    return m_parentWidget;
}

RenderingContext &ApplicationContext::renderingContext() const {
    return *m_renderingContext;
}

SpatialContext &ApplicationContext::spatialContext() const {
    return *m_spatialContext;
}

UIContext &ApplicationContext::uiContext() const {
    return *m_uiContext;
}

SelectionContext &ApplicationContext::selectionContext() const {
    return *m_selectionContext;
}

void ApplicationContext::reset() {
    selectionContext().reset();
    uiContext().reset();
    selectionContext().reset();
    spatialContext().reset();
    renderingContext().reset();
}
