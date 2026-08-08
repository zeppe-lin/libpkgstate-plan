// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/plan.h"
#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>

int main()
{
  const pkgstate::snapshot state = state_fixture::state_with_package();
  const auto target = plan_fixture::target_context(state.target_binding());
  const auto projected =
      pkgstate::plan_adapter::project_installed_state(state, target);

  TEST_EQ(projected.target(), target.identity());
  TEST_EQ(projected.packages().size(), std::size_t{1});

  const pkgstate::installed_package& state_package = state.packages().front();
  const pkgplan::installed_package_fact& package = projected.packages().front();
  TEST_EQ(package.identity().string(), state_package.identity().string());
  TEST_EQ(package.control().string(), state_package.control().identity().string());
  TEST_EQ(package.snapshot().string(), state.identity().string());
  TEST_EQ(package.release().identity().string(),
          state_package.release().identity().string());
  TEST_EQ(package.release().name(), "example");
  TEST_EQ(package.release().version(), "1.2.3");
  TEST_EQ(package.release().release(), "1");

  const pkgplan::installed_control_projection& control =
      package.control_projection();
  TEST_EQ(control.completeness().runtime_dependencies,
          pkgplan::control_fact_availability::known);
  TEST_EQ(control.completeness().removal_lifecycle,
          pkgplan::control_fact_availability::known);
  TEST_EQ(control.completeness().target_profile,
          pkgplan::control_fact_availability::known);
  TEST_EQ(control.runtime_dependencies().size(), std::size_t{1});
  TEST_EQ(control.runtime_dependencies().front().expression(), "libfoo");
}
