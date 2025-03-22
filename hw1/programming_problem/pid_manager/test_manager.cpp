#include "pid_manager.hpp"
#include <gtest/gtest.h>

// 測試 allocate_map 函數
TEST(PidManagerTest, AllocateMap) {
    pid_manager pm;
    EXPECT_EQ(pm.allocate_map(), 1);
}

// 測試 allocate_pid 和 release_pid 函數
TEST(PidManagerTest, AllocateAndReleasePid) {
    pid_manager pm;
    pm.allocate_map();

    int pid1 = pm.allocate_pid();
    EXPECT_GE(pid1, MIN_PID);
    EXPECT_LE(pid1, MAX_PID);

    pm.release_pid(pid1);

    int pid2 = pm.allocate_pid();
    EXPECT_EQ(pid1, pid2);
}

// 測試當所有 PID 都被分配後的情況
TEST(PidManagerTest, ExhaustPids) {
    pid_manager pm;
    pm.allocate_map();

    // 分配所有可用的 PID
    for (int i = 0; i <= MAX_PID - MIN_PID; ++i) {
        EXPECT_NE(pm.allocate_pid(), -1);
    }

    // 此時應該無法再分配新的 PID
    EXPECT_EQ(pm.allocate_pid(), -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
