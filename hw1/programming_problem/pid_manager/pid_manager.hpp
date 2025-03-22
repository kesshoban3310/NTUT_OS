#ifndef PID_MANAGER_HPP
#define PID_MANAGER_HPP

#define MIN_PID 300
#define MAX_PID 5000
#include <vector>

class pid_manager
{
private:
    /* data */
    std::vector<int> pid;
public:
    pid_manager();
    int allocate_map(void);
    int allocate_pid(void);
    void release_pid(int pid);
};

#endif