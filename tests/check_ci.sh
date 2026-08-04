#!/bin/sh
# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
root=$1
fail(){ echo "ci-contract: $*" >&2; exit 1; }
workflow=$root/.github/workflows/ci.yml
[ -s "$workflow" ] || fail 'workflow is absent'
for script in ci/configure-and-test.sh ci/build-dependencies.sh ci/qualify-installed.sh ci/audit-shared-boundary.sh; do
  [ -x "$root/$script" ] || fail "missing executable $script"
  sh -n "$root/$script" || fail "invalid shell: $script"
done
for token in 'GCC shared' 'GCC static' 'Clang shared' 'Clang static' 'GCC release' 'address,undefined' 'meson==1.10.2' '--wrap-mode=nofallback'; do
  grep -F -- "$token" "$workflow" "$root/ci/configure-and-test.sh" "$root/ci/build-dependencies.sh" >/dev/null || fail "missing $token"
done
grep -F -- 'repository: zeppe-lin/libpkgplan' "$workflow" >/dev/null || fail 'missing dependency pin: repository: zeppe-lin/libpkgplan'
grep -F -- 'ref: v0.3.0' "$workflow" >/dev/null || fail 'missing dependency pin: ref: v0.3.0'
grep -F -- 'repository: zeppe-lin/libpkgstate' "$workflow" >/dev/null || fail 'missing dependency pin: repository: zeppe-lin/libpkgstate'

grep -F 'html: enabled' "$root/.github/workflows/ci.yml" >/dev/null || fail 'GCC shared HTML build is absent'
grep -F 'pandoc' "$root/.github/workflows/ci.yml" >/dev/null || fail 'Pandoc qualification dependency is absent'
grep -F -- '-Dhtml_docs=' "$root/.github/workflows/ci.yml" >/dev/null || fail 'HTML Meson feature is not configured'
grep -F 'qualify-html-docs.sh' "$root/.github/workflows/ci.yml" >/dev/null || fail 'installed HTML qualification is absent'
