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

void boost() {
    for (int i = PRIORITY_MID; i <= PRIORITY_LOW; ++i) {
        // Boost jobs in middle priority queue and low priority queue to high priority queue
	while (!mlfq.at(i).empty()) {
            thread_attr *t = mlfq.at(i).front();
            mlfq.at(i).pop();
            t->priority = PRIORITY_HIGH;
            t->quota = periods[PRIORITY_HIGH];
            t->quotaremain = periods[PRIORITY_HIGH];
            mlfq.at(PRIORITY_HIGH).push(t);
        }
    }
}

void sched() {
    // int empty = 0;
    int counter = 0;
    int i = PRIORITY_HIGH;
    int boostcnt = 1;

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
	    int nextarrive = waiting.empty() ? INT_MAX : waiting.front()->arrivetime;
	    int nextboost = BOOST_PERIOD * boostcnt;
	    int next = MIN(nextarrive, nextboost);
	    bool flag = false;
            if (counter + sec >= next) {
		sec = next - counter;
		flag = true;
	    } 
            
            int cost = t->job((void *) &sec);
            // modify quotaremain and executeremain
            t->quotaremain -= cost;
            t->executeremain -= cost; 
            counter += cost;

            if (t->executeremain != 0) {
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

	    // solve new arrive and boost
	    if (flag && next == nextarrive) {
                mlfq.at(PRIORITY_HIGH).push(waiting.front());
                waiting.pop();
                if (counter >= nextboost) {
                    boost();
                    boostcnt++;
                }
            }
	    else if (flag && next == nextboost) {
                boost();
                boostcnt++;
                if (counter >= nextarrive) {
                    mlfq.at(PRIORITY_HIGH).push(waiting.front());
                    waiting.pop();
                }
            }

        }
    }

    return;
}

