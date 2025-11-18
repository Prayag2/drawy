#pragma once

#include <QString>
#include <QByteArray>

namespace utils::compression {
/**
 * @brief Compresses data with Huffman coding and LZX.
 */
QByteArray compressData(const QByteArray& data);

/**
 * @brief Performs the inverse operation of compressData().
 */
QByteArray decompressData(const QByteArray& data);
}
