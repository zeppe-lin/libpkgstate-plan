// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/plan.h"
#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>
#include <libpkgplan/fact_error.h>

#include <string>

int main()
{
  const pkgstate::state_target_binding binding = state_fixture::target();
  const pkgstate::installed_package first = state_fixture::package_with_manifest(
      "alpha", 40, binding,
      {state_fixture::entry("usr/share/shared",
                            state_fixture::regular_object(90, 0644))});
  const pkgstate::installed_package second = state_fixture::package_with_manifest(
      "beta", 60, binding,
      {state_fixture::entry("usr/share/shared",
                            state_fixture::regular_object(91, 0600))});

  const pkgstate::snapshot state =
      pkgstate::snapshot::make(binding, {first, second});
  TEST_EQ(state.owners(pkgstate::package_path::parse("usr/share/shared")).size(),
          std::size_t{2});

  try
  {
    (void)pkgstate::plan_adapter::project_installed_state(
        state, plan_fixture::target_context(binding));
    TEST(false);
  }
  catch (const pkgstate::plan_adapter::projection_error& error)
  {
    TEST_EQ(error.code(),
            pkgstate::plan_adapter::projection_error_code::object_translation);
    TEST(std::string(error.what()).find("planner ownership vocabulary") !=
         std::string::npos);
  }
  catch (const pkgplan::fact_error&)
  {
    TEST(false);
  }
}
