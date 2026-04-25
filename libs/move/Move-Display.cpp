#include "Move-Display.h"

namespace ableton {

MoveDisplay *MoveDisplay::create()
{
  return new MoveDisplay;
}

NBase::Result MoveDisplay::Init()
{
  return communicator_.Init(dataSource_);
}

} // namespace ableton
