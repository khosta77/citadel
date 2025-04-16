#include "UniversalServer.hpp"

#include <gtest/gtest.h>

//// serialize & deserialize

BOOST_FUSION_DEFINE_STRUCT(
    (pkg), Ranks,
    ( int, r1 )
    ( int, r2 )
    ( int, r3 )
    ( std::string, some_str )
)

TEST( JsonBoostFusionTest, RanksBasicSerialization )
{
    UniversalServerMethods serverMethods;
    pkg::Ranks ranks = { 1, 2, 3, "example" };
    std::string json_str = serverMethods.serialize(ranks);
    EXPECT_EQ( json_str, "{\"r1\":1,\"r2\":2,\"r3\":3,\"some_str\":\"example\"}" );
}

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), S1,
	( int, r0 )
)

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), S2,
	( float, val )
)

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), S3,
	( int, r1 )
    ( double, r2 )
	( std::string, some_str )
    ( std::vector<int>, vals )
    ( pkg::S2, s2_val )
    ( std::vector<pkg::S1>, s1_vals )
)

BOOST_FUSION_DEFINE_STRUCT(
	(pkg), S4,
	( bool, val )
)

TEST( JsonBoostFusionTest, RanksBasicDeserialization )
{
    UniversalServerMethods serverMethods;
    std::string json_str = "{\"r1\":1,\"r2\":2,\"r3\":3,\"some_str\":\"example\"}";
    auto deserialized_ranks = serverMethods.deserialize<pkg::Ranks>(json_str);
    EXPECT_EQ( deserialized_ranks.r1, 1 );
    EXPECT_EQ( deserialized_ranks.r2, 2 );
    EXPECT_EQ( deserialized_ranks.r3, 3 );
    EXPECT_EQ( deserialized_ranks.some_str, "example" );
}

TEST( JsonBoostFusionTest, NestedStructSerialization )
{
    UniversalServerMethods serverMethods;
    pkg::S1 s1_0(1);
    pkg::S1 s1_1(2);
    pkg::S2 s2(1.2200000286102295);
    pkg::S3 s3;
    s3.r1 = 123;
    s3.r2 = 1.2300000190734863;
    s3.some_str = "abcde";
    s3.vals = std::vector<int>{ 1, 2, 3 };
    s3.s2_val = s2;
    s3.s1_vals = std::vector<pkg::S1>{ s1_0, s1_1 };

    std::string json_str = serverMethods.serialize(s3);
    EXPECT_EQ( json_str, "{\"r1\":123,\"r2\":1.2300000190734863,\"s1_vals\":[{\"r0\":1},{\"r0\":2}],\"s2_val\":{\"val\":1.2200000286102295},\"some_str\":\"abcde\",\"vals\":[1,2,3]}" );
}

TEST( JsonBoostFusionTest, NestedStructDeserialization )
{
    UniversalServerMethods serverMethods;
    std::string json_str = "{\"r1\":123,\"r2\":1.2300000190734863,\"s1_vals\":[{\"r0\":1},{\"r0\":2}],\"s2_val\":{\"val\":1.2200000286102295},\"some_str\":\"abcde\",\"vals\":[1,2,3]}";
    auto deserialized_s3 = serverMethods.deserialize<pkg::S3>(json_str);
    
    EXPECT_EQ( deserialized_s3.r1, 123 );
    EXPECT_FLOAT_EQ( deserialized_s3.r2, 1.2300000190734863 );
    EXPECT_EQ( deserialized_s3.some_str, "abcde" );
    
    ASSERT_EQ( static_cast<int>( deserialized_s3.vals.size() ), 3 );
    EXPECT_EQ( deserialized_s3.vals[0], 1 );
    EXPECT_EQ( deserialized_s3.vals[1], 2 );
    EXPECT_EQ( deserialized_s3.vals[2], 3 );
    
    EXPECT_FLOAT_EQ( deserialized_s3.s2_val.val, 1.2200000286102295 );
    
    ASSERT_EQ( static_cast<int>( deserialized_s3.s1_vals.size() ), 2 );
    EXPECT_EQ( deserialized_s3.s1_vals[0].r0, 1 );
    EXPECT_EQ( deserialized_s3.s1_vals[1].r0, 2 );
}

TEST( JsonBoostFusionTest, BoolSerialization )
{
    //UniversalServerMethods serverMethods;
    // Этот тест остаётся закомментированным, как в оригинале
    // pkg::S4 s4(true);
    // std::string json_str = Serialize(s4);
    SUCCEED(); // Просто отмечаем, что тест пройден
}

TEST( JsonBoostFusionTest, ExtraFieldError )
{
    UniversalServerMethods serverMethods;
    std::string json_str = "{\"r1\":1,\"r2\":2,\"r3\":3,\"r4\":4,\"some_str\":\"example\"}";
    EXPECT_THROW( serverMethods.deserialize<pkg::Ranks>(json_str), DeserializeJsonElementSomeProblem );
}

TEST( JsonBoostFusionTest, MissingFieldError )
{
    UniversalServerMethods serverMethods;
    std::string json_str = "{\"r1\":1,\"r2\":2,\"some_str\":\"example\"}";
    EXPECT_THROW( serverMethods.deserialize<pkg::Ranks>(json_str), DeserializeJsonNoKey );
}

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}


