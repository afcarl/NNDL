#include <iostream>

#include "signal.h"

#include "simple_synapse.h"


SimpleSynapse::SimpleSynapse()
{
}

SimpleSynapse::SimpleSynapse(const Neuron *from, const Neuron *to, const Signal &w)
{
    this->from = from;
    this->to = to;
    this->last_fired = Signal(0);
    this->weight = w;
    this->batch_sum = Signal(0);
}

SimpleSynapse::~SimpleSynapse()
{
}

std::ostream& operator<<(std::ostream &outstream, const SimpleSynapse &s)
{
    outstream << "Synapse: " << s.get_weight() << " (";
    outstream << s.get_from()->get_id();
    outstream << ", " << s.get_to()->get_id() << ")";

    return outstream;
}

Signal SimpleSynapse::fire_backward(uint64_t t, const Signal &incoming)
{
    std::cout << "d_o: " << incoming << std::endl;
    static const Signal learning_rate(0.6);
    this->batch_sum += -learning_rate * incoming * this->last_input;

    Signal ret(this->weight * incoming);

    if (t % BATCH_SIZE == 0)
        this->weight += this->batch_sum;

    return ret;
}

Signal SimpleSynapse::fire_forward(uint64_t t, const Signal &incoming)
{
    const Signal dI(0.1);
    this->last_input = incoming + dI;
    this->last_fired = (incoming + dI) * this->weight;
    return this->last_fired;
}








