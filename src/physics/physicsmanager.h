#include "utils/common/singleton.h"

using WhiteDeer::Utils::Singleton;

namespace WhiteDeer::Engine {

class PhysicsManager : public Singleton<PhysicsManager> {
  friend class Singleton<PhysicsManager>;

 public:
  void Update();

 protected:
  PhysicsManager();
  void InitPhysics();
  void CleanupPhysics();
};

}  // namespace WhiteDeer::Engine