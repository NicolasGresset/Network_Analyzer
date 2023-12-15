# Network analyzer

This project is an academic project that aims to implement a C-like tcpdump.
We use the pcap C-library to get network frames and then parses them so we can display relevant informations about the data : transport protocol, ip-like protocol and applicative level relatives informations.

# Compile

`make clean`
`make`

# Run

You can whether choose to do a live analyze of use previously captured trames from a file. In the first case, use -i option with the name of the interface you want to listen on. Otherwise, use -o option with the name of the file you want to analyze frames from. You can also choose a verbose level to monitor what amount of information you want to be displayed (1 )