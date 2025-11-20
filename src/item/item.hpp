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

#include <QPainter>
#include <QRect>

#include "../properties/property.hpp"

class Item {
public:
    Item();
    virtual ~Item();

    virtual bool intersects(const QRectF &rect) = 0;
    virtual bool intersects(const QLineF &rect) = 0;

    virtual void draw(QPainter &painter, const QPointF &offset) = 0;
    virtual void erase(QPainter &painter, const QPointF &offset) const;

    virtual void translate(const QPointF &amount) = 0;

    virtual const QRectF boundingBox() const;

    void setBoundingBoxPadding(int padding);
    int boundingBoxPadding() const;


    virtual void setProperty(const Property::Type propertyType, Property newObj);
    virtual const Property property(const Property::Type propertyType) const;
    virtual const QVector<Property> properties() const;
    virtual const QVector<Property::Type> propertyTypes() const;

    enum Type { Freeform, Rectangle, Ellipse, Line, Arrow, Text, Group };

    virtual Type type() const = 0;

    virtual void updateAfterProperty();

protected:
    QRectF m_boundingBox{};
    int m_boundingBoxPadding{};
    std::unordered_map<Property::Type, Property> m_properties{};

    virtual void m_draw(QPainter &painter, const QPointF &offset) const = 0;
};
