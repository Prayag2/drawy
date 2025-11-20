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

#include "updatepropertycommand.hpp"

#include <QRectF>

#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/spatialcontext.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../item/item.hpp"
#include "../properties/property.hpp"

UpdatePropertyCommand::UpdatePropertyCommand(QVector<std::shared_ptr<Item>> items,
                                             Property newProperty)
    : ItemCommand{items},
      m_newProperty{newProperty} {
}

void UpdatePropertyCommand::execute(ApplicationContext *context) {
    Property::Type type{m_newProperty.type()};

    QRectF dirtyRegion{};
    for (auto &item : m_items) {
        try {
            m_properties[item] = item->property(type);
            item->setProperty(type, m_newProperty);
            dirtyRegion |= item->boundingBox();
        } catch (const std::logic_error &e) {
            // Ignore if not found
        }
    }

    QRect gridDirtyRegion{
        context->spatialContext().coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->spatialContext().cacheGrid().markDirty(gridDirtyRegion);
};

void UpdatePropertyCommand::undo(ApplicationContext *context) {
    Property::Type type{m_newProperty.type()};

    QRectF dirtyRegion{};
    for (auto &item : m_items) {
        try {
            item->setProperty(type, m_properties[item]);
            dirtyRegion |= item->boundingBox();
        } catch (const std::logic_error &e) {
            // Ignore if not found
        }
    }

    QRect gridDirtyRegion{
        context->spatialContext().coordinateTransformer().worldToGrid(dirtyRegion).toRect()};
    context->spatialContext().cacheGrid().markDirty(gridDirtyRegion);
};
