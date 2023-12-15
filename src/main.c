#include "../include/handler.h"
#include "../include/options.h"
#include "../include/capture.h"

pcap_t *capture;

int main(int argc, char **argv) {
    options opt;    
    manage_options(argc, argv, &opt);
    print_options(&opt);

    capture = open_capture(&opt);
    manage_filter(capture, &opt);
    start_loop(capture, handler, &opt);

    pcap_close(capture);
    return 0;
}