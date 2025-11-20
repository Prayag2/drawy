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

#include "item.hpp"

#include <algorithm>

// PUBLIC
Item::Item() {
}

Item::~Item() {
    qDebug() << "Item deleted: " << m_boundingBox;
}

const QRectF Item::boundingBox() const {
    int mg{m_boundingBoxPadding};
    return m_boundingBox.adjusted(-mg, -mg, mg, mg);
}

void Item::setBoundingBoxPadding(int padding) {
    m_boundingBoxPadding = padding;
}

const Property Item::property(const Property::Type propertyType) const {
    if (m_properties.find(propertyType) == m_properties.end()) {
        throw std::logic_error("Item does not support this property.");
    }

    return m_properties.at(propertyType);
}

const QVector<Property::Type> Item::propertyTypes() const {
    QVector<Property::Type> result;

    for (auto &[type, _] : m_properties) {
        result.push_back(type);
    }

    return result;
}

const QVector<Property> Item::properties() const {
    QVector<Property> result;

    for (auto &[_, property] : m_properties) {
        result.push_back(property);
    }

    return result;
}

void Item::setProperty(const Property::Type propertyType, Property newObj) {
    if (m_properties.find(propertyType) != m_properties.end()) {
        m_properties[propertyType] = newObj;
    }

    updateAfterProperty();
}

void Item::updateAfterProperty() {}
void Item::erase(QPainter &painter, const QPointF &offset) const {}

int Item::boundingBoxPadding() const {
    return m_boundingBoxPadding;
}
