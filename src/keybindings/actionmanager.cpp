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

#include "actionmanager.hpp"
#include "../command/selectcommand.hpp"
#include "../command/deselectcommand.hpp"
#include "../command/groupcommand.hpp"
#include "../command/ungroupcommand.hpp"
#include "../command/commandhistory.hpp"
#include "../command/removeitemcommand.hpp"
#include "../components/propertybar.hpp"
#include "../components/toolbar.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/coordinatetransformer.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/selectioncontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../context/uicontext.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../data-structures/quadtree.hpp"
#include "../serializer/loader.hpp"
#include "../serializer/serializer.hpp"
#include "action.hpp"
#include "keybindmanager.hpp"
#include <memory>

ActionManager::ActionManager(ApplicationContext *context) : m_context{context}, QObject(context) {
    KeybindManager &keybindManager{m_context->uiContext().keybindManager()};

    Action *undoAction{new Action{"Undo", "Undo last action", [&]() { this->undo(); }, context}};

    Action *redoAction{
        new Action{"Redo", "Redo last undone action", [&]() { this->redo(); }, context}};

    Action *zoomInAction{new Action{"Zoom In", "Zoom in", [&]() { this->zoomIn(); }, context}};

    Action *zoomOutAction{new Action{"Zoom Out", "Zoom out", [&]() { this->zoomOut(); }, context}};

    Action *increaseThicknessAction{new Action{"Increase Thickness",
                                               "Increase brush thickness",
                                               [&]() { this->increaseThickness(); },
                                               context}};

    Action *decreaseThicknessAction{new Action{"Decrease Thickness",
                                               "Decrease brush thickness",
                                               [&]() { this->decreaseThickness(); },
                                               context}};

    Action *freeformToolAction{new Action{"Freeform Tool",
                                          "Switch to freeform drawing tool",
                                          [&]() { this->switchToFreeformTool(); },
                                          context}};

    Action *eraserToolAction{new Action{"Eraser Tool",
                                        "Switch to eraser tool",
                                        [&]() { this->switchToEraserTool(); },
                                        context}};

    Action *selectionToolAction{new Action{"Selection Tool",
                                           "Switch to selection tool",
                                           [&]() { this->switchToSelectionTool(); },
                                           context}};

    Action *rectangleToolAction{new Action{"Rectangle Tool",
                                           "Switch to rectangle drawing tool",
                                           [&]() { this->switchToRectangleTool(); },
                                           context}};

    Action *ellipseToolAction{new Action{"Ellipse Tool",
                                         "Switch to ellipse drawing tool",
                                         [&]() { this->switchToEllipseTool(); },
                                         context}};

    Action *lineToolAction{new Action{"Line Tool",
                                      "Switch to line drawing tool",
                                      [&]() { this->switchToLineTool(); },
                                      context}};

    Action *textToolAction{new Action{"Text Tool",
                                      "Switch to the text tool",
                                      [&]() { this->switchToTextTool(); },
                                      context}};

    Action *arrowToolAction{new Action{"Arrow Tool",
                                       "Switch to arrow drawing tool",
                                       [&]() { this->switchToArrowTool(); },
                                       context}};

    Action *moveToolAction{new Action{"Move Tool",
                                      "Switch to move tool",
                                      [&]() { this->switchToMoveTool(); },
                                      context}};

    Action *groupAction{new Action{"Group Elements",
                                      "Groups selected items",
                                      [&]() { this->groupItems(); },
                                      context}};

    Action *unGroupAction{new Action{"Ungroup Elements",
                                      "Ungroups selected groups",
                                      [&]() { this->ungroupItems(); },
                                      context}};

    Action *selectAllAction{new Action{
        "Select All",
        "Select all items",
        [&, context]() { this->selectAll(); },
        context}};

    Action *deleteAction{new Action{
        "Delete",
        "Deletes selected items",
        [&, context]() { this->deleteSelection(); },
        context}};

    Action *saveAction{new Action{"Save",
                                  "Save canvas",
                                  [&, context]() { this->saveToFile(); },
                                  context}};

    Action *openFileAction{new Action{"Open File",
                                      "Open an existing file",
                                      [&, context]() { this->loadFromFile(); },
                                      context}};

    keybindManager.addKeybinding(undoAction, "Ctrl+Z");
    keybindManager.addKeybinding(redoAction, "Ctrl+Y");
    keybindManager.addKeybinding(redoAction, "Ctrl+Shift+Z");
    keybindManager.addKeybinding(zoomInAction, "Ctrl++");
    keybindManager.addKeybinding(zoomOutAction, "Ctrl+-");
    keybindManager.addKeybinding(textToolAction, "T");
    keybindManager.addKeybinding(freeformToolAction, "P");
    keybindManager.addKeybinding(freeformToolAction, "B");
    keybindManager.addKeybinding(eraserToolAction, "E");
    keybindManager.addKeybinding(selectionToolAction, "S");
    keybindManager.addKeybinding(rectangleToolAction, "R");
    keybindManager.addKeybinding(ellipseToolAction, "O");
    keybindManager.addKeybinding(lineToolAction, "L");
    keybindManager.addKeybinding(arrowToolAction, "A");
    keybindManager.addKeybinding(moveToolAction, "M");
    keybindManager.addKeybinding(selectAllAction, "Ctrl+A");
    keybindManager.addKeybinding(deleteAction, "Delete");
    keybindManager.addKeybinding(saveAction, "Ctrl+S");
    keybindManager.addKeybinding(openFileAction, "Ctrl+O");
    keybindManager.addKeybinding(groupAction, "Ctrl+G");
    keybindManager.addKeybinding(unGroupAction, "Ctrl+Shift+G");
}

