#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include "consumer.hpp"
#include "ts_queue.hpp"
#include "item.hpp"
#include "transformer.hpp"

#ifndef CONSUMER_CONTROLLER
#define CONSUMER_CONTROLLER

class ConsumerController : public Thread {
public:
	// constructor
	ConsumerController(
		TSQueue<Item*>* worker_queue,
		TSQueue<Item*>* writer_queue,
		Transformer* transformer,
		int check_period,
		int low_threshold,
		int high_threshold
	);

	// destructor
	~ConsumerController();

	virtual void start();

private:
	std::vector<Consumer*> consumers;

	TSQueue<Item*>* worker_queue;
	TSQueue<Item*>* writer_queue;

	Transformer* transformer;

	// Check to scale down or scale up every check period in microseconds.
	int check_period;
	// When the number of items in the worker queue is lower than low_threshold,
	// the number of consumers scaled down by 1.
	int low_threshold;
	// When the number of items in the worker queue is higher than high_threshold,
	// the number of consumers scaled up by 1.
	int high_threshold;

	static void* process(void* arg);
};

// Implementation start

ConsumerController::ConsumerController(
	TSQueue<Item*>* worker_queue,
	TSQueue<Item*>* writer_queue,
	Transformer* transformer,
	int check_period,
	int low_threshold,
	int high_threshold
) : worker_queue(worker_queue),
	writer_queue(writer_queue),
	transformer(transformer),
	check_period(check_period),
	low_threshold(low_threshold),
	high_threshold(high_threshold) {
}

ConsumerController::~ConsumerController() {}

void ConsumerController::start() {
	// TODO: starts a ConsumerController thread
	// ConsumerController runs in a
	// thread that controls the number of consumer threads dynamically.
	// same. use pthread_create to create one thread, and the details are in process
	pthread_create(&t, nullptr, ConsumerController::process, this);
}

void* ConsumerController::process(void* arg) {
	// TODO: implements the ConsumerController's work
	ConsumerController* controller = (ConsumerController*)arg;
	while(1){
		int size = controller->worker_queue->get_size();
		if(size < controller->low_threshold && controller->consumers.size() > 1) {
			Consumer* consumer = controller->consumers.back();
            consumer->cancel();
            controller->consumers.pop_back();
			// printf("size = %d, low = %d\n", size, controller->low_threshold);
			printf("Scaling down consumers from %d to %d\n", controller->consumers.size()+1, controller->consumers.size());
		}
		else if(size > controller->high_threshold){
			Consumer* new_consumer = new Consumer(controller->worker_queue, controller->writer_queue, controller->transformer);
            new_consumer->start();
            controller->consumers.push_back(new_consumer);
			// printf("size = %d, high = %d\n", size, controller->high_threshold);
			printf("Scaling up consumers from %d to %d\n", controller->consumers.size()-1, controller->consumers.size());
		}
	}

	return NULL;
}

#endif // CONSUMER_CONTROLLER_HPP
