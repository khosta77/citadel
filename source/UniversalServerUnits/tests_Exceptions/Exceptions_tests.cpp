#include "Exceptions.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

TEST(MyExceptionTest, ConstructorAndWhat) {
    const std::string error_msg = "Test error message";
    MyException ex(error_msg);

    // Проверяем, что what() возвращает правильное сообщение
    EXPECT_STREQ(ex.what(), error_msg.c_str());

    // Проверяем, что сообщение точно совпадает
    EXPECT_EQ(std::string(ex.what()), error_msg);
}

TEST(MyExceptionTest, ThrowsCorrectly) {
    const std::string error_msg = "Exception thrown";

    // Проверяем, что исключение бросается с правильным сообщением
    EXPECT_THROW({
        throw MyException(error_msg);
    }, MyException);

    // Проверяем конкретное сообщение в исключении
    try {
        throw MyException(error_msg);
        FAIL() << "Expected MyException to be thrown";
    } catch (const MyException& ex) {
        EXPECT_STREQ(ex.what(), error_msg.c_str());
    } catch (...) {
        FAIL() << "Expected MyException but got different exception";
    }
}

TEST(MyExceptionTest, NoexceptSpecification) {
    // Проверяем, что метод what() действительно noexcept
    MyException ex("Test");
    EXPECT_TRUE(noexcept(ex.what()));
}

TEST(MyExceptionTest, EmptyMessage) {
    MyException ex("");
    EXPECT_STREQ(ex.what(), "");
}

TEST(MyExceptionTest, CopyConstructor) {
    MyException ex1("Original");
    MyException ex2 = ex1;

    EXPECT_STREQ(ex1.what(), ex2.what());
}

TEST(MyExceptionTest, MoveConstructor) {
    MyException ex1("Original");
    MyException ex2 = std::move(ex1);

    EXPECT_STREQ(ex2.what(), "Original");
}

// Базовые тесты для ErrorReadingFromSocket
TEST(ErrorReadingFromSocketTest, BasicFunctionality) {
    const int test_socket = 42;
    ErrorReadingFromSocket ex(test_socket);

    // Проверяем сообщение
    EXPECT_STREQ(ex.what(), "In \'UniversalServerMethods::readFromSock\', something error");

    // Проверяем поле sock
    EXPECT_EQ(ex.sock, test_socket);

    // Проверяем наследование
    EXPECT_TRUE(dynamic_cast<MyException*>(&ex));
    EXPECT_TRUE(dynamic_cast<std::exception*>(&ex));
}

TEST(ErrorReadingFromSocketTest, ThrowAndCatch) {
    const int test_socket = 123;

    try {
        throw ErrorReadingFromSocket(test_socket);
        FAIL() << "Expected ErrorReadingFromSocket to be thrown";
    }
    catch (const ErrorReadingFromSocket& ex) {
        EXPECT_EQ(ex.sock, test_socket);
        EXPECT_STREQ(ex.what(), "In \'UniversalServerMethods::readFromSock\', something error");
    }
    catch (...) {
        FAIL() << "Expected ErrorReadingFromSocket but got different exception";
    }
}

// Тесты для NotCorrectMessageToSend
TEST(NotCorrectMessageToSendTest, BasicFunctionality) {
    NotCorrectMessageToSend ex;

    // Проверяем сообщение
    EXPECT_STREQ(ex.what(), "Not correct message to send");

    // Проверяем наследование
    EXPECT_TRUE(dynamic_cast<MyException*>(&ex));
    EXPECT_TRUE(dynamic_cast<std::exception*>(&ex));
}

TEST(NotCorrectMessageToSendTest, ThrowAndCatch) {
    try {
        throw NotCorrectMessageToSend();
        FAIL() << "Expected NotCorrectMessageToSend to be thrown";
    }
    catch (const NotCorrectMessageToSend& ex) {
        EXPECT_STREQ(ex.what(), "Not correct message to send");
    }
    catch (...) {
        FAIL() << "Expected NotCorrectMessageToSend but got different exception";
    }
}

// Тесты для ErrorWritingToSocket
TEST(ErrorWritingToSocketTest, BasicFunctionality) {
    const int test_socket = 99;
    ErrorWritingToSocket ex(test_socket);

    // Проверяем сообщение
    EXPECT_STREQ(ex.what(), "In \'UniversalServerMethods::writeToSock\', something error");

    // Проверяем поле sock
    EXPECT_EQ(ex.sock, test_socket);

    // Проверяем наследование
    EXPECT_TRUE(dynamic_cast<MyException*>(&ex));
    EXPECT_TRUE(dynamic_cast<std::exception*>(&ex));
}

TEST(ErrorWritingToSocketTest, ThrowAndCatch) {
    const int test_socket = 456;

    try {
        throw ErrorWritingToSocket(test_socket);
        FAIL() << "Expected ErrorWritingToSocket to be thrown";
    }
    catch (const ErrorWritingToSocket& ex) {
        EXPECT_EQ(ex.sock, test_socket);
        EXPECT_STREQ(ex.what(), "In \'UniversalServerMethods::writeToSock\', something error");
    }
    catch (...) {
        FAIL() << "Expected ErrorWritingToSocket but got different exception";
    }
}

TEST(ExceptionCopyTest, CopyBehavior) {
    const int test_socket = 789;
    ErrorReadingFromSocket ex1(test_socket);
    ErrorReadingFromSocket ex2 = ex1;

    EXPECT_EQ(ex2.sock, test_socket);
    EXPECT_STREQ(ex2.what(), ex1.what());
}

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
