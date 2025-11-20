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

#include <QObject>
#include <unordered_map>

class IconManager : public QObject {
public:
    IconManager(QObject *parent = nullptr);

    enum Icon {
        TOOL_SELECTION,
        TOOL_FREEFORM,
        TOOL_RECTANGLE,
        TOOL_ELLIPSE,
        TOOL_ARROW,
        TOOL_LINE,
        TOOL_ERASER,
        TOOL_TEXT,
        TOOL_MOVE,
        ACTION_SAVE,
        ACTION_OPEN_FILE,
        ACTION_ZOOM_IN,
        ACTION_ZOOM_OUT,
        ACTION_UNDO,
        ACTION_REDO,
        ACTION_DARK_MODE,
        ACTION_LIGHT_MODE,
        ACTION_GROUP,
        ACTION_UNGROUP,
        ACTION_DELETE,
        UI_ANGLE_UP,
        UI_ANGLE_DOWN,
    };

    QIcon icon(Icon icon) const;

private:
    std::unordered_map<Icon, QString> m_iconName;

    QString m_curTheme{"fa-dark"};  // default
};
