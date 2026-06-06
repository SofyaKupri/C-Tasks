#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Document {
  std::string title;
  std::string body;
  Document(std::string t, std::string b)
      : title(std::move(t)), body(std::move(b)) {}
  ~Document() { std::cout << "Деструктор " << title << "\n"; }
};

class DocumentManager {
public:
  std::vector<std::unique_ptr<Document>> docs;
  const Document *cur = nullptr;

  void add(std::string t, std::string b) {
    docs.push_back(std::make_unique<Document>(t, b));
  }

  void remove(size_t index) {
    if (index >= docs.size()) {
      return;
    }
    if (docs[index].get() == cur) {
      cur = nullptr;
    }
    docs.erase(docs.begin() + index);
  }

  void setCurrent(size_t index) {
    cur = (index < docs.size()) ? docs[index].get() : nullptr;
  }
};

int main() {
  DocumentManager manager;
  manager.add("Doc1", "Content 1");
  manager.add("Doc2", "Content 2");
  manager.add("Doc3", "Content 3");

  manager.setCurrent(0);
  std::cout << "Установлен текущий: " << manager.cur->title << "\n";

  manager.remove(1);
  std::cout << "Удален Doc2. Текущий валиден? "
            << (manager.cur != nullptr ? "Да" : "Нет") << "\n";

  manager.remove(0);
  std::cout << "Удален Doc1 (текущий). Текущий == nullptr? "
            << (manager.cur == nullptr ? "Да" : "Нет") << "\n";

  std::vector<std::unique_ptr<Document>> otherDocs;
  std::cout << "Перемещаем Doc3 в другой вектор через std::move\n";
  if (!manager.docs.empty()) {
    otherDocs.push_back(std::move(manager.docs[0]));
    manager.docs.erase(manager.docs.begin());
  }
  std::cout << "Размер основного вектора: " << manager.docs.size()
            << "; размер другого: " << otherDocs.size() << "\n";
}