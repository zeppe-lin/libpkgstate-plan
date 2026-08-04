# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
public_inc = include_directories('../include')

elf_export_generator = find_program(
  meson.project_source_root() / 'tools/generate-elf-export-script.sh',
)
export_script_name = 'libpkgstate-plan.exports.map'
export_script_path = meson.current_build_dir() / export_script_name
export_script = configure_file(
  input: '../abi/libpkgstate-plan.exports',
  output: export_script_name,
  command: [elf_export_generator, '@INPUT@'],
  capture: true,
)

pkgstate_plan = library(
  'pkgstate-plan',
  [
    'plan_adapter.cpp',
  ],
  cpp_args: visibility_cpp_args + ['-DPKGSTATE_PLAN_BUILDING_LIBRARY', '-D_FILE_OFFSET_BITS=64'],
  gnu_symbol_visibility: 'hidden',
  link_args: ['-Wl,--version-script=' + export_script_path],
  link_depends: [export_script],
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
  '../include/libpkgstate-plan/export.h',
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
  requires: [
    'libpkgstate >= 3.0.0',
    'libpkgplan >= 0.3.0',
  ],
)
