#include <assert.h>
#include <stdlib.h>
#include "ts_queue.hpp"
#include "item.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "producer.hpp"
#include "consumer_controller.hpp"

#define input_queue_SIZE 200
#define WORKER_QUEUE_SIZE 200
#define output_queue_SIZE 4000
#define CONSUMER_CONTROLLER_LOW_THRESHOLD_PERCENTAGE 20
#define CONSUMER_CONTROLLER_HIGH_THRESHOLD_PERCENTAGE 80
#define CONSUMER_CONTROLLER_CHECK_PERIOD 1000000

int main(int argc, char** argv) {
	assert(argc == 4);

	int n = atoi(argv[1]);
	std::string input_file_name(argv[2]);
	std::string output_file_name(argv[3]);

	// TODO: implements main function
	// The description 
	// The program starts with 
	// 1 Reader thread, 4 Producer threads, 0 Consumer thread and 1 Writer thread.

	// so first declare the queues we need (based on the graph in description graph)

	TSQueue<Item*> input_queue(input_queue_SIZE);
	TSQueue<Item*> worker_queue(WORKER_QUEUE_SIZE);
	TSQueue<Item*> output_queue(output_queue_SIZE);

	Transformer transformer;
	Reader reader(n, input_file_name, &input_queue);
	Producer producer1(&input_queue, &worker_queue, &transformer);
	Producer producer2(&input_queue, &worker_queue, &transformer);
	Producer producer3(&input_queue, &worker_queue, &transformer);
	Producer producer4(&input_queue, &worker_queue, &transformer);
	Writer writer(n, output_file_name, &output_queue);
	ConsumerController consumer_controller(&worker_queue, &output_queue, &transformer,
											CONSUMER_CONTROLLER_CHECK_PERIOD, 
											CONSUMER_CONTROLLER_LOW_THRESHOLD_PERCENTAGE * WORKER_QUEUE_SIZE / 100., 
											CONSUMER_CONTROLLER_HIGH_THRESHOLD_PERCENTAGE * WORKER_QUEUE_SIZE/ 100.);

	// all objects are called by start()
	reader.start();
	// printf("reader finish\n");
	writer.start();
	// printf("writer finish\n");
	producer1.start(); producer2.start(); producer3.start(); producer4.start();
	// printf("producer finish\n");
	consumer_controller.start();
	// printf("consumer_controller finish\n");

	// block
	reader.join();
	// printf("reader.join finish\n");
	writer.join();
	// printf("all finish\n");

	return 0;
}
