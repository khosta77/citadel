#ifndef DATAFRAME_HPP_
#define DATAFRAME_HPP_

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

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), Setting,
	( int, period )
)

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), Metric,
	( std::string, type )
    ( std::vector<std::string>, spec )
    ( std::vector<int>, ids )
)

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), Output,
	( std::string, type )
	( std::string, path )
	( std::string, ip )
    ( int, port )
)

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), SystemSetting,
    ( pkg::Setting, setting )
    ( std::vector<pkg::Metric>, metrics )
    ( std::vector<pkg::Output>, outputs )
)

// Структура для возвращения данных о системе
BOOST_FUSION_DEFINE_STRUCT(
    (pkg), SystemInfo,
    ( std::vector<int>, cpu )
    ( std::vector<int>, memory )
)

#endif  // DATAFRAME_HPP_
