# libpkgstate-plan

libpkgstate-plan provides read-only projection of durable installed state into planner vocabulary.

```text
canonical snapshot + caller target context -> planner-owned facts
```

## Authority

This repository owns the only durable-state to `libpkgplan` translation, including candidate control projection reused by downstream composition boundaries. It is a translation boundary, not another semantic owner. Its input and output models remain authoritative in their respective repositories.

The public operation accepts one complete canonical snapshot and one caller-authoritative planner target context bound to that snapshot target. A successful projection retains planner package facts, runtime and removal control, target profile facts, and the complete translated ownership inventory.

The adapter performs no discovery, parsing, dependency resolution, build execution, archive inspection, target mutation, state publication, migration, retry policy, or compatibility import unless the operation is explicitly part of the contract above. It exports refusal rather than guessing. It refuses target binding mismatch and identity, path, object, or control vocabulary disagreement.

See `docs/architecture.md` for invariants and `docs/integration.md` for placement in the package-management graph.

## Dependency boundary

Public installed closure: `libpkgstate >=3.0.0` and `libpkgplan >=0.3.0`.

Private implementation closure: none.

Fallback subprojects are intentionally unsupported. Shared consumers receive only public requirements; static consumers receive the complete private closure through pkg-config.

## Build

```sh
meson setup build-shared \
  -Ddefault_library=shared \
  -Dlink_mode=shared
meson compile -C build-shared
meson test -C build-shared --print-errorlogs

meson setup build-static \
  -Ddefault_library=static \
  -Dlink_mode=static
meson compile -C build-static
meson test -C build-static --print-errorlogs
```

Shared and static artifacts must come from separate build directories. `default_library=both` is rejected because one dependency closure cannot truthfully represent both linkage modes.

## Release lineage

The 3.0 repository was extracted from `libpkgstate` 2.5.1. The repository root preserves extraction provenance; later commits may evolve the independent product without rewriting that history. The library preserves SONAME generation 2.

Release after `libpkgstate`, `libpkgimage` 0.4.0, and `libpkgplan` 0.3.0. Release before `libpkgstate-apply`.

## Documentation

- `docs/architecture.md` — authority and refusal invariants;
- `docs/integration.md` — composition and release order;
- `docs/testing.md` — qualification matrix;
- `docs/abi.md` — ABI and pkg-config policy;
- `man/libpkgstate-plan.3.scdoc` — installed `libpkgstate-plan.3` interface manual;
- `MAINTAINING.md` — release gate.

## License

GPL-3.0-or-later. See `COPYING` and `COPYRIGHT`.
