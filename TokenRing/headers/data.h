#pragma once
#include <stdio.h>
#include <cstring>

enum class DataType { kToken, kMessage };

class Data {
public:
  const DataType data_type;
  virtual ~Data() = default;

protected:
  explicit Data(DataType dt);
};

class Token : public Data {
public:
  explicit Token() : Data(DataType::kToken) {}
};

struct Header {
  int sender_id, reciever_id;
};
struct Chunk {
  unsigned total; /* на сколько всего частей разбили */
  unsigned id;    /* номер текущей части */
  unsigned size;  /* длина строки, записанная в поле data */
};

constexpr size_t MAX_PACKEGE_SIZE = 32;
constexpr size_t MESSAGE_SIZE =
    MAX_PACKEGE_SIZE - (sizeof(Header) + sizeof(Chunk));

class Message : public Data {
public:
  Header header;
  Chunk chunk;

  char data[MESSAGE_SIZE]{'\0'};
  explicit Message(int sender_id, int reciever_id, const char *s, int id,
                   int total);
};