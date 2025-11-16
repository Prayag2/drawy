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

#include "uicontext.h"

#include "../canvas/canvas.h"
#include "../command/commandhistory.h"
#include "../common/constants.h"
#include "../common/renderitems.h"
#include "../components/actionbar.h"
#include "../components/propertybar.h"
#include "../components/toolbar.h"
#include "../data-structures/cachegrid.h"
#include "../data-structures/quadtree.h"
#include "../event/event.h"
#include "coordinatetransformer.h"
#include "../keybindings/actionmanager.h"
#include "../keybindings/keybindmanager.h"
#include "../properties/widgets/propertymanager.h"
#include "../serializer/loader.h"
#include "../serializer/serializer.h"
#include "../tools/arrowtool.h"
#include "../tools/ellipsetool.h"
#include "../tools/erasertool.h"
#include "../tools/freeformtool.h"
#include "../tools/linetool.h"
#include "../tools/movetool.h"
#include "../tools/rectangletool.h"
#include "../tools/selectiontool/selectiontool.h"
#include "../tools/texttool.h"
#include "applicationcontext.h"
#include "renderingcontext.h"
#include "selectioncontext.h"
#include "spatialcontext.h"

UIContext::UIContext(ApplicationContext *context)
    : QObject{context},
      m_applicationContext{context} {
}

UIContext::~UIContext() {
    delete m_event;
    qDebug() << "Object deleted: UIContext";
}

