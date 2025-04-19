import java.util.*;
import java.util.concurrent.ThreadLocalRandom;

class pid_manager{
    private final int MIN_PID = 300;
    private final int MAX_PID = 5000;
    private Vector<Boolean> man;
    int allocate_map() {
        try {
            man = new Vector<>(MAX_PID - MIN_PID + 1);
            for (int i = 0; i < man.capacity(); i++) {
                man.add(false);
            }
            return 1;
        } catch (Exception e) {
            return -1;
        }
    }
    int allocate_pid(){
        for (int i = 0; i < man.size(); i++) {
            if (!man.get(i)) {
                man.set(i, true);
                return i + MIN_PID;
            }
        }
        return -1;
    }
    void release_pid(int pid){
        if(pid > MAX_PID || pid < MIN_PID){
            throw new RuntimeException("PID out of range");
        }
        this.man.set(pid - MIN_PID, false);
    }

    boolean state(int pid){
        if(pid > MAX_PID || pid < MIN_PID){
            throw new RuntimeException("PID out of range");
        }
        return man.get(pid - MIN_PID);
    }

}


public class pid {
    static class Sleep extends Thread{
        int id;

        Sleep(int id){
            this.id = id;
        }

        @Override
        public void run(){
            int sleepTime = ThreadLocalRandom.current().nextInt(1, 6); // sleep 1~4 sec
            try {
                Thread.sleep(sleepTime * 1000); // Java 的 sleep 以毫秒為單位
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Thread " + id + " sleep " + sleepTime + " seconds.");
        }
    }

    public static void main(String args[]) throws InterruptedException{
        if(args.length != 1){
            System.err.println("Usage: java PID <thread number>");
            System.exit(1);
        }
        int thread_number = Integer.parseInt(args[0]);

        pid_manager manager = new pid_manager();
        manager.allocate_map();

        System.out.println("Start Execute!");

        Sleep[] threads = new Sleep[thread_number];
        for(int i = 0; i < thread_number; i++){
            threads[i] = new Sleep(manager.allocate_pid());
            threads[i].start();
        }

        for(int i = 0; i < thread_number; i++){
            threads[i].join();
            manager.release_pid(threads[i].id);
        }

        for(int i = 0; i < thread_number; i++){
            if(manager.state(threads[i].id)){
                System.out.println("Thread with PID " + threads[i].id + " is still running!");
            }
        }

        System.out.println("End Execute!");
    }

}
