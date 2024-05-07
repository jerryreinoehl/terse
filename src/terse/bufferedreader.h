#pragma once

#include <iostream>

namespace terse {

  class BufferedReader {
    public:
      BufferedReader(std::istream& stream, size_t size = 4096)
        : stream_{stream}, size_{size}, buf_{new char[size_]}
      {
        readmore();
      }

      ~BufferedReader() {
        delete [] buf_;
        buf_ = cur_ = end_ = nullptr;
      }

      char getchar() {
        char c = *cur_++;

        if (cur_ == end_) {
          readmore();
        }

        return c;
      }

      bool eof() const noexcept {
        return eof_;
      }

      BufferedReader& operator>>(char& c) {
        c = getchar();
        return *this;
      }

    private:
      std::istream& stream_;

      size_t size_{};

      char *buf_{};
      char *cur_{};
      char *end_{};

      bool eof_{false};

      void readmore() {
        int bytes_read = stream_.readsome(buf_, size_);

        cur_ = buf_;
        end_ = buf_ + bytes_read;

        if (bytes_read == 0) {
          eof_ = true;
          buf_[0] = 0;
        }
      }
  };

}
