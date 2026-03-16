#include "interfaces.h"

class VetClinic : public IVetClinic {
public:
    bool IsHealthy(IAlive *) const noexcept override { return true; }
    using inject = std::tuple<>;
};