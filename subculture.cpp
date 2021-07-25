//
// Created by robert on 22/07/2021.
//

#include "subculture.h"

subculture::subculture(const std::string &id, const std::map<transport_mode, float> &desirability) : id(id),
                                                                                                     desirability(
                                                                                                         desirability)
{
}

const std::string &subculture::get_id() const
{
  return id;
}

const std::map<transport_mode, float> &subculture::get_desirability() const
{
  return desirability;
}

void subculture::set_desirability(const std::map<transport_mode, float> &desirability)
{
  subculture::desirability = desirability;
}

bool subculture::operator==(const subculture &rhs) const
{
  return id == rhs.id;
}

bool subculture::operator!=(const subculture &rhs) const
{
  return !(rhs == *this);
}
