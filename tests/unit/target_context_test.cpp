// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/plan.h"
#include "../support/test.h"

int main()
{
  const pkgstate::state_target_binding state = state_fixture::target(11);
  const auto identity =
      plan_fixture::identity<pkgplan::target_system_context_identity>(80);
  const pkgstate::plan_adapter::planning_target_context context(identity, state);

  TEST_EQ(context.identity(), identity);
  TEST_EQ(context.state_projection(), state);

  const auto copied = context;
  TEST_EQ(copied.identity(), identity);
  TEST_EQ(copied.state_projection(), state);
}
