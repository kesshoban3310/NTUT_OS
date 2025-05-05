import java.util.*;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.locks.ReentrantLock;

// PID 管理器類別
class pid_manager {
    private final int MIN_PID = 300;
    private final int MAX_PID = 5000;
    private Vector<Boolean> man;
    private final ReentrantLock lock = new ReentrantLock(); // ✅ 加入鎖

    int allocate_map() {
        try {
            man = new Vector<>(MAX_PID - MIN_PID + 1);
            for (int i = 0; i < (MAX_PID - MIN_PID + 1); i++) {
                man.add(false);
            }
            return 1;
        } catch (Exception e) {
            return -1;
        }
    }

    int allocate_pid() {
        lock.lock(); // ✅ 加鎖
        try {
            for (int i = 0; i < man.size(); i++) {
                if (!man.get(i)) {
                    man.set(i, true);
                    return i + MIN_PID;
                }
            }
            return -1;
        } finally {
            lock.unlock(); // ✅ 解鎖
        }
    }

    void release_pid(int pid) {
        if (pid > MAX_PID || pid < MIN_PID) {
            throw new RuntimeException("PID out of range");
        }
        lock.lock(); // ✅ 加鎖
        try {
            man.set(pid - MIN_PID, false);
        } finally {
            lock.unlock(); // ✅ 解鎖
        }
    }

    boolean state(int pid) {
        if (pid > MAX_PID || pid < MIN_PID) {
            throw new RuntimeException("PID out of range");
        }
        lock.lock(); // ✅ 加鎖
        try {
            return man.get(pid - MIN_PID);
        } finally {
            lock.unlock(); // ✅ 解鎖
        }
    }
}

// 主程式
public class pid {
    static class Sleep extends Thread {
        int id;

        Sleep(int id) {
            this.id = id;
        }

        @Override
        public void run() {
            int sleepTime = ThreadLocalRandom.current().nextInt(1, 6); // sleep 1~5 秒
            try {
                Thread.sleep(sleepTime * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Thread " + id + " slept " + sleepTime + " seconds.");
        }
    }

    public static void main(String[] args) throws InterruptedException {
        if (args.length != 1) {
            System.err.println("Usage: java pid <thread_number>");
            System.exit(1);
        }

        int thread_number = Integer.parseInt(args[0]);

        pid_manager manager = new pid_manager();
        if (manager.allocate_map() == -1) {
            System.err.println("Failed to initialize PID map.");
            System.exit(1);
        }

        System.out.println("Start Execute!");

        Sleep[] threads = new Sleep[thread_number];
        for (int i = 0; i < thread_number; i++) {
            int pid = manager.allocate_pid();
            if (pid == -1) {
                System.out.println("No PID available for Thread " + i);
                continue;
            }
            threads[i] = new Sleep(pid);
            threads[i].start();
        }

        for (int i = 0; i < thread_number; i++) {
            if (threads[i] != null) {
                threads[i].join();
                manager.release_pid(threads[i].id);
            }
        }

        // 檢查是否還有遺留的 PID（理論上都該釋放）
        for (int i = 0; i < thread_number; i++) {
            if (threads[i] != null && manager.state(threads[i].id)) {
                System.out.println("Thread with PID " + threads[i].id + " is still marked as active!");
            }
        }

        System.out.println("End Execute!");
    }
}
