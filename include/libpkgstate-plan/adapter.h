// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

/*!
 * \file adapter.h
 * \brief Explicit projection of canonical installed state into libpkgplan.
 */

#pragma once

#include <libpkgstate-plan/export.h>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include <libpkgplan/control.h>
#include <libpkgplan/digest.h>
#include <libpkgplan/ownership.h>
#include <libpkgplan/package_fact.h>
#include <libpkgstate/package_source_record.h>
#include <libpkgstate/snapshot.h>

/*! \brief Durable-state projection into planner-owned fact vocabulary. */
namespace pkgstate::plan_adapter {

/*! \brief Structured reason that installed-state projection failed. */
enum class projection_error_code : std::uint8_t {
  target_binding_mismatch, //!< Caller target projection does not match state.
  identity_translation,    //!< Identity wire representation was incompatible.
  path_translation,        //!< Canonical path vocabularies were incompatible.
  object_translation,      //!< Installed object vocabulary was incompatible.
  control_translation,     //!< Native control vocabulary was incompatible.
};

/*! \brief Failure to project canonical state into planner-owned fact types. */
class PKGSTATE_PLAN_API projection_error final : public std::invalid_argument {
public:
  /*!
   * \brief Construct a typed projection failure.
   * \param code Stable refusal category.
   * \param message Human-readable diagnostic text.
   */
  projection_error(projection_error_code code, std::string message);

  /*! \brief Destroy the polymorphic projection failure. */
  ~projection_error() override;

  /*!
   * \brief Return the machine-readable failure class.
  *  \return The machine-readable failure class.
   */
  [[nodiscard]] projection_error_code code() const noexcept;

private:
  projection_error_code code_;
};

/*!
 * \brief Project durable source control into planner candidate control.
 *
 * This is the canonical state-to-planner translation for runtime dependencies,
 * removal lifecycle declarations, and target-profile facts retained by one
 * package source record.  It does not construct an installed package fact,
 * inspect a snapshot, or infer any missing source authority.
 *
 * \param source Complete durable source authority retained by libpkgstate.
 * \return Planner-owned candidate control with every representable fact.
 * \throws projection_error with control_translation when vocabularies differ.
 */
[[nodiscard]] PKGSTATE_PLAN_API pkgplan::candidate_control_projection
project_candidate_control(const package_source_record& source);

/*!
 * \brief Caller-authoritative planner target and its durable state projection.
 *
 * libpkgstate cannot derive a complete planner target-system context from an
 * installed snapshot.  The caller supplies that context identity together with
 * the exact durable state projection it claims belongs to the context.
 */
class PKGSTATE_PLAN_API planning_target_context final {
public:
  /*!
   * \brief Bind a planner context to its durable state projection.
   * \param identity Caller-authoritative complete planner target context.
   * \param state_projection Exact durable state target represented by it.
   */
  planning_target_context(
      pkgplan::target_system_context_identity identity,
      state_target_binding state_projection);

  /*!
   * \brief Return the caller-authoritative planner context identity.
  *  \return The caller-authoritative planner context identity.
   */
  [[nodiscard]] const pkgplan::target_system_context_identity&
  identity() const noexcept;

  /*!
   * \brief Return the claimed durable state projection.
  *  \return The claimed durable state projection.
   */
  [[nodiscard]] const state_target_binding&
  state_projection() const noexcept;

private:
  pkgplan::target_system_context_identity identity_;
  state_target_binding state_projection_;
};

/*!
 * \brief Planner-owned fact projection of one complete canonical snapshot.
 *
 * Package facts remain in snapshot package-name order and carry complete
 * planner-relevant durable control. Ownership claims are normalized by
 * libpkgplan and carry every recorded object field represented in planner
 * vocabulary. Hard-link topology remains state-owned because libpkgplan has
 * no corresponding object-metadata field.
 */
class PKGSTATE_PLAN_API installed_state_projection final {
public:
  /*!
   * \brief Return the caller-authoritative complete target context.
  *  \return The caller-authoritative complete target context.
   */
  [[nodiscard]] const pkgplan::target_system_context_identity&
  target() const noexcept;

  /*!
   * \brief Return installed package facts in package-name order.
  *  \return Installed package facts in package-name order.
   */
  [[nodiscard]] const std::vector<pkgplan::installed_package_fact>&
  packages() const noexcept;

  /*!
   * \brief Return the complete installed path-to-owner relation.
  *  \return The complete installed path-to-owner relation.
   */
  [[nodiscard]] const pkgplan::installed_ownership_inventory&
  ownership() const noexcept;

private:
  /*! \brief Construct the projection through project_installed_state(). */
  friend PKGSTATE_PLAN_API installed_state_projection project_installed_state(
      const snapshot&, const planning_target_context&);

  installed_state_projection(
      pkgplan::target_system_context_identity target,
      std::vector<pkgplan::installed_package_fact> packages,
      pkgplan::installed_ownership_inventory ownership);

  pkgplan::target_system_context_identity target_;
  std::vector<pkgplan::installed_package_fact> packages_;
  pkgplan::installed_ownership_inventory ownership_;
};

/*!
 * \brief Project complete canonical installed truth into libpkgplan facts.
 *
 * The function accepts only native pkgstate::snapshot authority. It
 * verifies the caller's durable target projection before copying canonical
 * identity representations and projecting planner-relevant durable control
 * into the matching planner domains.
 *
 * \param state Complete canonical installed-state snapshot.
 * \param target Caller-authoritative planner target and matching state binding.
 * \return Planner-owned package and ownership facts for \p state.
 * \throws projection_error when the target or vocabularies disagree.
 */
[[nodiscard]] PKGSTATE_PLAN_API installed_state_projection
project_installed_state(const snapshot& state,
                        const planning_target_context& target);

} // namespace pkgstate::plan_adapter
