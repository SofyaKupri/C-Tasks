#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

std::string trim(const std::string &str) {
  size_t first = str.find_first_not_of(" \t\r\n");
  if (first == std::string::npos) {
    return "";
  }
  size_t last = str.find_last_not_of(" \t\r\n");
  return str.substr(first, (last - first + 1));
}

std::string replaceAll(std::string str, const std::string &from,
                       const std::string &to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return str;
}

void save(const std::map<std::string, std::string> &data,
          const std::string &path) {
  std::ofstream out(path);
  for (const auto &[key, value] : data) {
    std::string escaped_val = replaceAll(value, "\n", "\\n");
    out << key << " = " << escaped_val << "\n";
  }
}

void loadLogic(std::map<std::string, std::string> &data,
               const std::string &path) {
  std::ifstream in(path);
  std::string line;
  int success = 0, comments = 0, errors = 0;
  while (std::getline(in, line)) {
    std::string t_line = trim(line);
    if (t_line.empty() || t_line[0] == '#') {
      comments++;
      continue;
    }
    size_t pos = t_line.find('=');
    if (pos == std::string::npos) {
      errors++;
      continue;
    }
    std::string key = trim(t_line.substr(0, pos));
    std::string value = trim(t_line.substr(pos + 1));
    if (key.empty() || key.find("\\n") != std::string::npos) {
      errors++;
      continue;
    }
    value = replaceAll(value, "\\n", "\n");
    data[key] = value;
    success++;
  }
  std::cout << "Файл: " << path << " (Успешно: " << success
            << ", Комментарии: " << comments << ", Ошибки: " << errors << ")\n";
}

void load(std::map<std::string, std::string> &data, const std::string &path) {
  data.clear();
  loadLogic(data, path);
}

void mergeFromFile(std::map<std::string, std::string> &data,
                   const std::string &path) {
  loadLogic(data, path);
}

void print(const std::map<std::string, std::string> &data) {
  for (const auto &[key, value] : data) {
    std::cout << key << " = " << replaceAll(value, "\n", "\\n") << std::endl;
  }
}

int main() {
  std::map<std::string, std::string> grammy;
  grammy["year"] = "2025";
  grammy["best_vocal_album"] = "Short\nSweet";
  grammy["best_song"] = "not like us";

  save(grammy, "grammy1.txt");
  std::cout << "Файл grammy1.txt сохранен.\n";
  std::ofstream out2("grammy2.txt");
  out2 << "# комментарий\n";
  out2 << "year = 2026\n";
  out2 << "best_new_artist = Dean\n";
  out2 << "invalid_key\\n = error\n";
  out2.close();

  load(grammy, "grammy1.txt");
  std::cout << "\nПосле load():\n";
  print(grammy);

  mergeFromFile(grammy, "grammy2.txt");
  std::cout << "\nПосле mergeFromFile():\n";
  print(grammy);
}
