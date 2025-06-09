import java.util.*;

public class PageScheduler {
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java PageScheduler <number_of_frames>");
            return;
        }

        int numFrames = Integer.parseInt(args[0]);
        Random rand = new Random();
        int length = rand.nextInt(21) + 10;  // 產生 10～30 的隨機整數
        int[] referenceString = generateReferenceString(length);

        System.out.println("Reference String: " + Arrays.toString(referenceString));
        System.out.println("Frames: " + numFrames);

        System.out.println("FIFO Faults: " + simulateFIFO(referenceString, numFrames));
        System.out.println("LRU Faults:  " + simulateLRU(referenceString, numFrames));
        System.out.println("OPT Faults:  " + simulateOPT(referenceString, numFrames));
    }

    private static int[] generateReferenceString(int length) {
        Random rand = new Random();
        int[] result = new int[length];
        for (int i = 0; i < length; i++) {
            result[i] = rand.nextInt(10);
        }
        return result;
    }

    private static int simulateFIFO(int[] refs, int frames) {
        Set<Integer> memory = new HashSet<>();
        Queue<Integer> queue = new LinkedList<>();
        int faults = 0;

        for (int ref : refs) {
            if (!memory.contains(ref)) {
                faults++;
                if (memory.size() == frames) {
                    int removed = queue.poll();
                    memory.remove(removed);
                }
                memory.add(ref);
                queue.add(ref);
            }
        }
        return faults;
    }

    private static int simulateLRU(int[] refs, int frames) {
        Set<Integer> memory = new HashSet<>();
        Map<Integer, Integer> recentUse = new HashMap<>();
        int faults = 0;

        for (int i = 0; i < refs.length; i++) {
            int ref = refs[i];
            if (!memory.contains(ref)) {
                faults++;
                if (memory.size() == frames) {
                    int lru = Collections.min(recentUse.entrySet(), Map.Entry.comparingByValue()).getKey();
                    memory.remove(lru);
                    recentUse.remove(lru);
                }
                memory.add(ref);
            }
            recentUse.put(ref, i);
        }
        return faults;
    }

    private static int simulateOPT(int[] refs, int frames) {
        Set<Integer> memory = new HashSet<>();
        int faults = 0;

        for (int i = 0; i < refs.length; i++) {
            int ref = refs[i];
            if (!memory.contains(ref)) {
                faults++;
                if (memory.size() == frames) {
                    int pageToRemove = -1;
                    int farthestUse = -1;

                    for (int page : memory) {
                        int nextUse = Integer.MAX_VALUE;
                        for (int j = i + 1; j < refs.length; j++) {
                            if (refs[j] == page) {
                                nextUse = j;
                                break;
                            }
                        }
                        if (nextUse > farthestUse) {
                            farthestUse = nextUse;
                            pageToRemove = page;
                        }
                    }
                    memory.remove(pageToRemove);
                }
                memory.add(ref);
            }
        }
        return faults;
    }
}
