#include <queue>
#include <vector>
#include <ctime>
#include <iostream>
#include "mlfq.h"

using std::queue;
using std::vector;
using std::cout;
using std::endl;

vector<queue<thread_attr *> > mlfq;
queue<thread_attr *> waiting;

void sched() {
    // int empty = 0;
    int counter = 0;
    int i = PRIORITY_HIGH;
    bool boost = false;
    while (!mlfq.empty() && !(mlfq.at(PRIORITY_HIGH).empty() && mlfq.at(PRIORITY_MID).empty() && mlfq.at(PRIORITY_LOW).empty())) {
        // mlfq has three queues corresponding to high priority queue, middle priority queue and low priority queue

        // find an unempty queue with higher priority
        i = PRIORITY_HIGH;
        while (i != PRIORITY_LOW && mlfq.at(i).empty()) ++i; 

        // get a job
        {
            cout << "[SCHEDULER] Queue " << colored_queues[i] << "\t";
            thread_attr *t = mlfq.at(i).front();
	        mlfq.at(i).pop();
	    
            // run job for sec seconds
            int sec = MIN(t->quotaremain, t->executeremain);

            // preemption
            if (!waiting.empty()) {
                int nextarrive = waiting.front()->arrivetime;
                if (counter + sec >= nextarrive) {
                    sec = nextarrive - counter;
                    mlfq.at(PRIORITY_HIGH).push(waiting.front())
                    waiting.pop();
                }
            }
            
            int cost = t->job((void *) &sec);
            // modify quotaremain and executeremain
            t->quotaremain -= cost;
            t->executeremain -= cost; 
            // time to boost
            counter += cost;
            if (counter > BOOST_PERIOD) {
                boost = true;
                // break;
            }

            if (cost < sec) {
                mlfq.at(i).push(t);
            }
            else if (t->executeremain != 0) {
                // move t to a queue with lower priority
                int next = i;
                if (next != PRIORITY_LOW) {
                    t->priority = ++next;
                    t->quota = periods[next];
                }
                // if t's priority is LOW, reset the remain and push back 
                t->quotaremain = periods[next];
                mlfq.at(next).push(t);
            }
        }

        if (boost) {
            counter = counter % BOOST_PERIOD;  // FIXME: reset 0 or mod by BOOST_PERIOD
            for (int j = PRIORITY_MID; j != PRIORITY_LOW + 1; ++j) {
                // Boost jobs in middle priority queue and low priority queue to high priority queue
                while (!mlfq.at(j).empty()) {
                    thread_attr *t = mlfq.at(i).front();
                    mlfq.at(i).pop();
                    t->priority = PRIORITY_HIGH;
                    t->quota = periods[PRIORITY_HIGH];
                    t->quotaremain = periods[PRIORITY_HIGH];
                    mlfq.at(PRIORITY_HIGH).push(t);
                }
            }
            // reset to round robin high priority queue
            i = PRIORITY_HIGH;
            boost = false;
        }
    }

    return;
}
