# Architecture

## Authority flow

Contract shorthand: `canonical snapshot + caller target context -> planner facts`.

```text
canonical libpkgstate snapshot
+ caller-authoritative planner target context
                    |
                    v
planner package facts + ownership inventory
```

`libpkgstate-plan` is a read-only projection boundary. Installed state remains
owned by `libpkgstate`; target and fact vocabulary remain owned by
`libpkgplan`.

## Projection invariants

The caller supplies a `planning_target_context` containing the complete planner
target identity and the durable `state_target_binding` it claims belongs to
that context. `project_installed_state()` recomputes the binding represented by
the snapshot and refuses a mismatch. State therefore cannot invent kernel,
platform, or policy context that is absent from durable installed truth.

Package facts preserve snapshot package-name order. The projection carries
planner-relevant release and control identities, runtime requirements,
pre-remove and post-remove lifecycle programs, declared target architectures,
and a normalized complete path-to-owner inventory. Every recorded object field
that exists in planner vocabulary is translated. Hard-link topology remains
state-owned because the planner object model has no corresponding field.

Build/check requirements, profile expansion, installation lifecycle,
installation reason, build provenance, and rejected-object evidence are not
counterfeited into planner facts.

## Non-authorities

The adapter does not resolve dependencies, compare candidates, produce an
operation graph, inspect a target filesystem, or mutate state. Projection is
deterministic for the supplied snapshot and target context.

## Dependency placement

Both dependencies are public: installed declarations expose `libpkgstate`
snapshots and `libpkgplan` target/fact types. Source, build, image, and
application authorities are forbidden.

The root commit records the exact 2.5.1 implementation and behavior test.
The independent repository then promoted durable candidate-control projection
into this sole state-to-planner boundary without rewriting its extraction
history.
