#!/bin/sh
# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
build=$1
pc=$build/meson-private/libpkgstate-plan.pc
[ -s "$pc" ] || { echo "pkgconfig-metadata: missing $pc" >&2; exit 1; }
grep -F 'Version: 3.0.0' "$pc" >/dev/null
grep -F -- '-lpkgstate-plan' "$pc" >/dev/null
public=$(sed -n 's/^Requires:[[:space:]]*//p' "$pc")
private=$(sed -n 's/^Requires\.private:[[:space:]]*//p' "$pc")
private_libs=$(sed -n 's/^Libs\.private:[[:space:]]*//p' "$pc")
printf '%s\n' "$public" | grep -F 'libpkgstate >=3.0.0' >/dev/null || { echo 'pkgconfig-metadata: missing public libpkgstate >=3.0.0' >&2; exit 1; }
printf '%s\n' "$public" | grep -F 'libpkgplan >=0.3.0' >/dev/null || { echo 'pkgconfig-metadata: missing public libpkgplan >=0.3.0' >&2; exit 1; }
if printf '%s\n' "$public" | grep -F 'libpkgstate-plan' >/dev/null; then echo 'pkgconfig-metadata: private edge leaked publicly: libpkgstate-plan' >&2; exit 1; fi
