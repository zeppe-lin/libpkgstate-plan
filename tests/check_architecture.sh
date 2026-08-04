# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
root=$1
fail(){ echo "architecture-contract: $*" >&2; exit 1; }
if grep -R -F 'libpkgsource' "$root/include" "$root/src" "$root/meson.build" "$root/src/meson.build" >/dev/null 2>&1; then fail 'forbidden authority dependency: libpkgsource'; fi
if grep -R -F 'libpkgbuild' "$root/include" "$root/src" "$root/meson.build" "$root/src/meson.build" >/dev/null 2>&1; then fail 'forbidden authority dependency: libpkgbuild'; fi
if grep -R -F 'libpkgimage' "$root/include" "$root/src" "$root/meson.build" "$root/src/meson.build" >/dev/null 2>&1; then fail 'forbidden authority dependency: libpkgimage'; fi
if grep -R -F 'libpkgapply' "$root/include" "$root/src" "$root/meson.build" "$root/src/meson.build" >/dev/null 2>&1; then fail 'forbidden authority dependency: libpkgapply'; fi
if grep -R -F 'libpkgstate-source' "$root/include" "$root/src" "$root/meson.build" "$root/src/meson.build" >/dev/null 2>&1; then fail 'forbidden authority dependency: libpkgstate-source'; fi
if grep -R -F 'libpkgstate-build' "$root/include" "$root/src" "$root/meson.build" "$root/src/meson.build" >/dev/null 2>&1; then fail 'forbidden authority dependency: libpkgstate-build'; fi
grep -F 'canonical snapshot + caller target context -> planner facts' "$root/docs/architecture.md" >/dev/null || fail 'authority flow is undocumented'
grep -F "gnu_symbol_visibility: 'hidden'" "$root/src/meson.build" >/dev/null || fail 'hidden visibility is not enforced'
test -s "$root/abi/libpkgstate-plan.exports" || fail 'reviewed export manifest is absent'
if grep -R -F 'catch (const std::exception' "$root/src" >/dev/null; then fail 'adapter launders unrelated process failures through std::exception'; fi
