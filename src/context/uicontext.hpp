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

#include <QWidget>
class ToolBar;
class PropertyBar;
class ActionBar;
class Event;
class PropertyManager;
class Tool;
class ApplicationContext;
class KeybindManager;
class ActionManager;
class IconManager;

class UIContext : public QObject {
public:
    UIContext(ApplicationContext *context);
    ~UIContext();

    void setUIContext();

    ToolBar &toolBar() const;
    PropertyBar &propertyBar() const;
    ActionBar &actionBar() const;
    Event &event() const;
    KeybindManager &keybindManager() const;
    ActionManager &actionManager() const;
    PropertyManager &propertyManager() const;
    IconManager &iconManager() const;

    void reset();

public slots:
    void toolChanged(Tool &);

private:
    ToolBar *m_toolBar{nullptr};
    PropertyBar *m_propertyBar{};
    ActionBar *m_actionBar{};
    PropertyManager *m_propertyManager{};
    KeybindManager *m_keybindManager{};
    ActionManager *m_actionManager{};
    Event *m_event{nullptr};
    IconManager *m_iconManager{};

    Tool *m_lastTool{nullptr};  // Used to call the cleanup function of the last tool

    ApplicationContext *m_applicationContext;
};
