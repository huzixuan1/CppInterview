// UT（Unit Testing），即 单元测试，是指在软件开发过程中，针对程序中最小的可测试单元（通常是函数或方法）进行的独立测试。
// 其目的是验证单个组件是否按照预期工作。单元测试通常是自动化的，通常会使用特定的测试框架来编写和执行

主要特点：
测试最小单元：单元测试关注的是程序中的最小单元（如函数或方法），它会验证某个函数或方法是否按照预期的行为执行。
隔离性：单元测试通常是独立于其他系统或模块的，即便是一个数据库或者文件系统的依赖，也可以使用模拟（Mock）或打桩（Stub）来隔离外部依赖。
自动化：单元测试是自动化的，这意味着每次修改代码后，开发人员可以运行所有的单元测试来检查新代码是否破坏了现有功能。

单元测试的作用：
确保代码质量：单元测试可以帮助开发者发现代码中的错误，确保新开发的功能不会破坏现有的功能。
便于重构代码：如果你对代码进行重构或优化，单元测试可以帮助你确保代码的行为没有发生变化。
帮助设计代码：通过编写单元测试，开发者在开发时往往能够更清晰地思考函数的功能与边界，促进更好的代码设计。
提高开发效率：通过自动化的方式，单元测试可以减少手动测试的工作量，尤其是在回归测试和集成阶段。
文档化功能：单元测试可以作为文档来说明代码的行为，尤其是在团队合作时，别人可以通过测试用例理解代码的预期行为。

打桩案例：
// src/Database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database {
public:
    virtual bool connect(const std::string& connectionString) {
        // 假设我们真实的数据库连接代码在这里
        return true;
    }

    virtual void executeQuery(const std::string& query) {
        // 真实的查询执行代码
    }

    virtual ~Database() = default;
};

#endif // DATABASE_H


// src/ProcessData.cpp
#include "Database.h"
#include <iostream>

void processData(Database& db, const std::string& query) {
    if (db.connect("my_database_connection_string")) {
        db.executeQuery(query);
        std::cout << "Data processed!" << std::endl;
    }
}


// test/StubDatabase.h
#include "../src/Database.h"

class StubDatabase : public Database {
public:
    bool connect(const std::string& connectionString) override {
        // 打桩实现：这里假装成功连接数据库
        return true; // 假设连接总是成功
    }

    void executeQuery(const std::string& query) override {
        // 打桩实现：假装执行了查询
        // 我们可以记录传入的查询内容，以便后续验证
        lastQuery = query;
    }

    std::string getLastQuery() const {
        return lastQuery;
    }

private:
    std::string lastQuery;  // 记录最后一次执行的查询
};


// test/testProcessData.cpp
#include <gtest/gtest.h>
#include "../src/ProcessData.cpp"
#include "../src/StubDatabase.h"

// 测试 processData 函数
TEST(ProcessDataTest, TestProcessDataWithStubDatabase) {
    StubDatabase db;

    // 调用 processData 函数，实际上不会执行任何数据库操作
    processData(db, "SELECT * FROM users");

    // 验证 executeQuery 是否被正确调用了，并且使用了正确的查询
    EXPECT_EQ(db.getLastQuery(), "SELECT * FROM users");
}


// Mock的案例

// src/Database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database {
public:
    virtual bool connect(const std::string& connectionString) {
        // 假设我们真实的数据库连接代码在这里
        return true;
    }

    virtual void executeQuery(const std::string& query) {
        // 真实的查询执行代码
    }

    virtual ~Database() = default;
};

#endif // DATABASE_H


// src/ProcessData.cpp
#include "Database.h"
#include <iostream>

void processData(Database& db, const std::string& query) {
    if (db.connect("my_database_connection_string")) {
        db.executeQuery(query);
        std::cout << "Data processed!" << std::endl;
    }
}


// test/MockDatabase.h
#include <gmock/gmock.h>
#include "../src/Database.h"

class MockDatabase : public Database {
public:
    MOCK_METHOD(bool, connect, (const std::string& connectionString), (override));
    MOCK_METHOD(void, executeQuery, (const std::string& query), (override));
};


// test/testProcessData.cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/ProcessData.cpp"
#include "../src/MockDatabase.h"

using namespace testing;

// 测试 processData 函数
TEST(ProcessDataTest, TestProcessDataWithMockDatabase) {
    // 创建 MockDatabase 实例
    MockDatabase db;

    // 设置预期的行为：连接函数应该被调用并且返回 true
    EXPECT_CALL(db, connect("my_database_connection_string"))
        .WillOnce(Return(true));  // 模拟 connect 返回 true

    // 设置预期的行为：executeQuery 函数应该被调用并且接收指定的查询
    EXPECT_CALL(db, executeQuery("SELECT * FROM users"))
        .Times(1);  // 确保 executeQuery 被调用一次

    // 调用 processData 函数
    processData(db, "SELECT * FROM users");
}

