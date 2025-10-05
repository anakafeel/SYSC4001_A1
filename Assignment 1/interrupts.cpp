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

    const int SWITCH_MODE = 1;      // User mode = 1, Kernel mode = 0
    const int SAVE_CONTEXT = 10;    // Context save time
    const int RESTORE_CONTEXT = 10; // Context restore time
    const int FIND_VECTOR = 1;      // Time to find vector
    const int GET_ISR = 10;         // Time to get ISR
    const int IRET_TIME = 1;        // Time for IRET instruction
    const int VECTOR_ENTRY_BYTES = 2;

    long long time = 0; // Current time in the simulation


    // PLEASE DONT MIND THE WEIRD SPACES , THE FORMATTER I USE ADDS THEM AUTOMATICALLY
    
    // created an even struct to store event information
    struct event
    {
        enum Type
        {
            CPU,
            ENDIO,
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
        else if (type == "SYSCALL")
            t = event::SYSCALL;
        else if (type == "END_IO")
            t = event::ENDIO;
        else
            throw std::invalid_argument("Unknown type");

        return event{t, dur};
    };

    // using lambda function to create a function inside main to log event execution
    auto log_event = [&](long long start, long long dur, const std::string &desc)
    {
        execution += std::to_string(start) + "," + std::to_string(dur) + "," + desc + "\n";
    };

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        event e = event_convert(activity, duration_intr);

        // Normal CPU execution
        if (e.type == event::CPU)
        {

            log_event(time, e.dur, "CPU burst");
            time += e.dur;
        }

        else if (e.type == event::SYSCALL)
        {
            // Using boilerplate for interrupt setup
            auto [bp_log, new_time] = intr_boilerplate(time, 0, SAVE_CONTEXT, vectors);
            execution += bp_log;
            time = new_time;

            // ISR body
            log_event(time, e.dur, "Execute SYSCALL ISR body");
            time += e.dur;

            // Restoring Context
            log_event(time, RESTORE_CONTEXT, "Restore context");
            time += RESTORE_CONTEXT;

            // IRET
            log_event(time, IRET_TIME, "IRET");
            time += IRET_TIME;
        }

        // Handling END_IO events
        else if (e.type == event::ENDIO) 
        {
        //StorIng the device number
        int device_num = e.dur; 

        // Using boilerplate for END_IO setup    
        auto [bp_log, new_time] = intr_boilerplate(time, device_num, SAVE_CONTEXT, vectors);
        execution += bp_log;
        time = new_time;

        log_event(time, delays[device_num], 
                  "Complete I/O ISR for device " + std::to_string(device_num));
        time += delays[device_num];

        log_event(time, RESTORE_CONTEXT, "Restore context");
        time += RESTORE_CONTEXT;

        log_event(time, IRET_TIME, "IRET");
        time += IRET_TIME;
        }
        else
        {
            throw std::invalid_argument("Unknown event type");
        }
        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
