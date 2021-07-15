#pragma once

#include "details/TransformBase.h"

namespace mc_rtc::magnum
{

struct TransformWidget : public TransformBase<ControlAxis::ALL>
{
  TransformWidget(Client & client, const ElementId & id, McRtcGui & gui, const ElementId & reqId)
  : TransformBase(client, id, gui, reqId)
  {
  }

  void draw3D() override
  {
    TransformBase::draw3D();
    gui_.drawFrame(convert(marker_.pose()));
  }
};

} // namespace mc_rtc::magnum
