#include <iostream>
#include <queue>
#include <vector>
#include <unistd.h>
#include "mlfq.h"

using std::cin;
using std::cout;
using std::endl;
using std::queue;
using std::vector;

extern vector<queue<thread_attr *> > mlfq;

int user_job1(void *arg) {
    int time = *(int *)arg;
    cout << "JOB 1: " << "run for " << time << "seconds" << endl;
    sleep(1);
    return time;
}

int user_job2(void *arg) {
    int time = *(int *)arg;
    cout << "JOB 2: " << "run for " << time << "seconds" << endl;
    sleep(1);
    return time;
}

int user_job3(void *arg) {
    int time = *(int *)arg;
    cout << "JOB 3: " << "run for " << time << "seconds" << endl;
    sleep(1);
    return time;
}

int user_job4(void *arg) {
    int time = *(int *)arg;
    cout << "JOB 4: " << "run for " << time << "seconds" << endl;
    sleep(1);
    return time;
}

int main(int argc, char **argv) {
    cout << "### Multi-level feed back queue scheduling ###" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Scheduler Configuration" << endl;
    cout << " Queue | Priority | Time slice" << endl;
    cout << "-------|----------|-----------" << endl;
    cout << "     0 |   High   |         2s" << endl;
    cout << "     1 |  Middle  |         4s" << endl;
    cout << "     2 |    Low   |         8s" << endl;
    cout << "Boost period: " << BOOST_PERIOD << "s" << endl;
    cout << "------------------------------" << endl;

    mlfq = vector<queue<thread_attr *> >(3, queue<thread_attr *>());

    thread_attr *attr1 = new thread_attr();
    attr1->job = user_job1;
    attr1->name = "Alice";
    attr1->priority = PRIORITY_HIGH;
    attr1->quota = periods[PRIORITY_HIGH];
    attr1->remain = periods[PRIORITY_HIGH];
    attr1->arrivetime = 0;
    attr1->executetime = 4;
    attr1->executeremain = ;
    
    thread_attr *attr2 = new thread_attr();
    attr2->name = "Bob";
    attr2->job = user_job2;
    attr2->priority = PRIORITY_HIGH;
    attr2->quota = periods[PRIORITY_HIGH];
    attr2->remain = periods[PRIORITY_HIGH];
    attr1->arrivetime = 0;
    attr1->executetime = 8;
    attr1->executeremain = ;

    thread_attr *attr3 = new thread_attr();
    attr3->name = "Cambridge";
    attr3->job = user_job3;
    attr3->priority = PRIORITY_HIGH;
    attr3->quota = periods[PRIORITY_HIGH];
    attr3->remain = periods[PRIORITY_HIGH];
    attr1->arrivetime = 0;
    attr1->executetime = 16;
    attr1->executeremain = 16;

    thread_attr *attr4 = new thread_attr();
    attr4->name = "David";
    attr4->job = user_job4;
    attr4->priority = PRIORITY_HIGH;
    attr4->quota = periods[PRIORITY_HIGH];
    attr4->remain = periods[PRIORITY_HIGH];
    attr1->arrivetime = 0;
    attr1->executetime = 32;
    attr1->executeremain = 32;

    mlfq.at(PRIORITY_HIGH).push(attr1);
    mlfq.at(PRIORITY_HIGH).push(attr2);
    mlfq.at(PRIORITY_HIGH).push(attr3);
    mlfq.at(PRIORITY_HIGH).push(attr4);

    sched();

    delete attr1;
    delete attr2;
    delete attr3;
    delete attr4;
    return 0;
}