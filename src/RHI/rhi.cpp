#include "RHI/rhi.h"

#include "camera.h"
#include "RHI/fwd_rhi.h"

std::unique_ptr<RHI> RHI::m_spGFXInterface{};