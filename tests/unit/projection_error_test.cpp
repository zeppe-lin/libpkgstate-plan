// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>

#include <string>

int main()
{
  const pkgstate::plan_adapter::projection_error failure(
      pkgstate::plan_adapter::projection_error_code::object_translation,
      "object translation failed");

  TEST_EQ(failure.code(),
          pkgstate::plan_adapter::projection_error_code::object_translation);
  TEST_EQ(std::string(failure.what()), "object translation failed");
}
