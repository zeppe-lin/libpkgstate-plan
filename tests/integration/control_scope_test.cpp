// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/state.h"
#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>

int main()
{
  const pkgstate::package_source_record source = state_fixture::source(
      "example", 20, 1, {}, {},
      state_fixture::architectures({"x86_64", "aarch64"}));
  const pkgplan::candidate_control_projection projected =
      pkgstate::plan_adapter::project_candidate_control(source);

  TEST(projected.runtime_dependencies().empty());
  TEST_EQ(projected.removal_lifecycle().size(), std::size_t{1});
  TEST_EQ(projected.removal_lifecycle().front().phase(),
          pkgplan::removal_lifecycle_phase::pre_remove);
  TEST_EQ(projected.target_profile().front().value(), "aarch64,x86_64");

  const pkgstate::package_source_record open = state_fixture::source(
      "open", 30, 1, {}, {}, state_fixture::architectures({}));
  const pkgplan::candidate_control_projection open_projection =
      pkgstate::plan_adapter::project_candidate_control(open);
  TEST_EQ(open_projection.target_profile().front().value(), "*");
}
