# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
public_inc = include_directories('../include')
pkgstate_plan = library(
  'pkgstate-plan',
  [
    'plan_adapter.cpp',
  ],
  cpp_args: ['-D_FILE_OFFSET_BITS=64'],
  include_directories: public_inc,
  dependencies: [
    libpkgstate_dep,
    libpkgplan_dep,
  ],
  install: true,
  version: meson.project_version(),
  soversion: '2',
)
libpkgstate_plan_dep = declare_dependency(
  link_with: pkgstate_plan,
  include_directories: public_inc,
  dependencies: [
    libpkgstate_dep,
    libpkgplan_dep,
  ],
  compile_args: ['-D_FILE_OFFSET_BITS=64'],
  version: meson.project_version(),
)
meson.override_dependency('libpkgstate-plan', libpkgstate_plan_dep)
install_headers(
  '../include/libpkgstate-plan/adapter.h',
  '../include/libpkgstate-plan/libpkgstate-plan.h',
  subdir: 'libpkgstate-plan',
)
pkgconf.generate(
  pkgstate_plan,
  name: 'libpkgstate-plan',
  filebase: 'libpkgstate-plan',
  description: 'Projection of native libpkgstate authority into libpkgplan',
  subdirs: '.',
  requires: [libpkgstate_dep, libpkgplan_dep],
  requires_private: [],
)
