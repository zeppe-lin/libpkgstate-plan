# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
if get_option('tests').allowed()
  behavior = executable(
    'adapter-behavior-test',
    'plan_adapter_test.cpp',
    dependencies: libpkgstate_plan_dep,
    cpp_args: ['-UNDEBUG'],
  )
  test('adapter-behavior', behavior)
  public_headers = [
    ['export', 'libpkgstate-plan/export.h'],
    ['adapter', 'libpkgstate-plan/adapter.h'],
    ['libpkgstate_plan', 'libpkgstate-plan/libpkgstate-plan.h'],
  ]
  foreach header : public_headers
    public_header = executable(
      header[0] + '-public-header-test',
      'public_header_test.cpp',
      dependencies: libpkgstate_plan_dep,
      cpp_args: ['-DLIBPKGSTATE_ADAPTER_TEST_HEADER="' + header[1] + '"'],
    )
    test('compile public header ' + header[1], public_header)
  endforeach
  if library_kind == 'shared'
    nm = find_program('nm')
    test(
      'verify reviewed adapter ABI surface',
      find_program(meson.current_source_dir() / 'check_abi_surface.sh'),
      args: [
        pkgstate_plan.full_path(),
        meson.project_source_root() / 'abi/libpkgstate-plan.exports',
        nm.full_path(),
      ],
    )
  endif
  sh = find_program('sh')
  foreach contract : ['architecture', 'documentation', 'release_metadata', 'repository', 'extraction', 'ci']
    test(
      contract.replace('_', '-'),
      sh,
      args: [join_paths(meson.current_source_dir(), 'check_' + contract + '.sh'), meson.project_source_root()],
    )
  endforeach
  test(
    'style-contract',
    sh,
    args: [
      join_paths(meson.current_source_dir(), 'check_style.sh'),
      meson.project_source_root(),
      meson.project_build_root(),
    ],
  )
  test(
    'pkgconfig-metadata',
    sh,
    args: [join_paths(meson.current_source_dir(), 'check_pkgconfig_metadata.sh'), meson.project_build_root()],
  )
endif
