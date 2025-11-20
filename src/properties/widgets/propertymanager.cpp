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

#include "propertymanager.hpp"

#include "actionswidget.hpp"
#include "erasersizewidget.hpp"
#include "fontsizewidget.hpp"
#include "strokecolorwidget.hpp"
#include "strokewidthwidget.hpp"

PropertyManager::PropertyManager(QWidget *parent) : QObject{parent} {
    m_widgets[Property::StrokeWidth] = new StrokeWidthWidget(parent);
    m_widgets[Property::StrokeColor] = new StrokeColorWidget(parent);
    m_widgets[Property::EraserSize] = new EraserSizeWidget(parent);
    m_widgets[Property::FontSize] = new FontSizeWidget(parent);
    m_widgets[Property::Actions] = new ActionsWidget(parent);

    for (const auto &[_, widget] : m_widgets) {
        QObject::connect(widget, &PropertyWidget::changed, this, &PropertyManager::propertyUpdated);
    }
}

const PropertyWidget &PropertyManager::widget(const Property::Type type) const {
    if (m_widgets.find(type) == m_widgets.end()) {
        throw std::logic_error("A valid widget for the given Property::Type does not exist.");
    }

    return *m_widgets.at(type);
}

Property PropertyManager::value(const Property::Type type) const {
    return widget(type).value();
}
