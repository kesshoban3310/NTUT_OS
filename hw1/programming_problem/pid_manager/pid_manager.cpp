#include "pid_manager.hpp"

pid_manager::pid_manager(){
    
}
int pid_manager::allocate_map(void){
    try {
        pid.resize(MAX_PID - MIN_PID + 1, 0);  // 初始化 bitmap
    } catch (std::bad_alloc &e) {
        return -1;  // 分配失敗
    }
    return 1;  
}
int pid_manager::allocate_pid(void){
    for (int i = 0; i < pid.size(); ++i) {
        if (pid[i] == 0) {
            pid[i] = 1;
            return i + MIN_PID;
        }
    }
    return -1;

}
void pid_manager::release_pid(int pid){
    if (pid < MIN_PID || pid > MAX_PID) return;
    this->pid[pid - MIN_PID] = 0;
}