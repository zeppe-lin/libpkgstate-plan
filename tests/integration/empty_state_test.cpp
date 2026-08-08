// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/plan.h"
#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>

int main()
{
  const pkgstate::state_target_binding binding = state_fixture::target(8);
  const pkgstate::snapshot state = pkgstate::snapshot::make(binding);
  const auto target = plan_fixture::target_context(binding, 90);
  const auto projected =
      pkgstate::plan_adapter::project_installed_state(state, target);

  TEST_EQ(projected.target(), target.identity());
  TEST(projected.packages().empty());
  TEST(projected.ownership().claims().empty());
  TEST_EQ(projected.ownership().identity().string(),
          state.ownership_identity().string());
  TEST_EQ(projected.ownership().snapshot().string(), state.identity().string());
  TEST_EQ(projected.ownership().completeness(),
          pkgplan::fact_set_completeness::complete);
}
