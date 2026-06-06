#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Student {
  std::string name;
  std::map<std::string, double> grades;
};

double averageGrade(const Student &s) {
  if (s.grades.empty())
    return -1.0;
  double sum = 0;
  for (auto const &[item, score] : s.grades)
    sum += score;
  return sum / s.grades.size();
}

void printStudents(const std::string &title,
                   const std::vector<Student> &students) {
  std::cout << "--- " << title << " ---\n";
  for (const auto &s : students) {
    double avg = averageGrade(s);
    std::cout << s.name << ": "
              << (avg < 0 ? "нет оценок" : std::to_string(avg)) << "\n";
  }
}

int main() {
  std::cout << "\n>>> ЗАДАЧА 3 <<<\n";
  std::vector<Student> students = {{"Taylor", {{"Math", 5}, {"C++", 4}}},
                                   {"Olivia", {{"Math", 3}, {"C++", 3}}},
                                   {"Lana", {}},
                                   {"Sabrina", {{"Math", 5}, {"C++", 5}}}};

  auto sort1 = students;
  std::sort(sort1.begin(), sort1.end(),
            [](const Student &a, const Student &b) { return a.name < b.name; });
  printStudents("1. По имени (возр.)", sort1);
  auto sort2 = students;
  auto cmpAvgDesc = [](const Student &a, const Student &b) {
    double avgA = averageGrade(a);
    double avgB = averageGrade(b);
    if (avgA == -1.0)
      return false;
    if (avgB == -1.0)
      return true;
    return avgA > avgB;
  };
  std::sort(sort2.begin(), sort2.end(), cmpAvgDesc);
  printStudents("2. По среднему (убыв.), пустые в конце", sort2);

  auto sort3 = students;
  std::sort(sort3.begin(), sort3.end(),
            [&](const Student &a, const Student &b) {
              double avgA = averageGrade(a);
              double avgB = averageGrade(b);
              if (std::abs(avgA - avgB) < 0.001)
                return a.name < b.name;
              if (avgA == -1.0)
                return false;
              if (avgB == -1.0)
                return true;
              return avgA > avgB;
            });
  printStudents("3. По среднему (убыв.) + по имени", sort3);

  auto sort4 = students;
  std::stable_sort(
      sort4.begin(), sort4.end(),
      [](const Student &a, const Student &b) { return a.name < b.name; });
  printStudents("4. Stable sort по имени", sort4);

  double minAvg = 4.0;
  auto partVec = students;
  auto it = std::partition(partVec.begin(), partVec.end(),
                           [minAvg](const Student &s) {
                             double avg = averageGrade(s);
                             return avg >= minAvg;
                           });

  std::cout << "\nСреднее >= " << minAvg << "\n";
  std::cout << "Удовлетворяют:\n";
  for (auto i = partVec.begin(); i != it; i++) {
    std::cout << " " << i->name << "\n";
  }
  std::cout << "Не удовлетворяют:\n";
  for (auto i = it; i != partVec.end(); i++) {
    std::cout << " " << i->name << "\n";
  }

  std::string subject = "Math";
  double minGrade = 4.5;
  std::vector<Student> topMathStudents;
  std::copy_if(
      students.begin(), students.end(), std::back_inserter(topMathStudents),
      [subject, minGrade](const Student &s) {
        return s.grades.count(subject) && s.grades.at(subject) >= minGrade;
      });
  printStudents("copy_if (Math >= 4.5)", topMathStudents);
}