void UIContext::setUIContext() {
    m_toolBar = new ToolBar(m_applicationContext->parentWidget());
    m_actionBar = new ActionBar(m_applicationContext->parentWidget());
    m_propertyBar = new PropertyBar(m_applicationContext->parentWidget());
    m_keybindManager = new KeybindManager(&m_applicationContext->renderingContext().canvas());
    m_actionManager = new ActionManager(m_applicationContext);
    m_iconManager = new IconManager(m_applicationContext);

    m_propertyManager = new PropertyManager(m_propertyBar);
    m_propertyBar->setPropertyManager(m_propertyManager);

    QObject::connect(m_propertyManager,
                     &PropertyManager::propertyUpdated,
                     &m_applicationContext->selectionContext(),
                     &SelectionContext::updatePropertyOfSelectedItems);

    m_event = new Event();

    m_toolBar->addTool(new SelectionTool(), Tool::Selection);
    m_toolBar->addTool(new FreeformTool(), Tool::Freeform);
    m_toolBar->addTool(new RectangleTool(), Tool::Rectangle);
    m_toolBar->addTool(new EllipseTool(), Tool::Ellipse);
    m_toolBar->addTool(new ArrowTool(), Tool::Arrow);
    m_toolBar->addTool(new LineTool(), Tool::Line);
    m_toolBar->addTool(new EraserTool(), Tool::Eraser);
    m_toolBar->addTool(new TextTool(), Tool::Text);
    m_toolBar->addTool(new MoveTool(), Tool::Move);

    // TODO: Define their functions somewhere else
    m_actionBar->addButton("Save to File", IconManager::ACTION_SAVE, 6);
    m_actionBar->addButton("Open File", IconManager::ACTION_OPEN_FILE, 7);
    m_actionBar->addButton("Zoom Out", IconManager::ACTION_ZOOM_OUT, 1);
    m_actionBar->addButton("Zoom In", IconManager::ACTION_ZOOM_IN, 2);
    m_actionBar->addButton("Light Mode", IconManager::ACTION_LIGHT_MODE, 3);
    m_actionBar->addButton("Undo", IconManager::ACTION_UNDO, 4);
    m_actionBar->addButton("Redo", IconManager::ACTION_REDO, 5);

    QObject::connect(m_toolBar, &ToolBar::toolChanged, this, &UIContext::toolChanged);
    QObject::connect(m_toolBar,
                     &ToolBar::toolChanged,
                     m_propertyBar,
                     &PropertyBar::updateProperties);

    QObject::connect(&m_actionBar->button(1), &QPushButton::clicked, this, [this]() {
        m_applicationContext->renderingContext().updateZoomFactor(-1);
    });

    QObject::connect(&m_actionBar->button(2), &QPushButton::clicked, this, [this]() {
        m_applicationContext->renderingContext().updateZoomFactor(1);
    });

    QObject::connect(&m_actionBar->button(4), &QPushButton::clicked, this, [this]() {
        m_applicationContext->spatialContext().commandHistory().undo();
        m_applicationContext->renderingContext().markForRender();
        m_applicationContext->renderingContext().markForUpdate();
    });

    QObject::connect(&m_actionBar->button(5), &QPushButton::clicked, this, [this]() {
        m_applicationContext->spatialContext().commandHistory().redo();
        m_applicationContext->renderingContext().markForRender();
        m_applicationContext->renderingContext().markForUpdate();
    });

    QObject::connect(&m_actionBar->button(6), &QPushButton::clicked, this, [this]() {
        Serializer serializer{};

        serializer.serialize(m_applicationContext);
        serializer.saveToFile();
    });

    QObject::connect(&m_actionBar->button(7), &QPushButton::clicked, this, [this]() {
        Loader loader{};

        loader.loadFromFile(m_applicationContext);
    });

    QObject::connect(&m_actionBar->button(3), &QPushButton::clicked, this, [this]() {
        Canvas &canvas{m_applicationContext->renderingContext().canvas()};
        QPushButton &button{actionBar().button(3)};

        if (canvas.bg() == Common::lightBackgroundColor) {
            canvas.setBg(Common::darkBackgroundColor);
            button.setToolTip("Light Mode");
            button.setIcon(iconManager().icon(IconManager::ACTION_LIGHT_MODE));
        } else {
            canvas.setBg(Common::lightBackgroundColor);
            button.setToolTip("Dark Mode");
            button.setIcon(iconManager().icon(IconManager::ACTION_DARK_MODE));
        }

        // Switch colors of all drawn items (black <-> white)
        QVector<std::shared_ptr<Item>> allItems{
            m_applicationContext->spatialContext().quadtree().getAllItems()};
        CacheGrid &cacheGrid{m_applicationContext->spatialContext().cacheGrid()};
        CoordinateTransformer &transformer{
            m_applicationContext->spatialContext().coordinateTransformer()};

        for (auto &item : allItems) {
            try {
                const Property &strokeProp{item->property(Property::StrokeColor)};
                QColor currentColor{strokeProp.value<QColor>()};

                if (Common::isBlackOrWhite(currentColor)) {
                    QColor newColor{Common::invertBlackWhite(currentColor)};
                    item->setProperty(Property::StrokeColor, Property{newColor, Property::StrokeColor});

                    QRect gridDirtyRegion{transformer.worldToGrid(item->boundingBox()).toRect()};
                    cacheGrid.markDirty(gridDirtyRegion);
                }
            } catch (const std::logic_error &) {
                // Item doesn't have StrokeColor property, skip it
            }
        }

        m_applicationContext->renderingContext().markForRender();
        m_applicationContext->renderingContext().markForUpdate();
    });

    m_propertyBar->updateProperties(m_toolBar->curTool());
}

ToolBar &UIContext::toolBar() const {
    return *m_toolBar;
}

PropertyBar &UIContext::propertyBar() const {
    return *m_propertyBar;
}

ActionBar &UIContext::actionBar() const {
    return *m_actionBar;
}

KeybindManager &UIContext::keybindManager() const {
    return *m_keybindManager;
}

ActionManager &UIContext::actionManager() const {
    return *m_actionManager;
}

PropertyManager &UIContext::propertyManager() const {
    return *m_propertyManager;
}

Event &UIContext::event() const {
    return *m_event;
}

IconManager &UIContext::iconManager() const {
    return *m_iconManager;
}

void UIContext::toolChanged(Tool &tool) {
    m_applicationContext->selectionContext().selectedItems().clear();

    Common::renderCanvas(m_applicationContext);

    Canvas &canvas{m_applicationContext->renderingContext().canvas()};

    if (m_lastTool != nullptr)
        m_lastTool->cleanup();

    m_lastTool = &tool;
    canvas.setCursor(tool.cursor());

    m_applicationContext->renderingContext().markForUpdate();
}

void UIContext::reset() {
    m_lastTool = nullptr;
    toolBar().changeTool(Tool::Selection);
}
