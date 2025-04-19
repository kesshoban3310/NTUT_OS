
import java.util.*;

public class fibionacci {
    static class Calculator extends Thread{
        private int n;
        private long fib[];

        Calculator(int n,long[] a){
            this.n = n;
            this.fib = a;
        }

        @Override
        public void run(){
            fib[0] = 0;
            fib[1] = 1;
            for(int i = 2; i < n; i++){
                fib[i] = fib[i-1] + fib[i-2];
            }
        }
    }
    public static void main(String args[]) throws InterruptedException {
        if(args.length != 1){
            System.out.println("Usage: fibionacci <Seqlength>");
            System.exit(0);
        }
        int N = Integer.parseInt(args[0]);
        long[] a = new long[N];
        Calculator cal = new Calculator(N,a);

        System.out.println("Start Executing fibonacci sequence");

        cal.start();

        cal.join();
        System.out.println("End Executing fibonacci sequence");

        StringBuilder ans = new StringBuilder();

        for(int i=0;i<N;i++){
            ans.append(cal.fib[i]);
            if(i != N-1){
                ans.append(",");
            }
        }

        System.out.println("Result is:");

        System.out.println(ans);
    }
}
