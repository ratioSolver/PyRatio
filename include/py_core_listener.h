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

    void new_type(const ratio::core::type &t);
    void revise_type(const ratio::core::type &t);

    void new_predicate(const ratio::core::predicate &p);
    void revise_predicate(const ratio::core::predicate &p);

    void new_item(const ratio::core::item &itm);
    void new_atom(const ratio::core::atom &atm);

    void set(pybind11::object &c_obj, const std::string &name, const ratio::core::item &itm);

  private:
    pybind11::object py_slv;
    std::unordered_map<const ratio::core::type *, pybind11::object> all_types;
    std::unordered_map<const ratio::core::item *, pybind11::object> all_items;
  };
} // namespace ratio::python
