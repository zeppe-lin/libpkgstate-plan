// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "state.h"

#include <cstdint>

#include <libpkgplan/digest.h>
#include <libpkgstate-plan/adapter.h>

namespace plan_fixture {

template<typename Identity>
Identity identity(std::uint8_t seed)
{
  pkgplan::sha256_digest_bytes bytes{};
  for (std::size_t index = 0; index < bytes.size(); ++index)
    bytes[index] = static_cast<std::uint8_t>(seed + index);
  return Identity::from_sha256(bytes);
}

inline pkgstate::plan_adapter::planning_target_context target_context(
    const pkgstate::state_target_binding& binding, std::uint8_t seed = 70)
{
  return pkgstate::plan_adapter::planning_target_context(
      identity<pkgplan::target_system_context_identity>(seed), binding);
}

} // namespace plan_fixture
