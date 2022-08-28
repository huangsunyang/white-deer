#include "components/light.h"

#include "components/transform.h"
#include "scene/gameobject.h"
#include <fmt/format.h>

namespace WhiteDeer {
namespace Engine {

void Light::Use(shared_ptr<Program> program, int index) {
  auto lightIndex = fmt::format("u_lights[{}]", index);
  auto transform = GetGameObject()->GetComponent<Transform>();
  program->SetUniform1i(lightIndex + ".type", m_type);
  program->SetUniform3f(lightIndex + ".pos", transform->GetPosition());
  program->SetUniform3f(lightIndex + ".dir", transform->GetZDirection());
  program->SetUniform3f(lightIndex + ".color", GetColor());

  if (m_type != LightType_Direction) {
    program->SetUniform1f(lightIndex + ".constant", m_constant);
    program->SetUniform1f(lightIndex + ".linear", m_linear);
    program->SetUniform1f(lightIndex + ".quadratic", m_quadratic);
    if (m_type == LightType_Spot) {
      program->SetUniform1f(lightIndex + ".cutoff", glm::cos(glm::radians(m_cutoff)));
      program->SetUniform1f(lightIndex + ".outerCutoff", glm::cos(glm::radians(m_outerCutoff)));
    }
  }
}

}  // namespace Engine
}  // namespace WhiteDeer