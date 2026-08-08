# Testing

Qualification is layered so one successful installed-state projection cannot
hide a damaged vocabulary seam, package boundary, or test harness.

The `unit` suite exercises adapter-owned value and typed-error semantics that
require no installed-state authority graph. The `integration` suite constructs
real `libpkgstate` snapshots and projects them through the real `libpkgplan`
public vocabulary. It separately proves candidate control, scope exclusion,
installed package identity, empty state, target binding, every installed object
class, state-only hard-link/rejected evidence exclusion, shared ownership, and
contradictory shared-object refusal.

Contradictory object metadata for a shared path is valid durable state because
`libpkgstate` owns package-local recorded object history. `libpkgplan`
intentionally refuses contradictory object facts in one planner ownership
inventory. `libpkgstate-plan` therefore translates that planner refusal into its
typed projection-error domain; it must not leak a raw planner exception or
rewrite either neighbor's model.

Installation lifecycle, lifecycle requirement provenance, installation reason,
build provenance, rejected-object references, active-object origin, and
hard-link topology remain state-owned unless planner vocabulary explicitly
represents them. Integration tests pin that scope rather than inferring
equivalent planner facts.

The `header` suite compiles the umbrella and each installed header independently.
The `contract` suite owns architecture, documentation, release, repository,
extraction, CI, style, test-layout, ABI, and generated pkg-config checks. Shared
builds compare dynamic exports to `abi/libpkgstate-plan.exports`, verify SONAME
`2`, and audit direct `DT_NEEDED` edges. Static builds prove the complete private
pkg-config closure.

Fixtures contain deterministic state/target construction only. Shared
query/assertion helpers live under `tests/support`; behavioral assertions stay
in individual unit or integration programs. Fixtures do not duplicate
projection policy.

Source contracts verify architecture placement, release metadata, repository
hygiene, CI coverage, style, test topology, and root-commit extraction
provenance. The provenance contract checks the root extraction against the
recorded `libpkgstate` 2.5.1 hashes; it intentionally does not freeze current
implementation files.

CI runs GCC and Clang in separate shared and static configurations, one
optimized release configuration, and ASan/UBSan configurations. Installation
qualification compiles a consumer against staged headers and metadata, checks
every installed header, audits the shared boundary or static archive, and
verifies installed manual and project documentation.

The documentation contract receives the include roots of the production
dependencies resolved by Meson before Clang parses public headers. Ambient
system-installed zoo headers are not accepted as dependency closure.

A release candidate is incomplete until the exact dependency tags used by CI
exist and the whole matrix is green from clean build directories.
