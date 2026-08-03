# Integration

Construct `planning_target_context` in orchestration, where complete target
context is available, and bind it to the exact durable state projection for the
snapshot being planned. Then project once and pass the resulting planner-owned
facts to `libpkgplan`.

```text
state store read -> snapshot --+
                               +-> libpkgstate-plan -> libpkgplan inputs
caller target context ----------+
```

Do not ask the adapter to infer target context from the host or to perform
resolution. Link it only in the planning composition layer.

Release `libpkgimage` 0.4.0 and `libpkgplan` 0.3.0 before building the planner
closure, then release `libpkgstate` 3.0.0 and finally `libpkgstate-plan` 3.0.0.
