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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QColor>
#include <QMargins>
#include <QSize>
#include <array>

namespace Common {
constexpr QMargins cleanupMargin{10, 10, 10, 10};  // in pixels
constexpr int defaultEraserSize{30};

constexpr QColor eraserBorderColor{255, 0, 0};
constexpr QColor eraserBackgroundColor{255, 0, 0, 50};
constexpr int eraserBorderWidth{2};

constexpr QColor selectionBorderColor{67, 135, 244, 255};
constexpr QColor selectionBackgroundColor{67, 135, 244, 50};

constexpr unsigned int erasedItemColor{0x6E6E6E96};

constexpr QColor lightBackgroundColor{248, 249, 250};
constexpr QColor darkBackgroundColor{18, 18, 18};

constexpr QColor lightForegroundColor{30, 30, 30};
constexpr QColor darkForegroundColor{211, 211, 211};

constexpr int maxItemOpacity{255};
constexpr int eraseItemOpacity{50};

constexpr int translationDelta{1};        // in pixels
constexpr int shiftTranslationDelta{10};  // in pixels, when holding shift

constexpr double defaultTextBoxWidth{5};  // in pixels
constexpr std::array<QChar, 38> wordSeparators{
    '\t', '\n', '\v', '\f', '\r', ' ',  // whitespace
    '!',  '"',  '#',  '$',  '%',  '&', '\'', '(',  ')', '*', '+', ',', '-', '.', '/', ':',
    ';',  '<',  '=',  '>',  '?',  '@', '[',  '\\', ']', '^', '_', '`', '{', '|', '}', '~'};

constexpr int doubleClickInterval{300};  // milliseconds

constexpr qreal tabStopDistance{4};

// Helper functions for color management
inline bool isBlackOrWhite(const QColor &color) {
    // Check if color is near black or near white (within tolerance of 5)
    // This handles Qt's color conversions that may not produce exact values
    int r = color.red(), g = color.green(), b = color.blue();
    bool isNearBlack = (r <= 5 && g <= 5 && b <= 5);
    bool isNearWhite = (r >= 250 && g >= 250 && b >= 250);
    return isNearBlack || isNearWhite;
}

inline QColor invertBlackWhite(const QColor &color) {
    // Invert near-black to white and near-white to black, preserving alpha
    int r = color.red(), g = color.green(), b = color.blue();
    if (r <= 5 && g <= 5 && b <= 5) {
        return QColor(255, 255, 255, color.alpha());
    } else if (r >= 250 && g >= 250 && b >= 250) {
        return QColor(0, 0, 0, color.alpha());
    }
    return color;
}

};  // namespace Common

#endif  // CONSTANTS_H
