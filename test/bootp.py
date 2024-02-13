from scapy.all import *
import binascii

# Adresse MAC de la machine cliente (à remplacer par la valeur appropriée)
mac_client = "00:11:22:33:44:55"

# Convertir l'adresse MAC en format binaire
mac_binary = binascii.unhexlify(mac_client.replace(":", ""))

# Créer le paquet BOOTP/DHCP
bootp_packet = (
    Ether(src=mac_client, dst="ff:ff:ff:ff:ff:ff")
    / IP(src="0.0.0.0", dst="255.255.255.255")
    / UDP(sport=68, dport=67)
    / BOOTP(op=1, chaddr=mac_binary)
    / DHCP(options=[("message-type", "discover"), "end"])
)

# Afficher les détails du paquet
bootp_packet.show()

# Envoyer le paquet sur le réseau
wrpcap("data/bootp.pcap", [bootp_packet])
