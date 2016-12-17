#include "rw_lock.h"
#include <iostream>

using namespace std;

RW_lock::RW_lock() {
    num_writers = 0;
    num_readers = 0;
}

RW_lock::~RW_lock() {}

void RW_lock::read_lock() {
    std::unique_lock<std::mutex> guard(rw_lock);
    while(num_writers > 0) {
        waiting_reader.wait(guard);
    }
    num_readers++;
}

void RW_lock::write_lock() {
    std::unique_lock<std::mutex> guard(rw_lock);
    while(num_writers > 0 || num_readers > 0) {
        waiting_writer.wait(guard);
    }
    num_writers++;
}

void RW_lock::read_unlock() {
    std::unique_lock<std::mutex> guard(rw_lock);
    num_readers--;
    if(num_readers == 0) {
        waiting_writer.notify_one();
    }
}

void RW_lock::write_unlock() {
    std::unique_lock<std::mutex> guard(rw_lock);
    num_writers--;
    waiting_writer.notify_one();
    waiting_reader.notify_all();
}

