#pragma once
#include "interfaces.h"
#include <iostream>

class ConsoleJournal: public Journal {
public:
    using inject = std::tuple<>;
private:
    std::ostream& Out() override { return std::cout; }
};