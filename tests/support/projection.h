// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <stdexcept>
#include <string_view>

#include <libpkgplan/ownership.h>
#include <libpkgstate-plan/adapter.h>

namespace projection_support {

inline const pkgplan::installed_ownership_claim& claim(
    const pkgstate::plan_adapter::installed_state_projection& projection,
    std::string_view path)
{
  for (const pkgplan::installed_ownership_claim& value :
       projection.ownership().claims())
  {
    if (value.path().string() == path)
      return value;
  }
  throw std::runtime_error("projected ownership claim is absent");
}

} // namespace projection_support
