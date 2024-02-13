from scapy.all import *

# Adresse MAC source et destination
mac_source = "00:11:22:33:44:55"
mac_destination = "66:77:88:99:aa:bb"

# Adresse IP source et destination
ip_source = "192.168.1.2"
ip_destination = "192.168.1.1"

# Construction d'une trame Ethernet avec le protocole IP
eth_packet = Ether(src=mac_source, dst=mac_destination)
ip_packet = IP(
    src=ip_source,
    dst=ip_destination,
    options=[IPOption(b"Record Route"), IPOption(b"No-Operation")],
)

# Construction d'un segment TCP (par exemple) pour tester certaines options IPv4
tcp_packet = TCP(
    dport=80, sport=RandShort(), flags="S"
)  # SYN pour initier la connexion

# Assemblage final de la trame
final_packet = eth_packet / ip_packet / tcp_packet

# Affichage de la trame générée
final_packet.show()

# Enregistrement de la trame dans un fichier pcap
wrpcap("data/ipv4.pcap", [final_packet])
