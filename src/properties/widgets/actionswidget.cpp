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

#include "actionswidget.hpp"
#include "../../context/applicationcontext.hpp"
#include "../../context/uicontext.hpp"
#include "../../keybindings/actionmanager.hpp"
#include "../../iconmanager/iconmanager.hpp"
#include "../property.hpp"
#include <QBoxLayout>
#include <QPushButton>
#include <QWidget>

ActionsWidget::ActionsWidget(QWidget *parent) : PropertyWidget{parent} {
    m_widget = new QWidget{parent};

    QHBoxLayout *layout{new QHBoxLayout(m_widget)};
    layout->setContentsMargins(0, 0, 0, 0);

    QPushButton *deleteButton{new QPushButton("", m_widget)};
    QPushButton *groupButton{new QPushButton("", m_widget)};
    QPushButton *ungroupButton{new QPushButton("", m_widget)};

    IconManager &iconManager{ApplicationContext::instance()->uiContext().iconManager()};
    deleteButton->setIcon(iconManager.icon(IconManager::ACTION_DELETE));
    groupButton->setIcon(iconManager.icon(IconManager::ACTION_GROUP));
    ungroupButton->setIcon(iconManager.icon(IconManager::ACTION_UNGROUP));

    deleteButton->setToolTip("Delete selection");
    groupButton->setToolTip("Group selection");
    ungroupButton->setToolTip("Ungroup selection");

    deleteButton->setProperty("class", "drawyPropertyBarActionButton");
    groupButton->setProperty("class", "drawyPropertyBarActionButton");
    ungroupButton->setProperty("class", "drawyPropertyBarActionButton");

    layout->addWidget(deleteButton);
    layout->addWidget(groupButton);
    layout->addWidget(ungroupButton);

    ActionManager &actionManager{ApplicationContext::instance()->uiContext().actionManager()};

    QObject::connect(deleteButton, &QPushButton::clicked, this, [&](){ actionManager.deleteSelection(); });
    QObject::connect(groupButton, &QPushButton::clicked, this, [&](){ actionManager.groupItems(); });
    QObject::connect(ungroupButton, &QPushButton::clicked, this, [&](){ actionManager.ungroupItems(); });

    m_widget->hide();
}

QString ActionsWidget::name() const {
    return "Actions";
};

const Property ActionsWidget::value() const {
    return Property{};
}
