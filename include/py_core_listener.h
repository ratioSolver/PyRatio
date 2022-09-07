#pragma once
#include "core_listener.h"
#include <pybind11/pybind11.h>

namespace ratio::python
{
  class py_core_listener : public ratio::core::core_listener
  {
  public:
    py_core_listener(ratio::core::core &cr, pybind11::object &py_slv);
    ~py_core_listener();

  private:
    void log(const std::string &msg) override;
    void read(const std::string &script) override;
    void read(const std::vector<std::string> &files) override;

    void state_changed() override;

    void started_solving() override;
    void solution_found() override;
    void inconsistent_problem() override;

  private:
    pybind11::object py_slv;
    std::unordered_map<const ratio::core::type *, PyObject *> all_types;
    std::unordered_map<const ratio::core::item *, PyObject *> all_items;
  };
} // namespace ratio::python
