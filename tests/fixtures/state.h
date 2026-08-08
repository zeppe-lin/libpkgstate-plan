// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <libpkgstate/libpkgstate.h>

namespace state_fixture {

template<typename Identity>
Identity identity(std::uint8_t seed)
{
  pkgstate::sha256_digest_bytes bytes{};
  for (std::size_t index = 0; index < bytes.size(); ++index)
    bytes[index] = static_cast<std::uint8_t>(seed + index);
  return Identity::from_sha256(bytes);
}

inline pkgstate::declaration_provenance at(
    std::string path, std::uint32_t line = 1)
{
  return pkgstate::declaration_provenance(
      "recipe.yml", std::move(path), line, 3);
}

inline pkgstate::state_target_binding target(std::uint8_t seed = 1)
{
  return pkgstate::state_target_binding::make(
      identity<pkgstate::managed_target_identity>(seed),
      identity<pkgstate::state_store_identity>(seed + 1),
      identity<pkgstate::root_view_identity>(seed + 2),
      identity<pkgstate::state_backend_identity>(seed + 3),
      identity<pkgstate::publication_domain_identity>(seed + 4));
}

inline pkgstate::package_requirement requirement(
    std::string name, std::string path = "requirements.run[0]")
{
  return pkgstate::package_requirement(
      pkgstate::package_reference(std::move(name)),
      {pkgstate::requirement_origin(at(std::move(path)), {})});
}

inline pkgstate::architecture_binding architectures(
    std::vector<std::string> declared_target = {"x86_64"})
{
  std::vector<pkgstate::architecture_reference> targets;
  targets.reserve(declared_target.size());
  for (std::string& name : declared_target)
    targets.emplace_back(std::move(name));
  return pkgstate::architecture_binding::make(
      {pkgstate::architecture_reference("x86_64")}, std::move(targets),
      pkgstate::architecture_reference("x86_64"),
      pkgstate::architecture_reference("x86_64"));
}

inline pkgstate::package_source_record source(
    std::string name = "example", std::uint8_t seed = 20,
    std::uint32_t release_number = 1,
    std::vector<pkgstate::package_requirement> runtime =
        {requirement("libfoo")},
    std::vector<pkgstate::lifecycle_program> programs = {},
    pkgstate::architecture_binding architecture = architectures())
{
  if (programs.empty())
  {
    programs.emplace_back(
        pkgstate::lifecycle_action::post_install,
        pkgstate::program(pkgstate::program_language::posix_shell,
                          "echo installed\n"));
    programs.emplace_back(
        pkgstate::lifecycle_action::pre_remove,
        pkgstate::program(pkgstate::program_language::posix_shell,
                          "echo removing\n"));
  }

  std::vector<pkgstate::lifecycle_requirement> lifecycle;
  lifecycle.emplace_back(pkgstate::lifecycle_action::post_install,
                         requirement("desktop-file-utils",
                                     "requirements.lifecycle.post-install[0]"));

  std::vector<pkgstate::selected_profile> profiles;
  profiles.emplace_back(
      pkgstate::profile_reference("@toolchain"),
      identity<pkgstate::source_profile_identity>(seed + 1),
      std::vector<pkgstate::declaration_provenance>{
          at("requirements.build[0]", 7)});

  pkgstate::package_release release(
      identity<pkgstate::package_release_identity>(seed),
      pkgstate::package_reference(std::move(name)), "1.2.3", release_number);
  return pkgstate::package_source_record::make(
      std::move(release),
      pkgstate::package_metadata(
          "Example package", std::optional<std::string>("Long description\n"),
          std::optional<std::string>("https://example.invalid"),
          {"GPL-3.0-or-later"}),
      std::move(runtime), std::move(programs), std::move(lifecycle),
      std::move(architecture), std::move(profiles),
      identity<pkgstate::source_snapshot_identity>(seed + 2));
}

inline pkgstate::package_source_record removal_source(
    std::string name = "example", std::uint8_t seed = 20)
{
  std::vector<pkgstate::lifecycle_program> programs;
  programs.emplace_back(
      pkgstate::lifecycle_action::pre_install,
      pkgstate::program(pkgstate::program_language::posix_shell,
                        "echo pre-install\n"));
  programs.emplace_back(
      pkgstate::lifecycle_action::post_install,
      pkgstate::program(pkgstate::program_language::posix_shell,
                        "echo post-install\n"));
  programs.emplace_back(
      pkgstate::lifecycle_action::pre_remove,
      pkgstate::program(pkgstate::program_language::posix_shell,
                        "echo pre-remove\n"));
  programs.emplace_back(
      pkgstate::lifecycle_action::post_remove,
      pkgstate::program(pkgstate::program_language::posix_shell,
                        "echo post-remove\n"));
  return source(std::move(name), seed, 1, {requirement("libfoo")},
                std::move(programs));
}

inline pkgstate::installed_object_metadata regular_object(
    std::uint8_t seed, std::uint32_t mode = 0644,
    std::int64_t seconds = 1700000000,
    std::optional<pkgstate::package_path> hardlink_anchor = std::nullopt)
{
  return pkgstate::installed_object_metadata(
      pkgstate::owned_object_kind::regular, mode, 10, 20,
      pkgstate::installed_object_timestamp(seconds, 17), std::uint64_t{7},
      identity<pkgstate::installed_regular_content_identity>(seed),
      std::nullopt, std::nullopt, std::move(hardlink_anchor));
}

inline pkgstate::installed_object_metadata plain_object(
    pkgstate::owned_object_kind kind, std::uint32_t mode = 0644)
{
  return pkgstate::installed_object_metadata(
      kind, mode, 10, 20, pkgstate::installed_object_timestamp(1700000000, 17));
}

inline pkgstate::installed_object_metadata symlink_object()
{
  return pkgstate::installed_object_metadata(
      pkgstate::owned_object_kind::symlink, 0777, 10, 20,
      pkgstate::installed_object_timestamp(1700000001, 19), std::nullopt,
      std::nullopt, std::string("../lib/example"));
}

inline pkgstate::installed_object_metadata device_object(
    pkgstate::owned_object_kind kind, std::uint64_t major,
    std::uint64_t minor)
{
  return pkgstate::installed_object_metadata(
      kind, 0600, 10, 20, pkgstate::installed_object_timestamp(1700000002, 23),
      std::nullopt, std::nullopt, std::nullopt,
      pkgstate::installed_device_number(major, minor));
}

inline pkgstate::owned_entry entry(
    std::string path, pkgstate::installed_object_metadata object,
    pkgstate::active_object_origin origin =
        pkgstate::active_object_origin::incoming_payload,
    std::optional<pkgstate::rejected_object_reference> rejected = std::nullopt)
{
  return pkgstate::owned_entry::make(pkgstate::package_path::parse(path),
                                     std::move(object), origin,
                                     std::move(rejected));
}

inline pkgstate::installed_control control(
    std::string name = "example", std::uint8_t seed = 20,
    pkgstate::installation_reason reason =
        pkgstate::installation_reason::explicit_request(),
    std::optional<pkgstate::package_source_record> source_record = std::nullopt)
{
  pkgstate::package_source_record source_value =
      source_record ? std::move(*source_record) : source(name, seed);
  return pkgstate::installed_control::make(
      source_value, std::move(reason),
      pkgstate::build_provenance(
          source_value.identity(),
          identity<pkgstate::build_request_identity>(seed + 4),
          identity<pkgstate::build_input_set_identity>(seed + 6),
          identity<pkgstate::environment_policy_identity>(seed + 7),
          identity<pkgstate::build_policy_identity>(seed + 8),
          identity<pkgstate::build_result_identity>(seed + 9),
          identity<pkgstate::payload_manifest_identity>(seed + 10),
          identity<pkgstate::build_artifact_identity>(seed + 11),
          identity<pkgstate::artifact_content_identity>(seed + 12),
          identity<pkgstate::artifact_binding_identity>(seed + 13),
          identity<pkgstate::execution_evidence_identity>(seed + 14),
          identity<pkgstate::build_image_identity>(seed + 15),
          identity<pkgstate::artifact_image_identity>(seed + 16),
          identity<pkgstate::artifact_inspection_identity>(seed + 17)));
}

inline pkgstate::installed_package package_with_manifest(
    std::string name, std::uint8_t seed, pkgstate::state_target_binding binding,
    std::vector<pkgstate::owned_entry> manifest,
    std::optional<pkgstate::package_source_record> source_record = std::nullopt)
{
  pkgstate::installation_receipt receipt = pkgstate::installation_receipt::make(
      control(name, seed, pkgstate::installation_reason::explicit_request(),
              std::move(source_record)),
      binding, std::move(manifest),
      identity<pkgstate::operation_plan_identity>(seed + 18),
      identity<pkgstate::application_evidence_identity>(seed + 19));
  return pkgstate::installed_package::make(std::move(receipt));
}

inline pkgstate::installed_package package(
    std::string name = "example", std::uint8_t seed = 20,
    pkgstate::state_target_binding binding = target())
{
  std::vector<pkgstate::owned_entry> manifest;
  manifest.push_back(entry("usr/bin/example", regular_object(seed + 12, 0755, -1)));
  manifest.push_back(entry(
      "etc/example.conf", regular_object(seed + 13),
      pkgstate::active_object_origin::retained_existing,
      pkgstate::rejected_object_reference(
          pkgstate::rejected_object_side::incoming,
          identity<pkgstate::rejected_object_identity>(seed + 17))));
  return package_with_manifest(std::move(name), seed, std::move(binding),
                               std::move(manifest));
}

inline pkgstate::snapshot state_with_package(
    std::string name = "example", std::uint8_t seed = 20,
    pkgstate::state_target_binding binding = target())
{
  pkgstate::installed_package value = package(name, seed, binding);
  return pkgstate::snapshot::make(std::move(binding), {std::move(value)});
}

} // namespace state_fixture
