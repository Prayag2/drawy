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

#include "actionbar.hpp"

#include <stdexcept>

#include "../context/applicationcontext.hpp"
#include "../context/uicontext.hpp"

ActionBar::ActionBar(QWidget *parent) : QFrame{parent} {
    m_layout = new QHBoxLayout{this};
    m_layout->setSpacing(10);
    this->setLayout(m_layout);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void ActionBar::addButton(const QString &tooltip, IconManager::Icon icon, int id) {
    if (m_map.contains(id)) {
        throw std::logic_error("Button with same id exists in the ActionBar.");
    }

    ApplicationContext *context{ApplicationContext::instance()};

    m_map[id] = new QPushButton{"", this};
    m_map[id]->setIcon(context->uiContext().iconManager().icon(icon));
    m_map[id]->setToolTip(tooltip);

    m_map[id]->setProperty("class", "drawlyActionButton");
    m_map[id]->setCursor(Qt::PointingHandCursor);
    m_layout->addWidget(m_map[id]);
}

QPushButton &ActionBar::button(int id) {
    if (!m_map.contains(id)) {
        throw std::logic_error(
            QString::asprintf("Button with id %d was not found.", id).toStdString());
    }
    return *m_map[id];
}
