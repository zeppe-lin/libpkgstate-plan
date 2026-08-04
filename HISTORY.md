# History

## 3.0.0 (2026-08-04)

- Extracted `libpkgstate-plan` from the `libpkgstate` 2.5.1 repository.
- Preserved the extracted behavior as repository provenance and retained SONAME generation 2.
- Established an independent dependency closure: libpkgstate >=3.0.0; libpkgplan >=0.3.0.
- Added public-header, pkg-config, extraction-provenance, architecture, repository, compiler, sanitizer, shared, and static qualification.
- Completed the documented public projection contract under Doxygen warnings-as-errors.
- Renamed the installed manual from the former in-tree adapter name to `libpkgstate-plan.3`.
- Centralized durable source-control projection so downstream adapters do not recreate planner vocabulary.
- Qualified planner projection against package-source-record protocol version 2 without retired recipe identity.
