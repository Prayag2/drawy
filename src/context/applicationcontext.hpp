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
class RenderingContext;
class SpatialContext;
class UIContext;
class SelectionContext;

/**
 * @note: This class was made a Singleton recently, so in many places it is still accessed by
 * passing its pointer instead of using the `instance` method.
 */
class ApplicationContext : public QObject {
public:
    ~ApplicationContext();

    static ApplicationContext *instance(QWidget *parent = nullptr) {
        if (!m_instance) {
            m_instance = new ApplicationContext(parent);
        }

        return m_instance;
    }

    void setContexts();

    QWidget *parentWidget() const;

    RenderingContext &renderingContext() const;
    SpatialContext &spatialContext() const;
    UIContext &uiContext() const;
    SelectionContext &selectionContext() const;

    void reset();  // resets the canvas to a blank state

private:
    ApplicationContext(QWidget *parent = nullptr);

    ApplicationContext(const ApplicationContext &) = delete;
    ApplicationContext(ApplicationContext *) = delete;

    QWidget *m_parentWidget;

    RenderingContext *m_renderingContext{nullptr};
    SpatialContext *m_spatialContext{nullptr};
    UIContext *m_uiContext{nullptr};
    SelectionContext *m_selectionContext{nullptr};

    static ApplicationContext *m_instance;
};
