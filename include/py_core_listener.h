#pragma once
#include "core_listener.h"

namespace ratio::python
{
  class py_core_listener : public ratio::core::core_listener
  {
  public:
    py_core_listener(ratio::core::core &cr);
    ~py_core_listener();

  private:
  };
} // namespace ratio::python
