// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include <libpkgstate-plan/libpkgstate-plan.h>

int
main()
{
  auto* volatile function = &pkgstate::plan_adapter::project_installed_state;
  return function == nullptr ? 1 : 0;
}
