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

#include "toolbar.hpp"

#include "../context/applicationcontext.hpp"
#include "../context/uicontext.hpp"
#include "../iconmanager/iconmanager.hpp"

ToolBar::ToolBar(QWidget *parent) : QFrame{parent} {
    m_group = new QButtonGroup(this);
    m_layout = new QHBoxLayout(this);
    this->setLayout(m_layout);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);
    this->setAutoFillBackground(true);
    this->setProperty("class", "drawlyFrame drawlyToolBar");

    QObject::connect(m_group, &QButtonGroup::idClicked, this, &ToolBar::onToolChanged);
}

ToolBar::~ToolBar() {
    for (auto &idToolPair : m_tools) {
        delete idToolPair.second;
    }
}

Tool &ToolBar::curTool() const {
    int curID{m_group->checkedId()};

    if (m_tools.find(curID) == m_tools.end())
        throw std::logic_error("Trying to access non existent tool");

    return *m_tools.at(curID);
}

QVector<Tool *> ToolBar::tools() const {
    QVector<Tool *> result;
    for (auto &idToolPair : m_tools) {
        result.push_back(idToolPair.second);
    }

    return result;
}

void ToolBar::addTool(Tool *tool, Tool::Type type) {
    if (tool == nullptr)
        return;

    ApplicationContext *context{ApplicationContext::instance()};
    QPushButton *btn{new QPushButton("", this)};
    btn->setIcon(context->uiContext().iconManager().icon(tool->icon()));

    btn->setCheckable(true);
    btn->setProperty("class", "drawlyToolButton");
    btn->setCursor(Qt::PointingHandCursor);

    int id{static_cast<int>(type)};

    m_tools[id] = tool;
    m_group->addButton(btn, id);
    m_layout->addWidget(btn);
    if (m_tools.size() == 1) {
        m_group->button(id)->setChecked(true);
        emit toolChanged(*tool);
    }
};

void ToolBar::changeTool(Tool::Type type) {
    int id{static_cast<int>(type)};

    m_group->button(id)->setChecked(true);
    emit toolChanged(curTool());
}

Tool &ToolBar::tool(Tool::Type type) const {
    return *m_tools.at(type);
}

// PRIVATE SLOTS
void ToolBar::onToolChanged(int id) {
    emit toolChanged(curTool());
}
