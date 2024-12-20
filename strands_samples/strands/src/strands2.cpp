#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <iostream>

boost::mutex global_stream_lock;

void WorkerThread(boost::shared_ptr<boost::asio::io_context> iosvc, int counter)
{
    global_stream_lock.lock();
    std::cout << "Thread " << counter << " Start.\n";
    global_stream_lock.unlock();

    iosvc->run();

    global_stream_lock.lock();
    std::cout << "Thread " << counter << " End.\n";
    global_stream_lock.unlock();
}

void Print(int number)
{
    std::cout << "Number: " << number << std::endl;
}

int main(void)
{
    boost::shared_ptr<boost::asio::io_context> io_svc(new boost::asio::io_context);

    auto worker = boost::asio::make_work_guard(*io_svc);

    auto strand = boost::asio::make_strand(*io_svc);

    global_stream_lock.lock();
    std::cout << "The program will exit once all work has finished.\n";
    global_stream_lock.unlock();

    boost::thread_group threads;
    for (int i = 1; i <= 5; i++)
        threads.create_thread(boost::bind(&WorkerThread, io_svc, i));

    boost::this_thread::sleep(boost::posix_time::milliseconds(500));

    boost::asio::post(strand, boost::bind(&Print, 1));
    boost::asio::post(strand, boost::bind(&Print, 2));
    boost::asio::post(strand, boost::bind(&Print, 3));
    boost::asio::post(strand, boost::bind(&Print, 4));
    boost::asio::post(strand, boost::bind(&Print, 5));

    worker.reset();

    threads.join_all();

    return 0;
}