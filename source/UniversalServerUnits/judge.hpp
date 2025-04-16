#ifndef JUDGE_HPP_
#define JUDGE_HPP_

#include <typeinfo>
#include <iostream>
#include <cassert>
#include <string>

#include "json/single_include/nlohmann/json.hpp"
using Json = nlohmann::json;

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/struct.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/type_index.hpp> 
#include <boost/mpl/vector.hpp>
#include <boost/type_index.hpp>

template<typename T>
using is_fusion_struct = std::is_same<typename boost::fusion::traits::tag_of<T>::type, boost::fusion::struct_tag>;

template<typename T>
struct is_valid_t : std::false_type {};

template<>
struct is_valid_t<int> : std::true_type {};

template<>
struct is_valid_t<float> : std::true_type {};

template<>
struct is_valid_t<std::string> : std::true_type {};

template<typename T>
struct is_valid_ts : std::disjunction<is_valid_t<T>, is_fusion_struct<T>> {};

template<typename T>
struct is_valid_vector_t : std::false_type {};

template<typename T>
struct is_valid_vector_t<std::vector<T>> : is_valid_ts<T> {};

template<typename T>
struct is_valid : std::disjunction<is_valid_ts<T>, is_valid_vector_t<T>> {};

#endif  // JUDGE_HPP_
