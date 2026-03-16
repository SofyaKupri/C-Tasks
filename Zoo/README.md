# Moscow Zoo ERP System

## Описание решения
Консольное приложение для учёта животных и вещей в Московском зоопарке, получения информации о них, с использованием DI контейнера
## SOLID
1. Single Responsibility Principle: Каждый класс отвечает за одну сущность (Animal, Thing, Zoo, VetClinic, Journal)
2. Open/Closed Principle: Можно добавлять новые виды животных/вещей/сервисов, не меняя код
3. Liskov Substitution Principle: Предки могут заменяться детьми без изменения работы программы
4. Interface Segregation Principle: Интерфейсы разделены по назначению (IAlive, IInventory, IPlayerAnimal, IVetClinic, IZoo)
5. Dependency Inversion Principle

## Инструкция по сборке и запуску
```bash
cd build
cmake ..
cmake --build .
```