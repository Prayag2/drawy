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

#include "strokewidthwidget.hpp"

#include <QSpinBox>

#include "../property.hpp"

StrokeWidthWidget::StrokeWidthWidget(QWidget *parent) : PropertyWidget{parent} {
    QSpinBox *box{new QSpinBox(parent)};
    box->setRange(1, 10);
    box->setValue(6);

    box->hide();
    m_widget = box;

    QObject::connect(box, &QSpinBox::valueChanged, this, [this]() { emit changed(value()); });
}

QString StrokeWidthWidget::name() const {
    return "Thickness";
};

const Property StrokeWidthWidget::value() const {
    return Property{dynamic_cast<QSpinBox *>(m_widget)->value(), Property::StrokeWidth};
}
