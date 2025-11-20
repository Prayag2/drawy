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

#include <QCursor>

#include "../iconmanager/iconmanager.hpp"
#include "../properties/property.hpp"
class ApplicationContext;

// INTERFACE
class Tool {
public:
    Tool();
    virtual ~Tool();

    const QCursor &cursor() const;
    virtual QString tooltip() const = 0;

    virtual void mousePressed(ApplicationContext *context);
    virtual void mouseMoved(ApplicationContext *context);
    virtual void mouseReleased(ApplicationContext *context);
    virtual void keyPressed(ApplicationContext *context);
    virtual void keyReleased(ApplicationContext *context);
    virtual void mouseDoubleClick(ApplicationContext *context);
    virtual void mouseTripleClick(ApplicationContext *context);
    virtual void leave(ApplicationContext *context);

    virtual void cleanup();

    enum Type {
        Selection,
        Freeform,
        PolygonDrawing,
        Eraser,
        Move,
        Rectangle,
        Ellipse,
        Line,
        Arrow,
        Text
    };

    virtual Type type() const = 0;
    virtual IconManager::Icon icon() const = 0;

    virtual const QVector<Property::Type> properties() const;

protected:
    QCursor m_cursor{};
    QVector<Property::Type> m_properties;
};
