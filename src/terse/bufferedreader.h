#pragma once

#include <iostream>

namespace terse {

  struct BufferedReader {
    public:
      BufferedReader(std::istream& stream, size_t size = 4096);
      BufferedReader(const BufferedReader& other) = delete;
      BufferedReader(BufferedReader&& other);

      ~BufferedReader();

      BufferedReader& operator=(const BufferedReader& other) = delete;
      BufferedReader& operator=(BufferedReader&& other);

      // Get a character from the buffer. If the buffer is empty than fill the
      // buffer by reading from the `stream` this reader was constructed with.
      // Returns `EOF` if there are no more characters in the buffer or stream.
      char get();

      // "Puts back" a character by decrementing the pointer into the buffer.
      // This is only guaranteed to work once after a call to `get()`.
      void putback() noexcept;

      // Returns `true` if there are no more characters in the buffer or
      // stream.
      bool eof() const noexcept;

      // Assigned `c` the next character read from the reader.
      BufferedReader& operator>>(char& c);

    private:
      // The input stream this reader reads from.
      std::istream* stream_;

      // The size of buffer. This many characters are read from `stream_` at a
      // time.
      size_t size_{};

      // `buf_` points to the beginning of the buffer.
      // `cur_` points to the next character to be read.
      // `end_` points to the end of the filled buffered.
      char *buf_{};
      char *cur_{};
      char *end_{};

      // `true` if there are no more bytes to read from `stream_`.
      bool eof_{false};

      // Reads `size_` bytes from `stream_` into `buf_`.
      // Updates `cur_` to the beginning of buffer, `buf_` and updates `end_`
      // to the end of filled buffer.
      // Sets `eof_` to `true` if there are no more bytes to read from
      // `stream_`.
      void readmore();

      // Returns a size that is aligned such that is suitable for
      // `aligned-alloc`.
      // The returned size will be a power of two greater than
      // `x` and a multiple of `sizeof(void*)`.
      constexpr size_t align(size_t x);
  };

}

//*****************************************************************************
// struct BufferedReader
//*****************************************************************************

inline terse::BufferedReader::BufferedReader(std::istream& stream, size_t size)
  : stream_{&stream}, size_{align(size)}
{
  void *mem = aligned_alloc(size_, size_ * sizeof(char));
  cur_ = end_ = buf_ = static_cast<char*>(mem);
}

inline terse::BufferedReader::BufferedReader(BufferedReader&& other) {
  std::free(buf_);

  stream_ = other.stream_;
  size_ = other.size_;
  buf_ = other.buf_;
  cur_ = other.cur_;
  end_ = other.end_;
  eof_ = other.eof_;

  other.cur_ = other.end_ = other.buf_ = nullptr;
}

inline terse::BufferedReader::~BufferedReader() {
  std::free(buf_);
  buf_ = cur_ = end_ = nullptr;
}

inline terse::BufferedReader& terse::BufferedReader::operator=(BufferedReader&& other) {
  if (this == &other) {
    return *this;
  }

  std::free(buf_);

  stream_ = other.stream_;
  size_ = other.size_;
  buf_ = other.buf_;
  cur_ = other.cur_;
  end_ = other.end_;
  eof_ = other.eof_;

  other.cur_ = other.end_ = other.buf_ = nullptr;

  return *this;
}

inline char terse::BufferedReader::get() {
  if (cur_ == end_) {
    readmore();
    if (eof_) {
      return EOF;
    }
  }

  return *cur_++;
}

inline void terse::BufferedReader::putback() noexcept {
  if (cur_ > buf_) {
    --cur_;
  }
}

inline bool terse::BufferedReader::eof() const noexcept {
  return eof_;
}

inline terse::BufferedReader& terse::BufferedReader::operator>>(char& c) {
  c = get();
  return *this;
}

inline void terse::BufferedReader::readmore() {
  int bytes_read = stream_->readsome(buf_, size_);

  cur_ = buf_;
  end_ = buf_ + bytes_read;

  if (bytes_read == 0) {
    eof_ = true;
  }
}

inline constexpr size_t terse::BufferedReader::align(size_t x) {
  constexpr size_t max{4096 * 8};
  size_t pow{sizeof(void*)};
  for (; pow < x && pow <= max; pow <<= 1);
  return pow;
}
