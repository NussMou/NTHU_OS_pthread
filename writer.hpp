#include <fstream>
#include "thread.hpp"
#include "ts_queue.hpp"
#include "item.hpp"

#ifndef WRITER_HPP
#define WRITER_HPP

class Writer : public Thread {
public:
	// constructor
	Writer(int expected_lines, std::string output_file, TSQueue<Item*>* output_queue);

	// destructor
	~Writer();

	virtual void start() override;
private:
	// the expected lines to write,
	// the writer thread finished after output expected lines of item
	int expected_lines;

	std::ofstream ofs;
	TSQueue<Item*> *output_queue;
	// the method for pthread to create a writer thread
	static void* process(void* arg);
};

// Implementation start

Writer::Writer(int expected_lines, std::string output_file, TSQueue<Item*>* output_queue)
	: expected_lines(expected_lines), output_queue(output_queue) {
	ofs = std::ofstream(output_file);
}

Writer::~Writer() {
	ofs.close();
}

void Writer::start() {
	// TODO: starts a Writer thread
	// Writer runs in a thread that reads Item from the Output Queue 
	// then writes Items into the output file.
	// so same to the reader file, pthread_create and using a while loop
	pthread_create(&t, nullptr, Writer::process, this);
}

void* Writer::process(void* arg) {
	// TODO: implements the Writer's work
	Writer* writer = (Writer* )arg;
	while(writer->expected_lines--){
		Item* item = writer->output_queue->dequeue();
		writer->ofs << *item;
		// printf("writer join debug : %d\n", writer->expected_lines);
	}
	return NULL;
}

#endif // WRITER_HPP
