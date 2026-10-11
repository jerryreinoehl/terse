#pragma once

#include <iostream>

namespace terse {

  struct BufferedReader {
    public:
      BufferedReader(std::istream& stream, size_t size = 4096);

      ~BufferedReader();

      char get();

      void putback();

      bool eof() const noexcept;

      BufferedReader& operator>>(char& c);

    private:
      std::istream& stream_;

      size_t size_{};

      char *buf_{};
      char *cur_{};
      char *end_{};

      bool eof_{false};

      void readmore();
  };

}


//*****************************************************************************
// struct BufferedReader
//*****************************************************************************

inline terse::BufferedReader::BufferedReader(std::istream& stream, size_t size)
  : stream_{stream}, size_{size}
{
  cur_ = end_ = buf_ = new char[size_];
}

inline terse::BufferedReader::~BufferedReader() {
  delete [] buf_;
  buf_ = cur_ = end_ = nullptr;
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

inline void terse::BufferedReader::putback() {
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
  int bytes_read = stream_.readsome(buf_, size_);

  cur_ = buf_;
  end_ = buf_ + bytes_read;

  if (bytes_read == 0) {
    eof_ = true;
  }
}
