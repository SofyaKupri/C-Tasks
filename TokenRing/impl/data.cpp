#include "../headers/data.h"

Data::Data(DataType dt) : data_type(dt) {}

Message::Message(int sender_id, int reciever_id, const char *s, int id,
                 int total)
    : Data(DataType::kMessage) {
  header.sender_id = sender_id;
  header.reciever_id = reciever_id;

  chunk.total = total;
  chunk.id = id;
  chunk.size = strlen(s);
  for (size_t i = 0; i < chunk.size; i++) {
    data[i] = s[i];
  }
}
