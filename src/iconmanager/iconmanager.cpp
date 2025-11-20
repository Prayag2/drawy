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

#include "iconmanager.hpp"

#include <QIcon>

IconManager::IconManager(QObject *parent) : QObject{parent} {
    m_iconName = {{TOOL_SELECTION, "tool-selection"},
                  {TOOL_FREEFORM, "tool-freeform"},
                  {TOOL_RECTANGLE, "tool-rectangle"},
                  {TOOL_ELLIPSE, "tool-ellipse"},
                  {TOOL_ARROW, "tool-arrow"},
                  {TOOL_LINE, "tool-line"},
                  {TOOL_ERASER, "tool-eraser"},
                  {TOOL_TEXT, "tool-text"},
                  {TOOL_MOVE, "tool-move"},
                  {ACTION_SAVE, "action-save"},
                  {ACTION_OPEN_FILE, "action-open_file"},
                  {ACTION_ZOOM_IN, "action-zoom_in"},
                  {ACTION_ZOOM_OUT, "action-zoom_out"},
                  {ACTION_UNDO, "action-undo"},
                  {ACTION_REDO, "action-redo"},
                  {ACTION_DARK_MODE, "action-dark_mode"},
                  {ACTION_LIGHT_MODE, "action-light_mode"},
                  {ACTION_GROUP, "action-group"},
                  {ACTION_UNGROUP, "action-ungroup"},
                  {ACTION_DELETE, "action-delete"},
                  {UI_ANGLE_DOWN, "ui-angle_up"},
                  {UI_ANGLE_UP, "ui-angle_down"}};
}

QIcon IconManager::icon(Icon icon) const {
    QString path{":/icons/" + m_curTheme + "/" + m_iconName.at(icon) + ".svg"};
    return QIcon(path);
}