void ActionManager::undo() {
    m_context->spatialContext().commandHistory().undo();
    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();
}

void ActionManager::redo() {
    m_context->spatialContext().commandHistory().redo();
    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();
}

void ActionManager::zoomIn() {
    m_context->renderingContext().updateZoomFactor(1);
}

void ActionManager::zoomOut() {
    m_context->renderingContext().updateZoomFactor(-1);
}

void ActionManager::switchToFreeformTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Freeform);
}

void ActionManager::switchToEraserTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Eraser);
}

void ActionManager::switchToRectangleTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Rectangle);
}

void ActionManager::switchToEllipseTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Ellipse);
}

void ActionManager::switchToLineTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Line);
}

void ActionManager::switchToArrowTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Arrow);
}

void ActionManager::switchToMoveTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Move);
}

void ActionManager::switchToSelectionTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Selection);
}

void ActionManager::switchToTextTool() {
    m_context->uiContext().toolBar().changeTool(Tool::Text);
}

void ActionManager::groupItems() {
    auto &selectedItems{m_context->selectionContext().selectedItems()};
    if (selectedItems.size() <= 1)
        return;

    QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext().commandHistory().insert(std::make_shared<GroupCommand>(items));
    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();
}

void ActionManager::ungroupItems() {
    auto &selectedItems{m_context->selectionContext().selectedItems()};
    if (selectedItems.empty())
        return;

    QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext().commandHistory().insert(std::make_shared<UngroupCommand>(items));
    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();
}

void ActionManager::deleteSelection() {
    auto &selectedItems{m_context->selectionContext().selectedItems()};
    auto &transformer{m_context->spatialContext().coordinateTransformer()};
    auto &commandHistory{m_context->spatialContext().commandHistory()};

    QVector<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    commandHistory.insert(std::make_shared<RemoveItemCommand>(items));

    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();

    QVector<std::shared_ptr<Item>> selectedItemsVector{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext().commandHistory().insert(
        std::make_shared<DeselectCommand>(selectedItemsVector)
    );
}

void ActionManager::selectAll() {
    this->switchToSelectionTool();

    auto allItems{m_context->spatialContext().quadtree().getAllItems()};
    m_context->spatialContext().commandHistory().insert(
        std::make_shared<SelectCommand>(allItems)
    );

    m_context->uiContext().propertyBar().updateToolProperties();
    m_context->renderingContext().markForRender();
    m_context->renderingContext().markForUpdate();
}

void ActionManager::saveToFile() {
    Serializer serializer{};

    serializer.serialize(m_context);
    serializer.saveToFile();
}

void ActionManager::loadFromFile() {
    Loader loader{};
    loader.loadFromFile(m_context);
}

void ActionManager::increaseThickness() {
    // TODO: implement
}

void ActionManager::decreaseThickness() {
    // TODO: implement
}
