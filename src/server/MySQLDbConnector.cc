#include "MySQLDbConnector.h"
#include "Utils.h"

usersystem::UserModel MySQLDbConnector::CreateUser(const usersystem::UserModel &from) {
    return create(TABLE_USER, from);
}

usersystem::ResponseCode MySQLDbConnector::UpdateUser(const usersystem::UserModel &from) {
    return usersystem::ResponseCode::OK;
}

usersystem::UserModel MySQLDbConnector::FetchUser(const std::string &username) {
    auto schema = _session.createSchema(DB_NAME, true);
    auto collections = schema.createCollection(TABLE_USER, true);
    auto result = collections.find("username = \"" + username + "\"").execute();
    if (result.count() > 0) {
        return Utils::UserModelFromDbDoc(result.fetchOne());
    }
    return usersystem::UserModel();
}

usersystem::ResponseCode MySQLDbConnector::RemoveUser(const std::string &username) {
    return usersystem::ResponseCode::OK;
}

usersystem::UserLoginHistoryModel MySQLDbConnector::CreateLoginHistory(const usersystem::UserLoginHistoryModel &from) {
    return create(TABLE_USER_LOGIN_HISTORY, from);
}

usersystem::ResponseCode MySQLDbConnector::UpdateLoginHistory(const usersystem::UserLoginHistoryModel &from) {
    try {
        auto schema = _session.createSchema(DB_NAME, true);
        auto collections = schema.createCollection(TABLE_USER_LOGIN_HISTORY, true);
        auto result = collections.addOrReplaceOne(from.id(), mysqlx::Value(Utils::ToJsonString(from)));
        std::cout << "Update " << result.getAffectedItemsCount() << " items" << std::endl;
    } catch (const mysqlx::Error& error) {
        std::cerr << error.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    } catch (const char* ex) {
        std::cerr << ex << std::endl;
    }
    return usersystem::ResponseCode::OK;
}

usersystem::UserLoginHistoryModel MySQLDbConnector::FetchLoginHistory(const std::string &username) {
    try {
        auto schema = _session.createSchema(DB_NAME, true);
        auto collections = schema.createCollection(TABLE_USER_LOGIN_HISTORY, true);
        auto result = collections.find("username = :un").sort("create_at_millis DESC")
                .limit(1).bind("un", mysqlx::Value(username)).execute();
        if (result.count() > 0) {
            return Utils::UserLoginHistoryModelFromDbDoc(result.fetchOne());
        }
    } catch (const mysqlx::Error& error) {
        std::cerr << error.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    } catch (const char* ex) {
        std::cerr << ex << std::endl;
    }

    return usersystem::UserLoginHistoryModel();
}

usersystem::ResponseCode MySQLDbConnector::RemoveUserLoginHistory(const std::string &historyId) {
    return usersystem::ResponseCode::OK;
}
