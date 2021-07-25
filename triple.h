//
// Created by robert on 25/07/2021.
//

#ifndef _TRIPLE_H_
#define _TRIPLE_H_

/// A 3-tuple.
/// \tparam T1 The type of the first element.
/// \tparam T2 The type of the second element.
/// \tparam T3 The type of the third element.
template <typename T1, typename T2, typename T3>
struct triple
{
  T1 first;
  T2 second;
  T3 third;
};

#endif //_TRIPLE_H_
