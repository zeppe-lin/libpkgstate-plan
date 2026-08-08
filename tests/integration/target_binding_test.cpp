// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/plan.h"
#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>

int main()
{
  const pkgstate::snapshot state = state_fixture::state_with_package();
  const auto target = plan_fixture::target_context(state_fixture::target(40));

  try
  {
    (void)pkgstate::plan_adapter::project_installed_state(state, target);
    TEST(false);
  }
  catch (const pkgstate::plan_adapter::projection_error& error)
  {
    TEST_EQ(error.code(),
            pkgstate::plan_adapter::projection_error_code::
                target_binding_mismatch);
  }
}
