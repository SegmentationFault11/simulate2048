#pragma once

#include <mutex>
#include <condition_variable>
#include <iostream>

class RW_lock{
private:
    int num_writers;
    int num_readers;
    std::condition_variable waiting_reader;
    std::condition_variable waiting_writer;
    std::mutex rw_lock;

public:
    RW_lock();
    ~RW_lock();

    void read_lock();
    void write_lock();
    void read_unlock();
    void write_unlock();

};
