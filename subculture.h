//
// Created by robert on 22/07/2021.
//

#ifndef MOTIVATE_CPP_SUBCULTURE_H
#define MOTIVATE_CPP_SUBCULTURE_H

#include <string>
#include <map>
#include "transport_mode.h"

/// A demographic subculture
class subculture
{
private:
  /// The ID of the subculture
  std::string id;

  /// The desirability is a score from 0 to 1 for each transport mode.
  std::map<transport_mode, float> desirability;

public:
  /// Gets the ID of the subculture
  /// \return The ID of the subculture.
  [[nodiscard]] const std::string &get_id() const;

  /// Gets the desirability of the subculture. This is a score from 0 to 1 for each transport mode.
  /// \return The desirability of the subculture.
  [[nodiscard]] const std::map<transport_mode, float> &get_desirability() const;

  /// Sets the desirability of the subculture. This is a score from 0 to 1 for each transport mode.
  /// \param desirability The new desirability.
  void set_desirability(const std::map<transport_mode, float> &desirability);

  /// Creates a new demographic subculture.
  /// \param id The ID of the new subculture
  /// \param desirability The desirability of the new subculture. This is a score from 0 to 1 for each transport mode.
  subculture(const std::string &id, const std::map<transport_mode, float> &desirability);

  /// Compares two subcultures for equality.
  /// \param rhs The comparison subculture
  /// \return True iff their IDs are equal
  bool operator==(const subculture &rhs) const;

  /// Compares two subcultures for inequality.
  /// \param rhs The comparison subculture
  /// \return True iff their IDs are not equal
  bool operator!=(const subculture &rhs) const;
};

#endif //MOTIVATE_CPP_SUBCULTURE_H
