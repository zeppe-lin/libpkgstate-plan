#!/bin/sh
# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
root=$1
fail(){ echo "test-layout-contract: $*" >&2; exit 1; }

for dir in contracts fixtures header integration support unit; do
  [ -d "$root/tests/$dir" ] || fail "missing tests/$dir"
done

if find "$root/tests" -maxdepth 1 -type f \( -name '*.cpp' -o -name '*.h' -o -name 'check_*.sh' \) | grep . >/dev/null; then
  fail 'uncategorized test source remains in tests root'
fi

for file in \
  tests/fixtures/state.h \
  tests/fixtures/plan.h \
  tests/support/test.h \
  tests/support/projection.h \
  tests/unit/projection_error_test.cpp \
  tests/unit/target_context_test.cpp \
  tests/header/public_header_test.cpp \
  tests/integration/candidate_control_test.cpp \
  tests/integration/control_scope_test.cpp \
  tests/integration/installed_package_test.cpp \
  tests/integration/empty_state_test.cpp \
  tests/integration/target_binding_test.cpp \
  tests/integration/ownership_metadata_test.cpp \
  tests/integration/state_only_evidence_test.cpp \
  tests/integration/shared_ownership_test.cpp \
  tests/integration/contradictory_ownership_test.cpp; do
  [ -s "$root/$file" ] || fail "missing $file"
done

for suite in unit integration header contract; do
  grep -F "suite: '$suite'" "$root/tests/meson.build" >/dev/null ||
    fail "$suite suite is not registered"
done
