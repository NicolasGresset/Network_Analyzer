# Network analyzer

This project is an academic project that aims to implement a C-like tcpdump.
We use the pcap C-library to get network frames and then parses them in order to display relevant informations about the data : transport protocol, ip-like protocol and applicative level relatives informations.

# Dependencies

This project makes use of the [pcap library](https://github.com/the-tcpdump-group/libpcap) to capture the frames

# Build

Clone the repository using
```bash
git clone https://github.com/NicolasGresset/network_analyzer.git
```
at the place of your convenience then enters the folder with
```bash
cd network_analyzer
```

Therefore, you can build the project with

```bash
make clean
```  
```bash
make
```

# Run

You can whether choose to do a live analysis or use previously captured trames from a file, which will be referred as offline analysis.

## Live analysis

Run
```bash
./bin/sniffer -i interface_name -v verbose_level [-f filter]
```
The interface name refers to the one you can find with
```bash
ifconfig
```
e.g wlp2s0

Verbose level must be between 1 and 3, 1 being the less explanatory and 3 the more verbose

Filters are optional BPF filters and you can find syntax examples [`here`](https://biot.com/capstats/bpf.html)

Therefore, a complete utilization could look like
```bash
./bin/sniffer -i wlp2s0 -v 3 -f "port 80"
```

## Offline analysis

In offline mode, you should take as input a file you have previously obtained from network analysers tools such as [tcpdump](https://www.tcpdump.org/) or a packet generation tool like [scapy](https://scapy.net/)

Example command
```bash
./bin/sniffer -o packet1.pcap -v 2
```