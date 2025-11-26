#include "Push2-Display.h"

namespace ableton {

Push2Display *Push2Display::create()
{
  return new Push2Display;
}

NBase::Result Push2Display::Init(DeviceType deviceType)
{
  return communicator_.Init(dataSource_, deviceType);
}

} // namespace ableton
