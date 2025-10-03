/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include <interrupts.hpp>

int main(int argc, char **argv)
{

    // vectors is a C++ std::vector of strings that contain the address of the ISR
    // delays  is a C++ std::vector of ints that contain the delays of each device
    // the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;     //!< string to store single line of trace file
    std::string execution; //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/

    int SWITCH_MODE = 1;      // User mode = 1, Kernel mode = 0
    int SAVE_CONTEXT = 10;    // Context save time
    int RESTORE_CONTEXT = 10; // Context restore time
    int FIND_VECTOR = 1;      // Time to find vector
    int GET_ISR = 10;         // Time to get ISR
    const IRET_TIME = 1;      // Time for IRET instruction
    const int VECTOR_ENTRY_BYTES = 2;

    long long time = 0; // Current time in the simulation


    // PLEASE DONT MIND THE WEIRD SPACES , THE FORMATTER I USE ADDS THEM AUTOMATICALLY
    // created an even struct to store event information
    struct event
    {
        enum Type   
        {
            CPU,
            IO,
            SYSCALL
        };
        Type type;
        int dur;
    };

    // using lambda funciton to crea te a function inside main to convert trace type to event type
    auto event_convert = [](std::string type, int dur)
    {
        event::Type t;

        if (type == "CPU")
            t = event::CPU;
        else if (type == "IO")
            t = event::IO;
        else if (type == "SYSCALL")
            t = event::SYSCALL;
        else
            throw std::invalid_argument("Unknown type");

        return event{t, dur};
    };

    // using lambda function to create a function inside main to log event execution
    auto log_event = [&](long long start, long long dur, const std::string &desc)
    {
        execution += std::to_string(start) + " " + std::to_string(dur) + " " + desc + "\n";
    };

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
