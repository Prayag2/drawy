// NOTE: qt headers MUST be placed before
// kansi otherwise there will be a macro conflict
// fuck headers!!

#include <io/CompressedInputStream.hpp>
#include <io/CompressedOutputStream.hpp>
#include <sstream>

#include "compression.hpp"

namespace utils::compression {
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
    if (n <= 0) break;

    out.write(buffer.data(), n);

    if (cStream.eof()) break;
  }

  cStream.close();

  std::string result = out.str();
  return QByteArray(result.data(), static_cast<int>(result.size()));
}
}
