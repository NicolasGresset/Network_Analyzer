from scapy.all import *

# Adresse MAC source et destination
mac_source = "00:11:22:33:44:55"
mac_destination = "66:77:88:99:aa:bb"

# Adresse IP source et destination
ip_source = "192.168.1.2"
ip_destination = "192.168.1.1"

# Ports source et destination pour FTP (commande et données)
ftp_command_port = 21
ftp_data_port = 20

# Construction d'une trame Ethernet avec le protocole IP
eth_packet = Ether(src=mac_source, dst=mac_destination)
ip_packet = IP(src=ip_source, dst=ip_destination)

# Construction d'un segment TCP pour les commandes FTP
tcp_command_packet = TCP(
    dport=ftp_command_port, sport=RandShort(), flags="S"
)  # SYN pour initier la connexion

# Construction d'un segment TCP pour les données FTP
tcp_data_packet = TCP(
    dport=ftp_data_port, sport=RandShort(), flags="S"
)  # SYN pour initier la connexion

# Construction des données FTP (ici, une simple commande USER)
ftp_data = "USER anonymous\r\n"

# Assemblage final des trames
ftp_command_frame = eth_packet / ip_packet / tcp_command_packet
ftp_data_frame = eth_packet / ip_packet / tcp_data_packet / Raw(load=ftp_data)

# Affichage des trames générées
ftp_command_frame.show()
ftp_data_frame.show()

# Enregistrement des trames dans un fichier pcap
wrpcap("data/ftp.pcap", [ftp_command_frame, ftp_data_frame])
