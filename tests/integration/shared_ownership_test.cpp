// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/plan.h"
#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>

int main()
{
  const pkgstate::state_target_binding binding = state_fixture::target();
  const pkgstate::installed_object_metadata shared =
      state_fixture::regular_object(90, 0644);
  const pkgstate::installed_package zeta = state_fixture::package_with_manifest(
      "zeta", 40, binding,
      {state_fixture::entry("usr/share/shared", shared)});
  const pkgstate::installed_package alpha = state_fixture::package_with_manifest(
      "alpha", 60, binding,
      {state_fixture::entry("usr/share/shared", shared)});
  const pkgstate::snapshot state =
      pkgstate::snapshot::make(binding, {zeta, alpha});

  const auto projected = pkgstate::plan_adapter::project_installed_state(
      state, plan_fixture::target_context(binding));
  TEST_EQ(projected.packages().size(), std::size_t{2});
  TEST_EQ(projected.packages()[0].release().name(), "alpha");
  TEST_EQ(projected.packages()[1].release().name(), "zeta");

  const auto path = pkgplan::package_path::parse("usr/share/shared");
  const auto owners = projected.ownership().owners(path);
  TEST_EQ(owners.size(), std::size_t{2});
  TEST(projected.ownership().owns(projected.packages()[0].identity(), path));
  TEST(projected.ownership().owns(projected.packages()[1].identity(), path));
  TEST(projected.ownership().recorded_object(path) != nullptr);
}
