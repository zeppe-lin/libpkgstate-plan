// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/state.h"
#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>

int main()
{
  const pkgstate::package_source_record source =
      state_fixture::removal_source();
  const pkgplan::candidate_control_projection projected =
      pkgstate::plan_adapter::project_candidate_control(source);

  TEST_EQ(projected.runtime_dependencies().size(), std::size_t{1});
  TEST_EQ(projected.runtime_dependencies().front().expression(), "libfoo");
  TEST_EQ(projected.removal_lifecycle().size(), std::size_t{2});
  TEST_EQ(projected.removal_lifecycle()[0].phase(),
          pkgplan::removal_lifecycle_phase::pre_remove);
  TEST_EQ(projected.removal_lifecycle()[0].material(), "echo pre-remove\n");
  TEST_EQ(projected.removal_lifecycle()[1].phase(),
          pkgplan::removal_lifecycle_phase::post_remove);
  TEST_EQ(projected.removal_lifecycle()[1].material(), "echo post-remove\n");
  TEST_EQ(projected.target_profile().size(), std::size_t{1});
  TEST_EQ(projected.target_profile().front().name(),
          "pkgsource.target-architectures");
  TEST_EQ(projected.target_profile().front().value(), "x86_64");
}
