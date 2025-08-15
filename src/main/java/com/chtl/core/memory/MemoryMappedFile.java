package com.chtl.core.memory;
import java.io.File;
import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;


import java.io.*;
import java.nio.*;
import java.nio.channels.*;

public class MemoryMappedFile implements AutoCloseable {
    private final RandomAccessFile file;
    private final FileChannel channel;
    private final MappedByteBuffer buffer;

    public MemoryMappedFile(File file, long size) throws IOException {
        this.file = new RandomAccessFile(file, "rw");
        this.channel = this.file.getChannel();
        this.buffer = channel.map(FileChannel.MapMode.READ_WRITE, 0, size);
    }

    public MappedByteBuffer getBuffer() {
        return buffer;
    }

    @Override
    public void close() throws IOException {
        channel.close();
        file.close();
    }
}
