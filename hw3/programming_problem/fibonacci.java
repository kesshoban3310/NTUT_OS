public class fibonacci {
    static class Shared {
        long[] fib;
        int current = 0;
        boolean[] ready;

        Shared(int n) {
            fib = new long[n];
            ready = new boolean[n];
        }
    }

    static class Calculator extends Thread {
        private int n;
        private Shared shared;

        Calculator(int n, Shared shared) {
            this.n = n;
            this.shared = shared;
        }

        @Override
        public void run() {
            synchronized (shared) {
                shared.fib[0] = 0;
                shared.ready[0] = true;
                shared.notifyAll();

                if (n > 1) {
                    shared.fib[1] = 1;
                    shared.ready[1] = true;
                    shared.notifyAll();
                }

                for (int i = 2; i < n; i++) {
                    // 模擬計算延遲
                    try { Thread.sleep(100); } catch (InterruptedException e) {}

                    shared.fib[i] = shared.fib[i - 1] + shared.fib[i - 2];
                    shared.ready[i] = true;
                    shared.notifyAll();
                }
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        if (args.length != 1) {
            System.out.println("Usage: Fibonacci <length>");
            System.exit(1);
        }

        int N = Integer.parseInt(args[0]);
        Shared shared = new Shared(N);
        Calculator cal = new Calculator(N, shared);

        System.out.println("Start computing Fibonacci sequence...");
        long startTime = System.currentTimeMillis();

        cal.start();

        synchronized (shared) {
            for (int i = 0; i < N; i++) {
                while (!shared.ready[i]) {
                    shared.wait();
                }
                long timestamp = System.currentTimeMillis() - startTime;
                System.out.printf("[%4d ms] fib[%d] = %d\n", timestamp, i, shared.fib[i]);
            }
        }

        cal.join();
        System.out.println("Done.");
    }
}
