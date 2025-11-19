#pragma once

#include <QByteArray>
#include <QString>

namespace Common::Utils::Compression {
/**
 * @brief Compresses data with Huffman coding and LZX.
 */
QByteArray compressData(const QByteArray &data);

/**
 * @brief Performs the inverse operation of compressData().
 */
QByteArray decompressData(const QByteArray &data);
}  // namespace Common::Utils::Compression
