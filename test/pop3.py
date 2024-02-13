from scapy.all import *

# Adresse MAC source et destination
mac_source = "00:00:00:00:00:01"
mac_destination = "00:00:00:00:00:02"

# Création d'une trame Ethernet avec le protocole IP
ether_packet = Ether(src=mac_source, dst=mac_destination, type=0x0800)

# Adresse IP source et destination
ip_source = "192.168.1.2"
ip_destination = "192.168.1.1"

# Création d'une trame IP avec le protocole TCP
ip_packet = IP(src=ip_source, dst=ip_destination)
tcp_packet = TCP(
    dport=110, sport=RandShort(), flags="S"
)  # SYN pour initier la connexion

# Construction du corps du message POP3
pop3_body = b"USER user\r\nPASS password\r\nLIST\r\nRETR 1\r\nQUIT\r\n"

# Assemblage final de la trame
final_packet = ether_packet / ip_packet / tcp_packet / Raw(load=pop3_body)

# Affichage de la trame générée
final_packet.show()

# Enregistrement de la trame dans un fichier
wrpcap("data/pop3.pcap", [final_packet])
