import java.util.*;

public class DiskScheduler {
    static final int CYLINDER_MIN = 0;
    static final int CYLINDER_MAX = 4999;
    static final int REQUEST_COUNT = 1000;

    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java DiskScheduler <initial_head_position>");
            return;
        }

        int head = Integer.parseInt(args[0]);
        if (head < CYLINDER_MIN || head > CYLINDER_MAX) {
            System.out.println("Initial head position must be between 0 and 4999.");
            return;
        }

        // Generate random cylinder requests


        Random rand = new Random();
        List<Integer> requests = new ArrayList<>();
        for (int i = 0; i < REQUEST_COUNT; i++) {
            requests.add(rand.nextInt(CYLINDER_MAX + 1));
        }

        System.out.println("Initial head position: " + head);
        System.out.println("FCFS Total Head Movement: " + fcfs(new ArrayList<>(requests), head));
        System.out.println("SCAN Total Head Movement: " + scan(new ArrayList<>(requests), head));
        System.out.println("C-SCAN Total Head Movement: " + cscan(new ArrayList<>(requests), head));
    }

    // FCFS: First-Come, First-Served
    static int fcfs(List<Integer> requests, int head) {
        int totalMovement = 0;
        for (int r : requests) {
            totalMovement += Math.abs(r - head);
            head = r;
        }
        return totalMovement;
    }

    // SCAN: Elevator algorithm (assume moving toward higher cylinders)
    static int scan(List<Integer> requests, int head) {
        int totalMovement = 0;
        List<Integer> left = new ArrayList<>();
        List<Integer> right = new ArrayList<>();

        for (int r : requests) {
            if (r < head) left.add(r);
            else right.add(r);
        }

        Collections.sort(left, Collections.reverseOrder());
        Collections.sort(right);

        for (int r : right) {
            totalMovement += Math.abs(r - head);
            head = r;
        }

        if (!left.isEmpty()) {
            totalMovement += Math.abs(CYLINDER_MAX - head); // move to end
            head = CYLINDER_MAX;
            for (int r : left) {
                totalMovement += Math.abs(r - head);
                head = r;
            }
        }

        return totalMovement;
    }

    // C-SCAN: Circular SCAN (only moves in one direction)
    static int cscan(List<Integer> requests, int head) {
        int totalMovement = 0;
        List<Integer> left = new ArrayList<>();
        List<Integer> right = new ArrayList<>();

        for (int r : requests) {
            if (r < head) left.add(r);
            else right.add(r);
        }

        Collections.sort(right);
        Collections.sort(left);

        for (int r : right) {
            totalMovement += Math.abs(r - head);
            head = r;
        }

        // move to end then wrap around
        if (!left.isEmpty()) {
            totalMovement += Math.abs(CYLINDER_MAX - head); // to 4999
            totalMovement += CYLINDER_MAX; // jump from 4999 to 0
            head = 0;

            for (int r : left) {
                totalMovement += Math.abs(r - head);
                head = r;
            }
        }

        return totalMovement;
    }
}
