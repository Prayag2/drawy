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

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
class ApplicationContext;

class ActionManager : public QObject {
    Q_OBJECT
public:
    ActionManager(ApplicationContext *context);

    void zoomIn();
    void zoomOut();
    void undo();
    void redo();
    void increaseThickness();
    void decreaseThickness();
    void switchToFreeformTool();
    void switchToEraserTool();
    void switchToSelectionTool();
    void switchToRectangleTool();
    void switchToEllipseTool();
    void switchToLineTool();
    void switchToArrowTool();
    void switchToMoveTool();
    void switchToTextTool();

private:
    ApplicationContext *m_context;
};

#endif  // ACTIONMANAGER_H
