#include "InteractiveMarker.h"

#include "imgui.h"

#include "ImGuizmo.h"

#include "../utils.h"

static inline bool has(ControlAxis mask, ControlAxis value)
{
  return static_cast<std::underlying_type_t<ControlAxis>>(mask & value) != 0;
}

static inline ImGuizmo::OPERATION convert(ControlAxis mask)
{
  ImGuizmo::OPERATION out = static_cast<ImGuizmo::OPERATION>(0);
#define HANDLE(CA, OP) \
  if(has(mask, CA))    \
  {                    \
    out = out | OP;    \
  }
  HANDLE(ControlAxis::TX, ImGuizmo::OPERATION::TRANSLATE_X)
  HANDLE(ControlAxis::TY, ImGuizmo::OPERATION::TRANSLATE_Y)
  HANDLE(ControlAxis::TZ, ImGuizmo::OPERATION::TRANSLATE_Z)
  HANDLE(ControlAxis::RX, ImGuizmo::OPERATION::ROTATE_X)
  HANDLE(ControlAxis::RY, ImGuizmo::OPERATION::ROTATE_Y)
  HANDLE(ControlAxis::RZ, ImGuizmo::OPERATION::ROTATE_Z)
#undef HANDLE
  return out;
}

int InteractiveMarker::next_id_ = 0;

InteractiveMarker::InteractiveMarker(const sva::PTransformd & pose, ControlAxis mask) : pose_(pose), id_(next_id_++)
{
  this->mask(mask);
}

void InteractiveMarker::mask(ControlAxis mask)
{
  operation_ = convert(mask);
}

void InteractiveMarker::pose(const sva::PTransformd & pose)
{
  if(!active_)
  {
    pose_ = pose;
  }
}

bool InteractiveMarker::draw(const Camera & camera)
{
  if(operation_ == 0)
  {
    return false;
  }
  ImGuizmo::SetID(id_);
  auto view = camera.viewMatrix();
  auto projection = camera.camera().projectionMatrix();
  Magnum::Matrix4 mat = convert(pose_);
  float m[16];
  memcpy(&m, mat.data(), 16 * sizeof(float));
  auto op = static_cast<ImGuizmo::OPERATION>(operation_);
  bool changed = ImGuizmo::Manipulate(view.data(), projection.data(), op, ImGuizmo::MODE::LOCAL, m);
  active_ = ImGuizmo::IsUsing();
  if(changed)
  {
    pose_ = convert(Magnum::Matrix4::from(m));
  }
  return changed;
}
