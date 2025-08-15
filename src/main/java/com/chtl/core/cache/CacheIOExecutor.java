package com.chtl.core.cache;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;


import java.util.concurrent.*;

/**
 * 缓存IO执行器
 * 处理异步缓存IO操作
 */
public class CacheIOExecutor {
    private static final ExecutorService executor;
    
    static {
        // 创建专门的IO线程池
        ThreadFactory threadFactory = new ThreadFactory() {
            private int counter = 0;
            
            @Override
            public Thread newThread(Runnable r) {
                Thread thread = new Thread(r);
                thread.setName("chtl-cache-io-" + counter++);
                thread.setDaemon(true);
                thread.setPriority(Thread.MIN_PRIORITY);
                return thread;
            }
        };
        
        executor = new ThreadPoolExecutor(
            2,                          // 核心线程数
            4,                          // 最大线程数
            60L,                        // 空闲时间
            TimeUnit.SECONDS,
            new LinkedBlockingQueue<>(1000),  // 队列容量
            threadFactory,
            new ThreadPoolExecutor.DiscardOldestPolicy()  // 丢弃最旧的任务
        );
    }
    
    public static void submit(Runnable task) {
        executor.submit(task);
    }
    
    public static void shutdown() {
        executor.shutdown();
        try {
            if (!executor.awaitTermination(10, TimeUnit.SECONDS)) {
                executor.shutdownNow();
            }
        } catch (InterruptedException e) {
            executor.shutdownNow();
        }
    }
}