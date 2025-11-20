/*
* Drawy - A simple brainstorming tool with an infinite canvas
* Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
*
* Authors:
* 1. quarterstar - quarterstar@proton.me
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

// NOTE: qt headers MUST be placed before
// kansi otherwise there will be a macro conflict
// fuck headers!!

#include "compression.hpp"

#include <io/CompressedInputStream.hpp>
#include <io/CompressedOutputStream.hpp>
#include <sstream>

namespace Common::Utils::Compression {
QByteArray compressData(const QByteArray &data) {
    std::ostringstream stream{std::ios::binary};

    kanzi::CompressedOutputStream cStream{stream, 1, "HUFFMAN", "LZX"};
    cStream.write(data.constData(), static_cast<size_t>(data.size()));
    cStream.close();

    std::string result = stream.str();
    return QByteArray(result.data(), static_cast<int>(result.size()));
}

QByteArray decompressData(const QByteArray &data) {
    std::string compressed{data.constData(), static_cast<size_t>(data.size())};

    std::istringstream stream{compressed, std::ios::binary};

    kanzi::CompressedInputStream cStream{stream, 1, "HUFFMAN", "LZX"};

    std::ostringstream out{std::ios::binary};
    std::vector<char> buffer(4096);

    while (true) {
        cStream.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

        std::streamsize n = cStream.gcount();
        if (n <= 0)
            break;

        out.write(buffer.data(), n);

        if (cStream.eof())
            break;
    }

    cStream.close();

    std::string result = out.str();
    return QByteArray(result.data(), static_cast<int>(result.size()));
}
}  // namespace Common::Utils::Compression
