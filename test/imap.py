from scapy.all import *

# Adresse MAC source et destination
mac_source = "00:11:22:33:44:55"
mac_destination = "66:77:88:99:aa:bb"

# Adresse IP source et destination
ip_source = "192.168.1.2"
ip_destination = "192.168.1.1"

# Création d'une trame Ethernet avec le protocole IP
eth_packet = Ether(src=mac_source, dst=mac_destination)
ip_packet = IP(src=ip_source, dst=ip_destination)
tcp_packet = TCP(
    dport=143, sport=RandShort(), flags="S"
)  # SYN pour initier la connexion

# Construction du corps du message IMAP
imap_body = b'1 LOGIN user@example.com password\r\n2 LIST "" "*"\r\n3 SELECT inbox\r\n4 FETCH 1 BODY[]\r\n5 LOGOUT\r\n'

# Assemblage final de la trame
final_packet = eth_packet / ip_packet / tcp_packet / Raw(load=imap_body)

# Affichage de la trame générée
final_packet.show()

# Enregistrement de la trame dans un fichier pcap
wrpcap("data/imap.pcap", [final_packet])
