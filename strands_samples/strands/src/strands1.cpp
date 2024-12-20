#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <syncstream>
#include <thread>

boost::mutex global_stream_lock;

void WorkerThread(boost::shared_ptr<boost::asio::io_context> iosvc, int counter)
{
    std::osyncstream sync_stream(std::cout);
    global_stream_lock.lock();
    sync_stream << "Thread " << std::this_thread::get_id() << ", " << counter << " Start." << std::endl;
    ;
    global_stream_lock.unlock();

    iosvc->run();

    global_stream_lock.lock();
    sync_stream << "Thread " << counter << " End." << std::endl;
    global_stream_lock.unlock();
}

void async_send_handler(int number)
{
    std::osyncstream sync_stream(std::cout);
    sync_stream << "Number: " << number << ", threadID: " << std::this_thread::get_id() << std::endl;
}

int main()
{
    boost::shared_ptr<boost::asio::io_context> iosvc(new boost::asio::io_context);

    // boost::asio::io_context::strand strand(*iosvc);
    auto worker = boost::asio::make_work_guard(*iosvc);

    auto strand1 = boost::asio::make_strand(*iosvc);
    auto strand2 = boost::asio::make_strand(*iosvc);

    global_stream_lock.lock();
    std::cout << "The program will exit once all work has finished.\n";
    global_stream_lock.unlock();

    boost::thread_group threads;
    for (int i = 1; i <= 5; i++)
        threads.create_thread(boost::bind(&WorkerThread, iosvc, i));

    boost::asio::post(*iosvc, boost::bind(&async_send_handler, 1));
    boost::asio::post(*iosvc, boost::bind(&async_send_handler, 2));
    boost::asio::post(*iosvc, boost::bind(&async_send_handler, 3));
    boost::asio::post(*iosvc, boost::bind(&async_send_handler, 4));
    boost::asio::post(*iosvc, boost::bind(&async_send_handler, 5));

    boost::this_thread::sleep(boost::posix_time::milliseconds(100));

    // This code will do what you exactly want;
    // It will execute the handlers sequentially in that order
    boost::asio::post(strand1, boost::bind(&async_send_handler, 1));
    boost::asio::post(strand1, boost::bind(&async_send_handler, 2));
    boost::asio::post(strand1, boost::bind(&async_send_handler, 3));
    boost::asio::post(strand1, boost::bind(&async_send_handler, 4));
    boost::asio::post(strand1, boost::bind(&async_send_handler, 5));
    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    boost::asio::post(strand2, boost::bind(&async_send_handler, 1));
    boost::asio::post(strand2, boost::bind(&async_send_handler, 2));
    boost::asio::post(strand2, boost::bind(&async_send_handler, 3));
    boost::asio::post(strand2, boost::bind(&async_send_handler, 4));
    boost::asio::post(strand2, boost::bind(&async_send_handler, 5));

    worker.reset();

    threads.join_all();

    return 0;
}
