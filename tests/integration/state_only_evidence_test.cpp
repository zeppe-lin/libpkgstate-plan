// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/plan.h"
#include "../support/projection.h"
#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>

int main()
{
  const pkgstate::state_target_binding binding = state_fixture::target();
  const pkgstate::installed_object_metadata anchor =
      state_fixture::regular_object(60, 0644);
  const pkgstate::installed_object_metadata peer = state_fixture::regular_object(
      60, 0644, 1700000000,
      pkgstate::package_path::parse("usr/lib/example.so"));
  std::vector<pkgstate::owned_entry> manifest;
  manifest.push_back(state_fixture::entry("usr/lib/example.so", anchor));
  manifest.push_back(state_fixture::entry(
      "usr/lib/example-link.so", peer,
      pkgstate::active_object_origin::retained_existing,
      pkgstate::rejected_object_reference(
          pkgstate::rejected_object_side::incoming,
          state_fixture::identity<pkgstate::rejected_object_identity>(79))));

  const pkgstate::installed_package package = state_fixture::package_with_manifest(
      "example", 20, binding, std::move(manifest));
  const pkgstate::snapshot state =
      pkgstate::snapshot::make(binding, {package});
  const auto projected = pkgstate::plan_adapter::project_installed_state(
      state, plan_fixture::target_context(binding));

  const auto& first = projection_support::claim(projected, "usr/lib/example.so");
  const auto& second =
      projection_support::claim(projected, "usr/lib/example-link.so");
  TEST(first.recorded_object().has_value());
  TEST(second.recorded_object().has_value());
  TEST_EQ(*first.recorded_object(), *second.recorded_object());

  TEST_EQ(projected.packages().front().control_projection()
              .removal_lifecycle()
              .size(),
          std::size_t{1});
}
