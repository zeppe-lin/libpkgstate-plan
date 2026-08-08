// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/plan.h"
#include "../support/projection.h"
#include "../support/test.h"

#include <libpkgstate-plan/adapter.h>

namespace {

const pkgplan::filesystem_object_metadata& object(
    const pkgstate::plan_adapter::installed_state_projection& projection,
    const char* path)
{
  const auto& claim = projection_support::claim(projection, path);
  TEST(claim.recorded_object().has_value());
  return *claim.recorded_object();
}

} // namespace

int main()
{
  const pkgstate::state_target_binding binding = state_fixture::target();
  std::vector<pkgstate::owned_entry> manifest;
  manifest.push_back(state_fixture::entry(
      "usr/lib/regular", state_fixture::regular_object(50, 0640, -7)));
  manifest.push_back(state_fixture::entry(
      "usr/share/example", state_fixture::plain_object(
                               pkgstate::owned_object_kind::directory, 0750)));
  manifest.push_back(
      state_fixture::entry("usr/bin/link", state_fixture::symlink_object()));
  manifest.push_back(state_fixture::entry(
      "run/example.fifo",
      state_fixture::plain_object(pkgstate::owned_object_kind::fifo, 0600)));
  manifest.push_back(state_fixture::entry(
      "dev/example-char",
      state_fixture::device_object(pkgstate::owned_object_kind::character_device,
                                   1, 7)));
  manifest.push_back(state_fixture::entry(
      "dev/example-block",
      state_fixture::device_object(pkgstate::owned_object_kind::block_device,
                                   8, 1)));
  manifest.push_back(state_fixture::entry(
      "run/example.sock",
      state_fixture::plain_object(pkgstate::owned_object_kind::socket, 0660)));
  manifest.push_back(state_fixture::entry(
      "var/lib/example.other",
      state_fixture::plain_object(pkgstate::owned_object_kind::other, 0644)));

  const pkgstate::installed_package package = state_fixture::package_with_manifest(
      "example", 20, binding, std::move(manifest));
  const pkgstate::snapshot state =
      pkgstate::snapshot::make(binding, {package});
  const auto projected = pkgstate::plan_adapter::project_installed_state(
      state, plan_fixture::target_context(binding));

  const auto& regular = object(projected, "usr/lib/regular");
  TEST_EQ(regular.kind(), pkgplan::filesystem_object_kind::regular);
  TEST_EQ(regular.mode(), std::uint32_t{0640});
  TEST_EQ(regular.uid(), std::uint64_t{10});
  TEST_EQ(regular.gid(), std::uint64_t{20});
  TEST_EQ(*regular.size(), std::uint64_t{7});
  TEST_EQ(regular.mtime()->seconds(), std::int64_t{-7});
  TEST_EQ(regular.mtime()->nanoseconds(), std::uint32_t{17});
  TEST_EQ(regular.regular_content()->string(),
          state_fixture::identity<pkgstate::installed_regular_content_identity>(
              50).string());

  const auto& directory = object(projected, "usr/share/example");
  TEST_EQ(directory.kind(), pkgplan::filesystem_object_kind::directory);
  TEST(!directory.size().has_value());
  TEST(!directory.regular_content().has_value());

  const auto& symlink = object(projected, "usr/bin/link");
  TEST_EQ(symlink.kind(), pkgplan::filesystem_object_kind::symlink);
  TEST_EQ(*symlink.symlink_target(), "../lib/example");

  const auto& fifo = object(projected, "run/example.fifo");
  TEST_EQ(fifo.kind(), pkgplan::filesystem_object_kind::fifo);
  const auto& character = object(projected, "dev/example-char");
  TEST_EQ(character.kind(), pkgplan::filesystem_object_kind::character_device);
  TEST_EQ(character.device()->major(), std::uint64_t{1});
  TEST_EQ(character.device()->minor(), std::uint64_t{7});
  const auto& block = object(projected, "dev/example-block");
  TEST_EQ(block.kind(), pkgplan::filesystem_object_kind::block_device);
  TEST_EQ(block.device()->major(), std::uint64_t{8});
  TEST_EQ(block.device()->minor(), std::uint64_t{1});
  TEST_EQ(object(projected, "run/example.sock").kind(),
          pkgplan::filesystem_object_kind::socket);
  TEST_EQ(object(projected, "var/lib/example.other").kind(),
          pkgplan::filesystem_object_kind::other);
}
